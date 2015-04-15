#include <sstream>


#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>

#include <llvm/Support/raw_ostream.h>

#include "llvmFunction2nts.hpp"
#include "ModuleMapping.hpp"
#include "FunctionMapping.hpp"

//#include "instructions/Constants.hpp"
//#include "instructions/InstAdd.hpp"
#include "instructions/InstLoadStore.hpp"
#include "instructions/InstAlloca.hpp"
//#include "instructions/InstBr.hpp"
//#include "instructions/InstIcmp.hpp"
#include "instructions/InstCall.hpp"




using namespace llvm;
using namespace nts;
using std::unique_ptr;
using std::string;
using std::to_string;

class Context
{
	public:
		Context ()// :
//			m_ia   ( m_constants ),
//			m_icmp ( m_constants )
		{

		}

		//Constants     m_constants;
		//InstAdd       m_ia;
		InstLoadStore m_ils;
		InstAlloca    m_alloca;
		//InstBr        m_ibr;
		//InstIcmp      m_icmp;
		InstCall      m_icall;
};


static Context ctx;

#if 0
const State * llvmFunction2nts::process_instruction (
		const State       * st_from ,
		const Instruction & i       )
{
	switch(i.getOpcode())
	{
		case Instruction::Alloca:
			break;

		case Instruction::Store:
		case Instruction::Load:
		case Instruction::Ret:
			return ctx.m_ils.process   ( st_from, i, m_map, m_nts, m_bb_id, m_inst_id );

		case Instruction::Add:
			return ctx.m_ia.process    ( st_from, i, m_map, m_nts, m_bb_id, m_inst_id );

		case Instruction::Br:
			return ctx.m_ibr.process   ( st_from, i, m_map, m_nts, m_bb_id, m_inst_id );

		case Instruction::ICmp:
			return ctx.m_icmp.process  ( st_from, i, m_map, m_nts, m_bb_id, m_inst_id );

		case Instruction::Call:
			return ctx.m_icall.process ( st_from, i, m_map, m_nts, m_bb_id, m_inst_id );

	}

	return st_from;
}
#endif

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
	for ( const auto & b : fun.getBasicBlockList() )
	{
		auto name = string ( "st_" ) + to_string ( bb_id ) + "_0";
		auto st   = new State ( std::move ( name ) );
		st->insert_to ( bni.bn );
		// I am not the owner of st anymore
		auto i     = unique_ptr<StateInfo> ( new StateInfo ( *st, b ) );
		i->bb_id   = 0;
		i->inst_id = 0;

		funmap.ins_bb_start ( b, std::move ( i ) );
		bb_id++;
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
	for ( const auto & bbi : funmap.bbinfo() )
	{
		process_basic_block ( *bbi.getSecond());
	}
}

void fun_llvm_2_nts::process_basic_block ( const StateInfo & bbi )
{
	StateInfo st = bbi;
	st.inst_id = 1;
	st.st->is_initial() = true;
	for ( const auto &i : bbi.bb->getInstList() )
	{
		process_instruction ( i, st );
		st.inst_id++;
	}
}

void fun_llvm_2_nts::process_instruction ( const llvm::Instruction & i, StateInfo & st )
{
	switch ( i.getOpcode() )
	{
		case Instruction::Alloca:
			return ctx.m_alloca.process ( bni, st, funmap, i );
			break;

		case Instruction::Call:
			return ctx.m_icall.process ( bni, st, funmap, i );

		case Instruction::Store:
		case Instruction::Load:
		case Instruction::Ret:
			return ctx.m_ils.process ( bni, st, funmap, i );

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
