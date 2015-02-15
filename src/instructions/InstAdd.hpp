#ifndef _INSTADD_HPP_
#define _INSTADD_HPP_

// Std c++ headers
#include <vector>

// LLVM headers
#include <llvm/IR/Instructions.h>

// Project headers
#include "IInst.hpp"
#include "../nts/Formula.hpp"
#include "../nts/ConcreteArithmetic.hpp"
#include "../nts/AbstractArithmetic.hpp"
#include "../nts/AtomicRelation.hpp"
#include "../nts/Havoc.hpp"

class InstAdd : public IInst
{
	private:

		// Abstract destination
		NTS::AbstArithValue m_adst;
		// Abstract left source
		NTS::AbstArithValue m_als;
		// Abstract right source
		NTS::AbstArithValue m_ars;

		// Abstract unsigned bound
		NTS::AbstArithValue m_aub;
		// Abstract signed bound
		NTS::AbstArithValue m_asb;

		// Left source + right source
		NTS::AbstArithTermRelation m_plus;
		// Left source + right source modulo unsigned bound
		NTS::AbstArithTermRelation m_plus_mod;
		// Unsigned bound + signed bound
		NTS::AbstArithTermRelation m_ub_plus_sb;

		// Left source is positive
		NTS::AtomicRelation m_ls_pos;
		// Right source is positive
		NTS::AtomicRelation m_rs_pos;
		// Both sources are positive
		NTS::FormulaBop m_srcs_pos;

		// Left source is negative
		NTS::AtomicRelation m_ls_neg;
		// Right source is negative
		NTS::AtomicRelation m_rs_neg;
		// Both sources are negative
		NTS::FormulaBop m_srcs_neg;

		// Sum of sources is greater than or equal to signed bound
		NTS::AtomicRelation m_plus_ge_sb;
		// Likewise, but unsigned bound
		NTS::AtomicRelation m_plus_ge_ub;
		// Sum is less than unsigned bound + signed bound
		NTS::AtomicRelation m_plus_lt_sb_ub;


		// For result to be valid, there are some preconditions:
		// 1) if NSW flag is set, there must be no signed overflow
		// 2) if NUW flag is set, there must be no unsigned overflow
		//
		// Signed overflow happens on two cases:
		// a) summands are positive and their sum is greater than
		//    largest positive value
		// b) summands are negative (so greater than largest positive value
		//    but still smaller than largest unsigned value)
		//    and their sum is positive (of course greater than largest
		//    unsigned value, but smaller than or equal to LPV + LUV)


		NTS::FormulaBop m_f_signed_of_positive;
		NTS::FormulaBop m_f_signed_of_negative;
		NTS::FormulaBop m_f_signed_of;
		NTS::FormulaNot m_f_no_signed_of;
		// Sum is less than unsigned bound
		NTS::AtomicRelation m_f_no_unsigned_of;

		// No overflow happens
		NTS::FormulaBop m_f_no_of;

		// result assignment (like c' = a + b mod 256)
		NTS::AtomicRelation m_f_assign;

		NTS::Havoc m_havoc;

		// No signed wrap, no unsigned wrap
		NTS::Formula &m_f;
		// Signed wrap
		NTS::FormulaBop m_fs;
		// Unsigned wrap
		NTS::FormulaBop m_fu;
		// Both signed and unsigned wrap
		NTS::FormulaBop m_fsu;

		NTS::FormulaBop m_f_havoc;
		NTS::FormulaBop m_fs_havoc;
		NTS::FormulaBop m_fu_havoc;
		NTS::FormulaBop m_fsu_havoc;

		struct BitsizeGroup
		{
			NTS::ConPositiveNumeral * unsigned_bound;
			NTS::ConPositiveNumeral * signed_bound;
		};


		std::vector<BitsizeGroup> m_bs;

		const BitsizeGroup & getBitsizeGroup(unsigned int bitsz);
		const NTS::Formula & getFormula(bool signed_wrap, bool unsigned_wrap) const;


	public:
		InstAdd();
		virtual ~InstAdd();

		virtual bool supports(unsigned int opcode) const;

		virtual const NTS::State * process(
				const NTS::State        * from    ,
				const llvm::Instruction & i       ,
				VariableManager         & vm      ,
				NTS::BasicNts           & n       ,
				int                       bb_id   ,
				int                       inst_id );
};

#endif // _INSTADD_HPP_
