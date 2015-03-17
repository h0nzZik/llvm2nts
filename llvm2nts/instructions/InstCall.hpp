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

// NTS headers
#include <nts/Havoc.hpp>
#include <nts/AtomicRelation.hpp>

// Project headers
#include "IInst.hpp"

class InstCall : public IInst
{
	public:
		InstCall ( );
		virtual ~InstCall();

		virtual bool supports(unsigned int opcode) const;

		virtual const NTS::State * process(
				const NTS::State        * from    ,
				const llvm::Instruction & i       ,
				FunctionMapping         & map     ,
				NTS::BasicNts           & n       ,
				int                       bb_id   ,
				int                       inst_id );
};

#endif /* SRC_INSTRUCTIONS_INSTCALL_HPP_ */
