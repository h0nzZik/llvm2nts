#ifndef LLVM2NTS_MODULE_MAPPING_H_
#define LLVM2NTS_MODULE_MAPPING_H_

#include <llvm/IR/Function.h>
#include <llvm/ADT/DenseMap.h>

#include <nts/NTS.hpp>

class ModuleMapping
{
	public:
		void ins_function ( const llvm::Function *fun, const NTS::BasicNts *nts );
		const NTS::BasicNts * get_nts ( const llvm::Function *fun );

	private:
		llvm::DenseMap < const llvm::Function *, const NTS::BasicNts * >
			m_functions;
};

#endif // LLVM2NTS_MODULE_MAPPING_H_


