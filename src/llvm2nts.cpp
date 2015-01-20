
#include <llvm/Pass.h>
// Note: In version 3.5 ValueMap is inside IR directory
#include <llvm/ADT/ValueMap.h>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/Function.h>
// Note: In version 3.5 InstIterator is inside IR directory
#include <llvm/Support/InstIterator.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Constants.h>

#include "llvm2nts.hpp"

// C++ STL headers
#include <stdexcept>
#include <ostream>
#include <sstream>

#include "nts/NTS.hpp"
//#include "nts/Arithmetic.hpp"
#include "nts/VariableIdentifier.hpp"
#include "nts/AtomicRelation.hpp"
#include "nts/Havoc.hpp"

#include "instructions/InstAdd.hpp"
#include "instructions/InstLoadStore.hpp"

using namespace llvm;
using namespace NTS;

// TODO: Support types
// TODO: Memory leaks: store all formulas somewhere

// TODO: try argument promotion pass (-argpromotion)

llvm2nts::llvm2nts(const llvm::Type *t)
{
	t->print(errs());
	m_nts.addState(); // first (initial) state
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

void llvm2nts::processInstruction(const Instruction &i)
{
	i.print(errs(), NULL);
	errs() << "\n";

	// TODO: Split this
	switch(i.getOpcode())
	{
		case Instruction::Alloca:
			break;

		case Instruction::Store:
			// fall through
		case Instruction::Load:
			// fall through
		case Instruction::Ret:
			{
				const State & st_from = m_nts.lastState();
				const State & st_to = i.getOpcode() == Instruction::Ret?
					m_nts.addFinalState() : m_nts.addState();

				m_nts.addTransition(&st_from, &st_to,
						ils.process(i, m_vm, m_nts));
			}

			break;

		case Instruction::Add:
			{
				const State & st_from = m_nts.lastState();
				const State & st_to = m_nts.addState();

				m_nts.addTransition(&st_from, &st_to, ia.process(i, m_vm, m_nts));
			}
			break;

	}

}

