#ifndef SRC_FUNCTION_MAPPING_HPP_
#define SRC_FUNCTION_MAPPING_HPP_

#include <llvm/IR/Value.h>
#include <llvm/IR/ValueMap.h>
#include <llvm/ADT/DenseMap.h>

#include <nts/NTS.hpp>

#include "ModuleMapping.hpp"

class FunctionMapping
{
	public:
		const ModuleMapping & m_modmap;

		FunctionMapping ( NTS::BasicNts & nts, const ModuleMapping &mod );

		const NTS::Variable * ins_variable ( const llvm::Value *llva );
		void ins_iprint ( const llvm::Value *llva, const NTS::IPrint *var );
		
		const NTS::IPrint * get_iprint ( int n );
		const NTS::IPrint * get_iprint ( const llvm::Value *value );

		void ins_bb_start ( const llvm::BasicBlock *block,
						    const NTS::State *s );

		const NTS::State * get_bb_start ( const llvm::BasicBlock * block );

	private:
		NTS::BasicNts & m_nts;
		llvm::ValueMap <const llvm::Value *, const NTS::IPrint *> m_values;
		llvm::DenseMap <const llvm::BasicBlock *, const NTS::State *> m_block_start;

};


#endif // SRC_FUNCTION_MAPPING_HPP_
