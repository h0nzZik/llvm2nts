#ifndef LLVM2NTS_MODULE_MAPPING_H_
#define LLVM2NTS_MODULE_MAPPING_H_

#include <vector>
#include <memory>

#include <llvm/IR/Function.h>
#include <llvm/ADT/DenseMap.h>

#include <libNTS/nts.hpp>

struct BasicNtsInfo
{
	nts::BasicNts & bn;
	// following may be null
	const llvm::Function * orig_function;
	nts::Variable  * ret_var;
	nts::Variable  * lbb_var;

	llvm::DenseMap <
		const llvm::Argument *,
		nts::Variable *         > args;

	BasicNtsInfo ( nts::BasicNts & bn ) :
		bn            ( bn      ),
		orig_function ( nullptr ),
		ret_var       ( nullptr ),
		lbb_var       ( nullptr )
	{
		;
	}
};

struct VariableInfo
{
	nts::Variable & var;
	// may be null
	const llvm::Value * val;

	VariableInfo ( nts::Variable & var ) :
		var ( var     ),
		val ( nullptr )
	{
		;
	}
};

class ModuleMapping
{
	using Functions = llvm::DenseMap < const llvm::Function *, BasicNtsInfo * >;

	public:
		~ModuleMapping();

		void ins_function (
				const llvm::Function & fun,
				std::unique_ptr<BasicNtsInfo> bni
		);
		BasicNtsInfo & get_nts ( const llvm::Function & fun ) const;

		const Functions & ntses() const { return m_functions; }

		void ins_variable (
				const llvm::GlobalValue & llval,
				std::unique_ptr<VariableInfo> var
		);
		VariableInfo & get_variable ( const llvm::GlobalValue & llval ) const;

		void ins_pthread_function ( const llvm::Function & f );
		unsigned int get_pthread_function_id ( const llvm::Function & f ) const;

	private:
		llvm::DenseMap < const llvm::Function *, BasicNtsInfo * >
			m_functions;

		llvm::DenseMap < const llvm::GlobalValue *, VariableInfo * >
			m_vars;

		// Maps Pthread function to identifier
		llvm::DenseMap < const llvm::Function *, unsigned int >
			m_ptf_id;

		// Pthread functions by identifier
		std::vector < const llvm::Function * > m_ptf;
};

#endif // LLVM2NTS_MODULE_MAPPING_H_


