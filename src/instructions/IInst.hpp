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
		virtual NTS::ConcreteFormula process(
				const llvm::Instruction &i,
				VariableManager &vm,
				NTS::BasicNts &n) = 0;
};







#endif // _IINST_HPP_
