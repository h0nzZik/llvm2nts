
#include "llvm/Pass.h"
// Note: In version 3.5 ValueMap is inside IR directory
#include "llvm/ADT/ValueMap.h"
#include "llvm/IR/Function.h"
// Note: In version 3.5 InstIterator is inside IR directory
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"

#include "llvm2nts.hpp"

// C++ STL headers
#include <stdexcept>
#include <ostream>
#include <sstream>

#include "nts/NTS.hpp"
#include "nts/Arithmetic.hpp"
#include "nts/VariableIdentifier.hpp"
#include "nts/AtomicRelation.hpp"
#include "nts/Havoc.hpp"

using namespace llvm;
using namespace NTS;

// TODO: try argument promotion pass (-argpromotion)

llvm2nts::llvm2nts(const llvm::Type *t)
{
	t->print(errs());
	m_nts.addState(); // first (initial) state
	m_return_type = t;
	// TODO: Add variable of that type

	NTS::Variable *v = new NTS::Variable("ret_var");
	m_nts.setRetVar(v);
	m_return_var = v;
}



void llvm2nts::print(std::ostream &o) const
{
	m_nts.print(o);
}

#if 0
const std::string & llvm2nts::getValueName(const Value *value)
{
	const NTS::Variable & v = getVariable(value);
	return v.m_name;
}
#endif

const NTS::Variable & llvm2nts::getVariable(const Value *value)
{
	NTS::Variable * v = m_variables.lookup(value);
	if (!v)
		v = m_params.lookup(value);

	if (!v)
		throw std::logic_error("Can not find used variable");
	return *v;
}

NTS::ArithmeticLiteral * llvm2nts::createArithmeticLiteral(const llvm::Value * value)
{
	if (isa<Constant>(value))
	{
		// TODO: Somehow remove type from the output
		// now there is: 'i32 1'
		const auto *c = cast<llvm::ConstantInt>(value);
		auto * pn = new NTS::PositiveNumeral();
		llvm::raw_string_ostream os(pn->getStr());
		c->print(os);
		return pn;
	}

	return new ArithmeticVariableIdentifier(&getVariable(value), false);
}

const NTS::Variable * llvm2nts::addValue(const Value *val)
{
	errs() << "Addvalue\n";
	/* Construct the name of NTL variable.
	 * Because in NTL there is no variable shadowing,
	 * global and local variables have to have different names.
	 */
	std::string s;
	if (isa<llvm::GlobalValue>(val))
	{
		s = std::string("g_") + val->getName().str();
	} else {
		s = std::string("l_") + val->getName().str();
	}

	Variable *v = new Variable(s);
	m_nts.addVariable(v);

	auto p = std::make_pair(val, v);
	m_variables.insert(p);

	return v;
}

const NTS::Variable * llvm2nts::addParam(const Argument * arg)
{
	Variable *v = new Variable(std::string("arg_") + arg->getName().str());
	m_nts.addArgument(v);
	auto p = std::make_pair(cast<Value>(arg), v);
	m_variables.insert(p);
	return v;
}

void llvm2nts::processInstruction(const Instruction &i)
{
	i.print(errs(), NULL);
	//	NTS::State &s = this->m_nts.addState();
	llvm::Type *t = i.getType();

	const NTS::Variable *ret_value = NULL;
	// Store the result
	if (t && !t->isVoidTy())
	{
		ret_value = addValue(cast<const Value>(&i));
	}
	errs() << "\n";


	// FIXME: What if one of arguments is a constant?

	// TODO: Ensure there is no problem with casting and saving values to our map
	//
	switch(i.getOpcode())
	{
		case Instruction::Alloca:
			break;
		case Instruction::Store:
			{
				const llvm::StoreInst &s = cast<llvm::StoreInst>(i);
				const llvm::Value *ptr = s.getPointerOperand();
				const llvm::Value *val = s.getValueOperand();
				auto *ptri = cast<llvm::Instruction>(ptr);

				if (ptri->getOpcode() != Instruction::Alloca)
					throw std::logic_error("Store instruction: only storing to local (alloca) variable is implemented");

				auto & dest = getVariable(ptr);
				auto & src = getVariable(val);

				Formula *f = new FormulaBop(BoolOp::And,
						new AtomicRelation(AtomicRelation::Relation::Eq,
							new ArithmeticVariableIdentifier(&dest, true),
							new ArithmeticVariableIdentifier(&src, false)
							),
						new Havoc({&dest})
						);
				const State & st_from = m_nts.lastState();
				const State & st_to = m_nts.addState();
				m_nts.addTransition(&st_from, &st_to, f);
			}
			break;

		case Instruction::Load:
			{
				const auto &l = cast<llvm::LoadInst>(i);
				const llvm::Value *ptr = l.getPointerOperand();
				auto *ptri = cast<llvm::Instruction>(ptr);

				if (ptri->getOpcode() != Instruction::Alloca)
					throw std::logic_error("Load instruction: only storing to local (alloca) variable is implemented");

				auto & src = getVariable(ptr);

				ArithmeticTerm *t1 = new ArithmeticVariableIdentifier(ret_value, true);
				ArithmeticTerm *t2 = new ArithmeticVariableIdentifier(&src, false);
				AtomicProposition *ap1 = new AtomicRelation(AtomicRelation::Relation::Eq, t1, t2);
				AtomicProposition *ap2 = new Havoc({ret_value});
				Formula *f = new FormulaBop(BoolOp::And, ap1, ap2);

				const State & st_from = m_nts.lastState();
				const State & st_to = m_nts.addState();
				m_nts.addTransition(&st_from, &st_to, f);

			}
			break;

		case Instruction::Add:
			{
				errs() << "Add\n";
				auto &bo = cast<llvm::BinaryOperator>(i);
				const llvm::Value *l = bo.getOperand(0);
				const llvm::Value *r = bo.getOperand(1);
				// TODO: Overflow
				// 'no unsigned unwrap' and 'no signed unwrap' are not implemented
				// see http://llvm.org/releases/3.5.0/docs/LangRef.html#add-instruction
				// see http://llvm.org/releases/3.5.0/docs/LangRef.html#poisonvalues

				Formula *f = new FormulaBop(BoolOp::And,
						new AtomicRelation(AtomicRelation::Relation::Eq,
							new ArithmeticVariableIdentifier(ret_value, true),
							new ArithmeticTermRelation(ArithmeticTermRelation::Connector::Add,
								createArithmeticLiteral(l),
								createArithmeticLiteral(r)
								)
							),
						new Havoc({ret_value})
						);

				const State & st_from = m_nts.lastState();
				const State & st_to = m_nts.addState();
				m_nts.addTransition(&st_from, &st_to, f);
			}
			break;
#if 1
		case Instruction::Ret:
			{
				const auto &ri = cast<llvm::ReturnInst>(i);
				const llvm::Value * rv = ri.getReturnValue();

				if (rv->getType() != m_return_type)
					throw std::logic_error("Return type missmatch");

				const NTS::Variable * var = &getVariable(rv);
				// like store, but may be void.
				Formula *f = new FormulaBop(BoolOp::And,
						new AtomicRelation(AtomicRelation::Relation::Eq,
							new ArithmeticVariableIdentifier(m_return_var, true),
							new ArithmeticVariableIdentifier(var, false)
							),
						new Havoc({m_return_var})
						);

				const State & st_from = m_nts.lastState();
				const State & st_to = m_nts.addFinalState();
				m_nts.addTransition(&st_from, &st_to, f);

			}
			break;
#endif
	}

}

