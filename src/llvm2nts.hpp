#ifndef _LLVM2NTS_HPP_
#define _LLVM2NTS_HPP_

// C++ STL headers
#include <ostream>

#include "llvm/Pass.h"
// Note: In version 3.5 ValueMap is inside IR directory
#include "llvm/ADT/ValueMap.h"

#include "nts/NTS.hpp"
#include "nts/AbstractArithmetic.hpp"
#include "VariableManager.hpp"

class llvm2nts
{
	private:
		const llvm::Type * m_return_type;
		const NTS::Variable * m_return_var;

		VariableManager m_vm;

		NTS::BasicNts m_nts;


	public:
		llvm2nts(const llvm::Type *t);
		~llvm2nts();

		void setReturnType(const llvm::Type *t);
		const NTS::Variable * addParam(const llvm::Argument *val);
		void processInstruction(const llvm::Instruction &i);
		void print(std::ostream &o) const;
};


#endif // _LLVM2NTS_HPP_