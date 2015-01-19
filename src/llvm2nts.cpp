
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

static InstAdd ia;

void llvm2nts::processInstruction(const Instruction &i)
{
	i.print(errs(), NULL);
	//	NTS::State &s = this->m_nts.addState();
	llvm::Type *t = i.getType();

	const NTS::IPrint *ret_value = NULL;
	// Store the result
	if (t && !t->isVoidTy())
	{
		ret_value = m_vm.getIPrint(cast<const Value>(&i));
	}
	errs() << "\n";

	// TODO: Split this
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

		case Instruction::Add:
			{
				const State & st_from = m_nts.lastState();
				const State & st_to = m_nts.addState();

				m_nts.addTransition(&st_from, &st_to, ia.process(i, m_vm));
			}
				break;

#if 0
				auto &bo = cast<llvm::BinaryOperator>(i);
				const llvm::Value *l = bo.getOperand(0);
				const llvm::Value *r = bo.getOperand(1);

				auto * l_read = m_vm.getIPrint(l);
				auto * r_read =m_vm.getIPrint(r);


				// Overflow handling
				// LLVM uses 'no unsigned unwrap' and 'no signed unwrap' flags.
				// For better understanding visit documentation:
				// http://llvm.org/releases/3.5.0/docs/LangRef.html#add-instruction
				// http://llvm.org/releases/3.5.0/docs/LangRef.html#poisonvalues

				unsigned int bw = bo.getType()->getIntegerBitWidth();
				// TODO: add support for greater scales (eg using APInt)
				if (bw >= 8 * sizeof(unsigned long long int))
					throw std::domain_error("Bit width too large");

				unsigned long long int ub = 1ULL << bw;
				unsigned long long int sb = 1ULL << (bw - 1);

				auto * unsigned_bound = new PositiveNumeral();
				{
					llvm::raw_string_ostream os(unsigned_bound->getStr());
					os << ub;
					os.flush();
				}

				// a + b
				auto * a_plus_b = new ArithmeticTermRelation(
						ArithmeticTermRelation::Connector::Add,
						l_read, r_read
						);

				// (a + b) % 256
				auto * a_plus_b_mod = new ArithmeticTermRelation(
						ArithmeticTermRelation::Connector::Mod,
						a_plus_b,
						unsigned_bound
						);

				// x' = (a + b) % 256
				auto * f_assign = new AtomicRelation(AtomicRelation::Relation::Eq,
						res_write,
						a_plus_b_mod
						);

				Formula * f_nuw_pre = NULL;
				if (bo.hasNoUnsignedWrap())
				{
					// Precondition for 'no unsigned unwrap'
					f_nuw_pre = new AtomicRelation(AtomicRelation::Relation::Lt,
							a_plus_b,
							unsigned_bound
							);
				}

				Formula * f_nsw_pre = NULL;
				if (bo.hasNoSignedWrap())
				{
					auto *signed_bound = new PositiveNumeral();
					{
						llvm::raw_string_ostream os(signed_bound->getStr());
						os << sb;
						os.flush();
					}

					// Precondition for 'no signed unwrap':
					// If both arguments are positive, result is positive, too.
					// If both arguments are negative, result is negative, too.

					// Atomic proposition for 'left operand is positive'
					auto * l_positive = new AtomicRelation(
							AtomicRelation::Relation::Lt,
							l_read,
							signed_bound
							);

					// 'lef operand is negative'
					auto *l_negative = new AtomicRelation(
							AtomicRelation::Relation::Ge,
							r_read,
							signed_bound
							);

					// 'right operand is positive'
					auto *r_positive = new AtomicRelation(
							AtomicRelation::Relation::Lt,
							r_read,
							signed_bound
							);

					// 'right operand is negative'
					auto *r_negative = new AtomicRelation(
							AtomicRelation::Relation::Ge,
							r_read,
							signed_bound
							);

					// 'result is positive'
					auto *res_positive = new AtomicRelation(
							AtomicRelation::Relation::Lt,
							a_plus_b,
							signed_bound
							);

					// 'result is negative'
					auto *res_negative = new AtomicRelation(
							AtomicRelation::Relation::Ge,
							a_plus_b_mod,
							signed_bound
							);

					auto *both_positive = new FormulaBop(
							BoolOp::And,
							l_positive,
							r_positive
							);

					auto *both_negative = new FormulaBop(
							BoolOp::And,
							l_negative,
							r_negative
							);

					auto *nsw_pre1 = new FormulaBop(
							BoolOp::Impl,
							both_positive,
							res_positive
							);

					auto * nsw_pre2 = new FormulaBop(
							BoolOp::Impl,
							both_negative,
							res_negative
							);

					f_nsw_pre = new FormulaBop(
							BoolOp::And,
							nsw_pre1,
							nsw_pre2
							);
				}

				Formula *pre = NULL;
				if (f_nsw_pre || f_nuw_pre)
				{
					if (f_nsw_pre && !f_nuw_pre)
					{
						pre = f_nsw_pre;
					} else if (!f_nsw_pre && f_nuw_pre) {
						pre = f_nuw_pre;
					} else {
						pre = new FormulaBop(BoolOp::And, f_nsw_pre, f_nuw_pre);
					}
				}

				Formula *f_add;
				if (pre)
				{
					f_add = new FormulaBop(BoolOp::Impl, pre, f_assign);
				} else {
					f_add = f_assign;
				}

				auto *f_havoc = new Havoc({ret_value});
				Formula *f = new FormulaBop(BoolOp::And, f_add, f_havoc);

				const State & st_from = m_nts.lastState();
				const State & st_to = m_nts.addState();
				m_nts.addTransition(&st_from, &st_to, f);
			}
			break;
#endif
#if 1
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
	}

}

