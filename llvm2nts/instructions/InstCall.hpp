/**
 * @file   InstCall.hpp
 * @date   8. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 *
 * Implements 'call' instruction.
 */

#ifndef SRC_INSTRUCTIONS_INSTCALL_HPP_
#define SRC_INSTRUCTIONS_INSTCALL_HPP_

// LLVM headers
#include <llvm/IR/Instructions.h>

// Project headers
#include "../FunctionMapping.hpp"
#include "IInst.hpp"

class InstCall : public IInst
{
	public:
		InstCall ( ) = default;
		virtual ~InstCall() = default;


		virtual void process (
				const BasicNtsInfo      & bntsi,
				StateInfo               & sti,
				FunctionMapping         & map,
				const llvm::Instruction & i    ) override;
};

#endif /* SRC_INSTRUCTIONS_INSTCALL_HPP_ */
