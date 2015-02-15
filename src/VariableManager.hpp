#ifndef _VARIABLEMANAGER_HPP_
#define _VARIABLEMANAGER_HPP_

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/ADT/DenseMap.h"
#include "nts/NTS.hpp"
#include "nts/AbstractArithmetic.hpp"
#include "nts/IPrint.hpp"

// Manages variables and constants
class VariableManager final
{
	private:
		llvm::ValueMap<const llvm::Value *, NTS::IPrint *> m_variables;
		llvm::DenseMap<const llvm::BasicBlock *, const NTS::State *> m_block_start;


	public:
		VariableManager() {;}
		~VariableManager();

		void insVariable(const llvm::Value *llva, NTS::IPrint *var);
		NTS::IPrint * getIPrint(const llvm::Value * llval);

		void ins_bb_start ( const llvm::BasicBlock *block,
						    const NTS::State *s );

		const NTS::State * get_bb_start ( const llvm::BasicBlock * block );
};










#endif // _VARIABLEMANAGER_HPP_
