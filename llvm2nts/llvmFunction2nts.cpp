#include <sstream>
#include <iostream>

#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>

#include <llvm/Support/raw_ostream.h>

#include "llvmFunction2nts.hpp"
#include "ModuleMapping.hpp"
#include "FunctionMapping.hpp"

#include "instructions/InstAdd.hpp"
#include "instructions/InstLoadStore.hpp"
#include "instructions/InstAlloca.hpp"
#include "instructions/InstBr.hpp"
#include "instructions/InstIcmp.hpp"
#include "instructions/InstCall.hpp"




using namespace llvm;
using namespace nts;
using std::unique_ptr;
using std::string;
using std::to_string;

class Context
{
	public:
		InstAdd       m_ia;
		InstLoadStore m_ils;
		InstAlloca    m_alloca;
		InstBr        m_ibr;
		InstIcmp      m_icmp;
		InstCall      m_icall;
};


static Context ctx;

struct fun_llvm_2_nts
{
	const llvm::Function & fun;
	const BasicNtsInfo & bni;
	FunctionMapping funmap;

	fun_llvm_2_nts (
			const llvm::Function & fun,
			const BasicNtsInfo   & bni,
			const ModuleMapping  & mm) :
		fun ( fun ),
		bni ( bni ),
		funmap ( bni.bn, mm )
	{
		;
	}

	void create_basicblock_mapping();
	void create_param_mapping();
	void process_basic_blocks();

	void process_basic_block ( const StateInfo & bbi );
	void process_instruction ( const llvm::Instruction & i, StateInfo & st );
};

void fun_llvm_2_nts::create_basicblock_mapping()
{
	unsigned int bb_id = 0;
	const auto & l = fun.getBasicBlockList();
	for ( const auto & b : l )
	{
		auto name = string ( "st_" ) + to_string ( bb_id ) + "_0";
		auto st   = new State ( std::move ( name ) );
		st->insert_to ( bni.bn );
		// I am not the owner of st anymore
		auto i     = unique_ptr<StateInfo> ( new StateInfo ( *st, b ) );
		i->bb_id   = bb_id;
		i->inst_id = 0;

		funmap.ins_bb_start ( b, std::move ( i ) );
		bb_id++;
	}

	// First basic block has initial state
	if ( l.size() != 0 )
	{
		const BasicBlock * first = l.begin();
		StateInfo & si = * funmap.bbinfo().find ( first )->getSecond();
		si.st->is_initial() = true;
	}
}

void fun_llvm_2_nts::create_param_mapping()
{
	for ( const auto & arg : bni.args )
	{
		funmap.ins_variable ( *arg.getFirst(), *arg.getSecond() );
	}
}

void fun_llvm_2_nts::process_basic_blocks()
{
	// We need to process the basic blocks
	// in 'natural' order, because later
	// blocks may need variables
	// defined in earlier blocks.
	// For this reason, we can not iterate
	// over funmap.bbinfo
	//
	// TODO: To support phi instructions,
	// all registers must be mapped to variables
	// before processing BasicBlocks.
	// When that is met, we could iterate there
	// over funmap.bbinfo.

	for ( const BasicBlock & b : fun.getBasicBlockList() )
	{
		auto it = funmap.bbinfo().find ( & b );
		process_basic_block ( * it->getSecond() );
	}
}

void fun_llvm_2_nts::process_basic_block ( const StateInfo & bbi )
{
	StateInfo st = bbi;
	st.inst_id = 1;
	for ( const auto &i : bbi.bb->getInstList() )
	{
		try {
			process_instruction ( i, st );
		} catch ( const std::exception & e ) {
			errs() << "An exception was thrown while processing ";
			i.print ( errs() );
			errs() << "\n";
			errs() << "what: " << string(e.what()) << "\n";
			throw e;
		}
		st.inst_id++;
	}
}

void fun_llvm_2_nts::process_instruction ( const llvm::Instruction & i, StateInfo & st )
{
	switch ( i.getOpcode() )
	{
		case Instruction::Alloca:
			return ctx.m_alloca.process ( bni, st, funmap, i );

		case Instruction::Call:
			return ctx.m_icall.process ( bni, st, funmap, i );

		case Instruction::Store:
		case Instruction::Load:
		case Instruction::Ret:
			return ctx.m_ils.process ( bni, st, funmap, i );

		case Instruction::Add:
			return ctx.m_ia.process ( bni, st, funmap, i );

		case Instruction::ICmp:
			return ctx.m_icmp.process ( bni, st, funmap, i );

		case Instruction::Br:
			return ctx.m_ibr.process ( bni, st, funmap, i );

		default:
		{
			string s ( "Instruction not implemented: " );
			throw std::domain_error ( ( s + i.getOpcodeName() ).c_str() );
		}

	}
}

void convert_function (
		const llvm::Function & f,
		BasicNtsInfo         & bni,
		ModuleMapping        & mm )
{
	fun_llvm_2_nts fl2n ( f, bni, mm );
	fl2n.create_basicblock_mapping();
	fl2n.create_param_mapping();
	fl2n.process_basic_blocks();	
}
