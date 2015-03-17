#ifndef LLVM2NTS_MODULE_MAPPING_H_
#define LLVM2NTS_MODULE_MAPPING_H_

#include <vector>

#include <llvm/IR/Function.h>
#include <llvm/ADT/DenseMap.h>

#include <nts/NtsRef.hpp>
#include <nts/NTS.hpp>

class ModuleMapping
{
	public:
		void ins_function ( const llvm::Function *fun, const NTS::NtsRef *nts );
		const NTS::NtsRef * get_nts ( const llvm::Function *fun ) const;

		void ins_iprint ( const llvm::GlobalValue *llval, const NTS::IPrint * ip );
		const NTS::IPrint * get_iprint ( const llvm::GlobalValue *llval ) const;

		void ins_pthread_function ( const llvm::Function *f );
		unsigned int get_pthread_function_id ( const llvm::Function * f ) const;

	private:
		llvm::DenseMap < const llvm::Function *, const NTS::NtsRef * >
			m_functions;

		llvm::DenseMap < const llvm::GlobalValue *, const NTS::IPrint * >
			m_values;

		// Maps Pthread function to identifier
		llvm::DenseMap < const llvm::Function *, unsigned int >
			m_ptf_id;

		// Pthread functions by identifier
		std::vector < const llvm::Function * > m_ptf;
};

#endif // LLVM2NTS_MODULE_MAPPING_H_


