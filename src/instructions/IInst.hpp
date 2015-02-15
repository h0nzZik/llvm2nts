#ifndef _IINST_HPP_
#define _IINST_HPP_

#include <llvm/IR/Instruction.h>
#include "../nts/Formula.hpp"
#include "../nts/NTS.hpp"
#include "../VariableManager.hpp"

class IInst
{
	public:
		virtual ~IInst() {};

		virtual bool supports(unsigned int opcode) const = 0;

		virtual const NTS::State * process(
				const NTS::State        * from    ,
				const llvm::Instruction & i       ,
				VariableManager         & vm      ,
				NTS::BasicNts           & n       ,
				int                       bb_id   ,
				int                       inst_id ) = 0;
};







#endif // _IINST_HPP_
