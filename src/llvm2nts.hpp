#ifndef _LLVM2NTS_HPP_
#define _LLVM2NTS_HPP_

// C++ STL headers
#include <ostream>

#include "llvm/Pass.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IR/BasicBlock.h"

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

		void process_basic_block ( 
				const llvm::BasicBlock & b,
				int                      bb_id );

		const NTS::State * process_instruction (
				const NTS::State        * st_from ,
				const llvm::Instruction & i       ,
				int                       bb_id   ,
				int                       inst_id );

	public:
		llvm2nts(const llvm::Type *t);
		~llvm2nts();

		void setReturnType(const llvm::Type *t);
		const NTS::Variable * addParam(const llvm::Argument *val);
		void process_function    ( const llvm::Function &f);
		void print(std::ostream &o) const;
};


#endif // _LLVM2NTS_HPP_
