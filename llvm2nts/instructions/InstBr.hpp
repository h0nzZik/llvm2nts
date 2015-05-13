/*
 * InstBr.hpp
 *
 *  Created on: 15. 2. 2015
 *      Author: jenda
 */

#ifndef SRC_INSTRUCTIONS_INSTBR_HPP_
#define SRC_INSTRUCTIONS_INSTBR_HPP_

// LLVM headers
#include <llvm/IR/Instructions.h>

// Project headers
#include "IInst.hpp"

class InstBr : public IInst
{
	private:
		static nts::Formula & formula_lbb_havoc (
				const BasicNtsInfo & bntsi,
				StateInfo & sti );

	public:
		InstBr ( ) = default;
		virtual ~InstBr() = default;

		virtual void process (
				const BasicNtsInfo      & bntsi,
				StateInfo               & sti,
				FunctionMapping         & map,
				const llvm::Instruction & i    ) override;
};


#endif /* SRC_INSTRUCTIONS_INSTBR_HPP_ */
