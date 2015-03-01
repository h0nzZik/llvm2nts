/**
 * @file InstIcmp.hpp
 * @date 28. 2. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 *
 * Implements 'icmp' instruction.
 */

#ifndef SRC_INSTRUCTIONS_INSTICMP_HPP_
#define SRC_INSTRUCTIONS_INSTICMP_HPP_

// LLVM headers
#include <llvm/IR/Instructions.h>
#include <llvm2nts/instructions/Constants.hpp>

// NTS headers
#include <nts/Havoc.hpp>
#include <nts/AtomicRelation.hpp>

// Project headers
#include "IInst.hpp"

class InstIcmp : public IInst
{
	/*
	 * Signed comparision is based on following observation:
	 * let a, b be signed integers (of the same range),
	 * represented by unsigned value a_u and a_s, respectively.
	 * Then a < a if and only if:
	 * ( ( neg(a) <==> neg(b) ) && ( a_u < b_u ) ) || ( neg(a) && !neg(b) )
	 *
	 * This class just builds a set of similar formulas.
	 */

	private:
		// Boundary values for different bitvector types
		Constants         & m_constants;
		// Maps to result
		NTS::AbstArithValue m_av_result;
		// Maps to first operant
		NTS::AbstArithValue m_av_op1;
		// Maps to second operant
		NTS::AbstArithValue m_av_op2;
		// Maps to signed bound
		NTS::AbstArithValue m_av_sigbound;
		// Maps to true
		NTS::AbstArithValue m_av_true;
		// Maps to false
		NTS::AbstArithValue m_av_false;

		// Havoc ( m_av_result )
		NTS::Havoc          m_havoc;

		// Positive / negative predicates for signed values
		NTS::AtomicRelation m_op1_negative;
		NTS::AtomicRelation m_op1_nonnegative;
		NTS::AtomicRelation m_op2_negative;
		NTS::AtomicRelation m_op2_nonnegative;

		// Both operants are equivalently negative
		// i.e. op1 is negative <==> op2 is negative
		NTS::FormulaBop     m_neg_equiv;
		// Op1 is negative but op1 is not
		NTS::FormulaBop     m_op1_neg_op2_nonneg;
		// Op2 is negative but op1 is not
		NTS::FormulaBop     m_op2_neg_op1_nonneg;

		// Compare equality
		NTS::AtomicRelation m_op1_eq_op2;
		NTS::AtomicRelation m_op1_neq_op2;

		// Compare unsigned operands
		NTS::AtomicRelation m_op1_ult_op2;
		NTS::AtomicRelation m_op1_ule_op2;
		NTS::AtomicRelation m_op1_uge_op2;
		NTS::AtomicRelation m_op1_ugt_op2;

		// Operands equivalently negative and op1 < op2
		NTS::FormulaBop     m_neg_equiv_and_op1_lt_op2;
		// Operands equivalently negative and op1 > op2
		NTS::FormulaBop     m_neg_equiv_and_op1_gt_op2;

		// Compare signed operands
		NTS::FormulaBop     m_op1_slt_op2;
		NTS::FormulaBop     m_op1_sgt_op2;
		NTS::FormulaBop     m_op1_sle_op2;
		NTS::FormulaBop     m_op1_sge_op2;

		// Assign operands to result
		NTS::AtomicRelation m_assign_true;
		NTS::AtomicRelation m_assign_false;

		// Conditionally assign operands to result
		NTS::FormulaBop     m_eq_assign_true;
		NTS::FormulaBop     m_neq_assign_false;

		NTS::FormulaBop     m_ult_assign_true;
		NTS::FormulaBop     m_ule_assign_true;

		NTS::FormulaBop     m_ugt_assign_false;
		NTS::FormulaBop     m_uge_assign_false;

		NTS::FormulaBop     m_slt_assign_true;
		NTS::FormulaBop     m_sle_assign_true;

		NTS::FormulaBop     m_sgt_assign_false;
		NTS::FormulaBop     m_sge_assign_false;


		// Assigns left operand when true, right operand when false
		NTS::FormulaBop     m_f_eq;
		NTS::FormulaBop     m_f_ult;
		NTS::FormulaBop     m_f_ule;
		NTS::FormulaBop     m_f_slt;
		NTS::FormulaBop     m_f_sle;

		// Add havoc to formulas above
		NTS::FormulaBop     m_f_eq_havoc;
		NTS::FormulaBop     m_f_ult_havoc;
		NTS::FormulaBop     m_f_ule_havoc;
		NTS::FormulaBop     m_f_slt_havoc;
		NTS::FormulaBop     m_f_sle_havoc;


		NTS::ConcreteFormula get_formula (
				llvm::ICmpInst::Predicate   p,
				const NTS::IPrint         * result,
				const NTS::IPrint         * op_1,
				const NTS::IPrint         * op_2,
				const NTS::IPrint         *  signed_bound);

	public:
		InstIcmp ( Constants & constants );
		virtual ~InstIcmp();

		virtual bool supports(unsigned int opcode) const;

		virtual const NTS::State * process(
				const NTS::State        * from    ,
				const llvm::Instruction & i       ,
				FunctionMapping         & map     ,
				NTS::BasicNts           & n       ,
				int                       bb_id   ,
				int                       inst_id );
};

#endif /* SRC_INSTRUCTIONS_INSTICMP_HPP_ */
