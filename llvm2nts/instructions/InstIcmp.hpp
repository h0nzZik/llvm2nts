/**
 * @file InstIcmp.hpp
 * @date 28. 2. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 *
 * Implements 'icmp' instruction.
 */

#ifndef SRC_INSTRUCTIONS_INSTICMP_HPP_
#define SRC_INSTRUCTIONS_INSTICMP_HPP_

#include <memory>

// LLVM headers
#include <llvm/IR/Instructions.h>

// NTS headers
#include <libNTS/logic.hpp>

// Project headers
#include "IInst.hpp"

class InstIcmp : public IInst
{
	/*
	 * Signed comparision is based on following observation:
	 * let a, b be signed integers (of the same range),
	 * represented by unsigned value a_u and a_s, respectively.
	 * Then a < b if and only if:
	 * ( ( neg(a) <==> neg(b) ) && ( a_u < b_u ) ) || ( neg(a) && !neg(b) )
	 *
	 * This class just builds a set of similar formulas.
	 */

	private:
		std::unique_ptr < nts::Formula > predicate (
				llvm::CmpInst::Predicate p,
				std::unique_ptr < nts::Term >,
				std::unique_ptr < nts::Term > ) const;

	public:
		InstIcmp ( ) = default;
		virtual ~InstIcmp() = default;

		virtual void process (
				const BasicNtsInfo      & bntsi,
				StateInfo               & sti,
				FunctionMapping         & map,
				const llvm::Instruction & i    ) override;
};

#endif /* SRC_INSTRUCTIONS_INSTICMP_HPP_ */
