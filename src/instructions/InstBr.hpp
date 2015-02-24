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
#include "../nts/Havoc.hpp"

/*
 *
 */
class InstBr : public IInst
{
	private:
		NTS::Havoc m_havoc;

	public:
		InstBr();
		virtual ~InstBr();

		virtual bool supports(unsigned int opcode) const;

		virtual const NTS::State * process(
				const NTS::State        * from    ,
				const llvm::Instruction & i       ,
				FunctionMapping         & map     ,
				NTS::BasicNts           & n       ,
				int                       bb_id   ,
				int                       inst_id );
};

#endif /* SRC_INSTRUCTIONS_INSTBR_HPP_ */
