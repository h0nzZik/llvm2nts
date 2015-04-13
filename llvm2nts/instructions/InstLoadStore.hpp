#ifndef _INSTRUCTION_INST_LOAD_STORE_HPP_
#define _INSTRUCTION_INST_LOAD_STORE_HPP_

// LLVM headers
#include <llvm/IR/Instructions.h>

// Project headers
#include "IInst.hpp"

class InstLoadStore : public IInst
{
	public:
		InstLoadStore() = default;
		virtual ~InstLoadStore() = default;

		virtual bool supports(unsigned int opcode) const override;

		virtual void process (
				const BasicNtsInfo      & bntsi,
				StateInfo               & sti,
				FunctionMapping         & map,
				const llvm::Instruction & i    ) override;
};


#endif // _INSTRUCTION_INST_LOAD_STORE_HPP_

