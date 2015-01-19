
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


#if 0
static const auto *f_mov = new FormulaBop
(
 BoolOp::And,
 new AtomicRelation
 (
  AtomicRelation::Relation::Eq,
  new AbstArithValue(0, true), // destination
  new AbstArithValue(1, false) // source
 ),
 new Havoc{0});

static const auto *f_add_nocheck = new FormulaBop
(
 BoolOp::And,
 new AtomicRelation
 (
  AtomicRelation::Relation::Eq,
  new AbstArithValue(0, true), // destination
  new AbstArithTermRelation
  (
   AbstArithTermRelation::Connector::Add,
   new AbstArithValue(1, false), // left source
   new AbstArithValue(2, false)  // right source
  )
 ),
 new Havoc{1,2});
#endif

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
#if 0
		case Instruction::Store:
			{
				const llvm::StoreInst &s = cast<llvm::StoreInst>(i);
				const llvm::Value *ptr = s.getPointerOperand();
				const llvm::Value *val = s.getValueOperand();
				auto *ptri = cast<llvm::Instruction>(ptr);

				if (ptri->getOpcode() != Instruction::Alloca)
					throw std::logic_error("Store instruction: only storing to local (alloca) variable is implemented");

				auto * dest = m_vm.getIPrint(ptr);
				auto * src = m_vm.getIPrint(val);

				const State & st_from = m_nts.lastState();
				const State & st_to = m_nts.addState();
				m_nts.addTransition(&st_from, &st_to,
						ConcreteFormula(*f_mov, {dest, src}));
			}
			break;

		case Instruction::Load:
			{
				const auto &l = cast<llvm::LoadInst>(i);
				const llvm::Value *ptr = l.getPointerOperand();
				auto *ptri = cast<llvm::Instruction>(ptr);

				if (ptri->getOpcode() != Instruction::Alloca)
					throw std::logic_error("Load instruction: only storing to local (alloca) variable is implemented");

				auto *src = m_vm.getIPrint(ptr);
				const State & st_from = m_nts.lastState();
				const State & st_to = m_nts.addState();
				m_nts.addTransition(&st_from, &st_to,
						ConcreteFormula(*f_mov, {ret_value, src}));

			}
			break;

		case Instruction::Ret:
			{
				const auto &ri = cast<llvm::ReturnInst>(i);
				const llvm::Value * rv = ri.getReturnValue();

				if (rv->getType() != m_return_type)
					throw std::logic_error("Return type missmatch");

				auto * src = m_vm.getIPrint(rv);
				const State & st_from = m_nts.lastState();
				const State & st_to = m_nts.addFinalState();
				m_nts.addTransition(&st_from, &st_to,
						ConcreteFormula(*f_mov, {m_return_var, src}));

			}
			break;
#endif
		case Instruction::Add:
			{
				const State & st_from = m_nts.lastState();
				const State & st_to = m_nts.addState();

				m_nts.addTransition(&st_from, &st_to, ia.process(i, m_vm, m_nts));
			}
				break;

	}

}

