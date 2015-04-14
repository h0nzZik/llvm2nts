#ifndef INSTRUCTIONS_INST_ALLOCA_HPP_
#define INSTRUCTIONS_INST_ALLOCA_HPP_

// LLVM headers
#include <llvm/IR/Instructions.h>

// Project headers
#include "IInst.hpp"

class InstAlloca : public IInst
{
	public:
		InstAlloca() = default;
		virtual ~InstAlloca() = default;

		virtual void process (
				const BasicNtsInfo      & bntsi,
				StateInfo               & sti,
				FunctionMapping         & map,
				const llvm::Instruction & i    ) override;
};


#endif // INSTRUCTIONS_INST_ALLOCA_HPP_

