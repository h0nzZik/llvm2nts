#ifndef _INSTADD_HPP_
#define _INSTADD_HPP_

// Std c++ headers
#include <vector>

// LLVM headers
#include <llvm/IR/Instructions.h>

// Project headers
#include "IInst.hpp"

class InstAdd : public IInst
{
	public:
		InstAdd () = default;
		virtual ~InstAdd() = default;;

		virtual void process (
				const BasicNtsInfo      & bntsi,
				StateInfo               & sti,
				FunctionMapping         & map,
				const llvm::Instruction & i    ) override;
};

#endif // _INSTADD_HPP_
