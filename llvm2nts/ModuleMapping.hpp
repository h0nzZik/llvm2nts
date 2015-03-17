#ifndef LLVM2NTS_MODULE_MAPPING_H_
#define LLVM2NTS_MODULE_MAPPING_H_

#include <llvm/IR/Function.h>
#include <llvm/ADT/DenseMap.h>

#include <nts/NTS.hpp>

class ModuleMapping
{
	public:
		void ins_function ( const llvm::Function *fun, const NTS::BasicNts *nts );
		const NTS::BasicNts * get_nts ( const llvm::Function *fun ) const;

		void ins_iprint ( const llvm::GlobalValue *llval, const NTS::IPrint * ip );
		const NTS::IPrint * get_iprint ( const llvm::GlobalValue *llval ) const;

	private:
		llvm::DenseMap < const llvm::Function *, const NTS::BasicNts * >
			m_functions;
		llvm::DenseMap < const llvm::GlobalValue *, const NTS::IPrint * >
			m_values;
};

#endif // LLVM2NTS_MODULE_MAPPING_H_


