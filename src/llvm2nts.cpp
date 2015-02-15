
#include <llvm/Pass.h>
#include <llvm/IR/ValueMap.h>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Constants.h>

#include "llvm2nts.hpp"

// C++ STL headers
#include <stdexcept>
#include <ostream>
#include <sstream>

#include "nts/NTS.hpp"
#include "nts/VariableIdentifier.hpp"
#include "nts/AtomicRelation.hpp"
#include "nts/Havoc.hpp"

#include "instructions/InstAdd.hpp"
#include "instructions/InstLoadStore.hpp"
#include "instructions/InstBr.hpp"

using namespace llvm;
using namespace NTS;

// TODO: Support types

// TODO: try argument promotion pass (-argpromotion)

llvm2nts::llvm2nts(const llvm::Type *t)
{
	m_return_type = t;

	NTS::Variable *v = new NTS::Variable("ret_var");
	m_nts.setRetVar(v);
	m_return_var = v;
}

llvm2nts::~llvm2nts()
{
	delete m_return_var;
}


void llvm2nts::print(std::ostream &o) const
{
	m_nts.print(o);
}


const NTS::Variable * llvm2nts::addParam(const Argument * arg)
{
	Variable *v = new Variable(std::string("arg_") + arg->getName().str());
	m_nts.addArgument(v);
	m_vm.insVariable(cast<Value>(arg), v);
	return v;
}

static InstAdd ia;
static InstLoadStore ils;
static InstBr ibr;

void llvm2nts::process_basic_block ( const llvm::BasicBlock &b, int bb_id )
{
	int inst_id = 1; // 0 is initial bb state
	const NTS::State * s = m_vm.get_bb_start(&b);
	for (const auto &i : b.getInstList())
	{
		// assert( s != NULL );
		s = process_instruction(s, i, bb_id, inst_id);
		inst_id ++;
	}
}

void llvm2nts::process_function ( const llvm::Function &f )
{
	errs()  << "process function\n";
	for ( const auto &v : f.getArgumentList() )
	{
		addParam ( &v );
	}

	int bb_id = 0;
	for ( const auto &b : f.getBasicBlockList() )
	{
		m_vm.ins_bb_start ( &b, &m_nts.addState ( bb_id, 0 ) );
		bb_id ++;
	}

	bb_id = 0;
	for ( const auto &b : f.getBasicBlockList() )
	{
		process_basic_block ( b, bb_id );
		bb_id ++;
	}
}

const State * llvm2nts::process_instruction (
		const State       * st_from ,
		const Instruction & i       ,
		int                 bb_id   ,
		int                 inst_id )
{
	i.print(errs());
	errs() << "\n";

	switch(i.getOpcode())
	{
		case Instruction::Alloca:
			break;

		case Instruction::Store:
		case Instruction::Load:
		case Instruction::Ret:
			return ils.process ( st_from, i, m_vm, m_nts, bb_id, inst_id );

		case Instruction::Add:
			return ia.process  ( st_from, i, m_vm, m_nts, bb_id, inst_id );

		case Instruction::Br:
			return ibr.process ( st_from, i, m_vm, m_nts, bb_id, inst_id );

	}

	return st_from;
}

