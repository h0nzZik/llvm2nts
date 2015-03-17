#ifndef _LLVM2NTS_HPP_
#define _LLVM2NTS_HPP_

// C++ STL headers
#include <ostream>
#include <vector>

// LLVM headers
#include <llvm/ADT/DenseMap.h>
#include <llvm/Pass.h>
#include <llvm/IR/ValueMap.h>
#include <llvm/IR/BasicBlock.h>

// NTS headers
#include <nts/NTS.hpp>
#include <nts/AbstractArithmetic.hpp>
#include <nts/Variable.hpp>
#include <nts/NtsModule.hpp>
#include <nts/NtsDeclaration.hpp>

#include "ModuleMapping.hpp"

// TODO: Split this into two parts: converter and NTS module
// NTS module will consist of name, NTSs and global variables
// Converter will provide mapping llvm -> nts symbols

class llvm2nts
{
	private:
		NTS::NtsModule     & m_nts_module;
		const llvm::Module & m_llvm_module;
		ModuleMapping        m_modmap;

		std::vector< NTS::NtsDeclaration * >
			                 m_extern_nts;

		bool                 m_use_pthreads;

		void process_pthreads ();

	public:
		llvm2nts ( NTS::NtsModule &nts_module, const llvm::Module & llvm_module );
		~llvm2nts();

		void process ( );
		void print ( std::ostream &o ) const;
};


#endif // _LLVM2NTS_HPP_
