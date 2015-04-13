#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>

#include <llvm/Support/raw_ostream.h>

#include "llvmFunction2nts.hpp"

#include "instructions/Constants.hpp"
#include "instructions/InstAdd.hpp"
#include "instructions/InstLoadStore.hpp"
#include "instructions/InstBr.hpp"
#include "instructions/InstIcmp.hpp"
#include "instructions/InstCall.hpp"


#include <sstream>

#include "ModuleMapping.hpp"
#include "FunctionMapping.hpp"

using namespace llvm;
using namespace nts;
using std::unique_ptr;
using std::string;
using std::to_string;

class Context
{
	public:
		Context () :
			m_ia   ( m_constants ),
			m_icmp ( m_constants )
		{

		}

		Constants     m_constants;
		InstAdd       m_ia;
		InstLoadStore m_ils;
		InstBr        m_ibr;
		InstIcmp      m_icmp;
		InstCall      m_icall;
};


static Context ctx;

llvmFunction2nts::llvmFunction2nts ( const Function      & f,
									 BasicNts            & nts,
									 const ModuleMapping & modmap ) :
	m_f   ( f ),
	m_map ( nts, modmap ),
	m_nts ( nts )
{

	m_bb_id = m_inst_id = 0;
}

llvmFunction2nts::~llvmFunction2nts()
{

}

const NTS::Variable * llvmFunction2nts::add_param ( const Argument * arg )
{
	const NTS::Variable *v = m_nts.add_argument("arg_" + arg->getName().str());
	m_map.ins_iprint ( cast<Value>(arg), v );
	return v;
}

void llvmFunction2nts::process (  )
{
	for ( const auto &v : m_f.getArgumentList() )
	{
		add_param ( &v );
	}

	m_bb_id = 0;
	for ( const auto &b : m_f.getBasicBlockList() )
	{
		m_map.ins_bb_start ( &b, m_nts.addState ( m_bb_id, 0 ) );
		m_bb_id ++;
	}

	m_bb_id = 0;
	for ( const auto &b : m_f.getBasicBlockList() )
	{
		process_basic_block ( b );
		m_bb_id ++;
	}
}

void llvmFunction2nts::process_basic_block ( const llvm::BasicBlock &b)
{
	m_inst_id = 1; // 0 is initial bb state
	const NTS::State * s = m_map.get_bb_start(&b);
	for ( const auto &i : b.getInstList() )
	{
		// assert( s != NULL );
		s = process_instruction( s, i );
		m_inst_id ++;
	}
}

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
		process_basic_block ( *bbi.getSecond().get());
	}
}

void fun_llvm_2_nts::process_basic_block ( const StateInfo & bbi )
{
	StateInfo st = bbi;
	st.inst_id = 1;
	for ( const auto &i : bbi.bb->getInstList() )
	{
		process_instruction ( i, st );
		st.inst_id++;
	}
}

void fun_llvm_2_nts::process_instruction ( const llvm::Instruction & i, StateInfo & st )
{

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
