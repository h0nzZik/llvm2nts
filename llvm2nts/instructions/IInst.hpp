#ifndef _IINST_HPP_
#define _IINST_HPP_

#include <llvm/IR/Instruction.h>
#include <libNTS/nts.hpp>
#include "../FunctionMapping.hpp"

class IInst
{
	public:
		virtual ~IInst() {};

		virtual void process (
				const BasicNtsInfo      & bntsi,
				StateInfo               & sti,
				FunctionMapping         & map,
				const llvm::Instruction & i    ) = 0;
};







#endif // _IINST_HPP_
