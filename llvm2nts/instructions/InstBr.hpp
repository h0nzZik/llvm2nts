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

// NTS headers
#include <nts/Havoc.hpp>
#include <nts/AtomicRelation.hpp>

// Project headers
#include "IInst.hpp"

/*
 *
 */
class InstBr : public IInst
{
	private:
		// Maps to variable, which stores
		// id of last visited basic block.
		NTS::AbstArithValue m_aval_lbb;
		// Maps to id of current BasicBlock
		NTS::AbstArithValue m_aval_bbid;
		// This instruction assigns to that variable
		NTS::AtomicRelation m_assign_lbb;
		// And does not assign to any other variable.
		NTS::Havoc          m_havoc;
		// Conjunction of assignment and havoc
		NTS::FormulaBop     m_formula;

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
