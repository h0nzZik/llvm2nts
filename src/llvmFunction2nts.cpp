#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>

#include <llvm/Support/raw_ostream.h>

#include "llvmFunction2nts.hpp"

#include "instructions/InstAdd.hpp"
#include "instructions/InstLoadStore.hpp"
#include "instructions/InstBr.hpp"


#include <sstream>

using namespace llvm;
using namespace NTS;

static InstAdd ia;
static InstLoadStore ils;
static InstBr ibr;

llvmFunction2nts::llvmFunction2nts ( const Function &f, BasicNts &nts ) :
	m_f   ( f ),
	m_map ( nts ),
	m_nts ( nts )
{

	m_bb_id = m_inst_id = 0;
}

llvmFunction2nts::~llvmFunction2nts()
{

}

const NTS::Variable * llvmFunction2nts::add_param ( const Argument * arg )
{
	const NTS::Variable *v = m_nts.add_argument(arg->getName().str());
	m_map.ins_iprint ( cast<Value>(arg), v );
	return v;
}

void llvmFunction2nts::process (  )
{
	errs()  << "process function\n";
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
	//i.print(errs());
	//errs() << "\n";

	switch(i.getOpcode())
	{
		case Instruction::Alloca:
			break;

		case Instruction::Store:
		case Instruction::Load:
		case Instruction::Ret:
			return ils.process ( st_from, i, m_map, m_nts, m_bb_id, m_inst_id );

		case Instruction::Add:
			return ia.process  ( st_from, i, m_map, m_nts, m_bb_id, m_inst_id );

		case Instruction::Br:
			return ibr.process ( st_from, i, m_map, m_nts, m_bb_id, m_inst_id );

	}

	return st_from;
}

