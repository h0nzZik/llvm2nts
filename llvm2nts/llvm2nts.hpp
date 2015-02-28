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

#include <nts/NTS.hpp>
#include <nts/AbstractArithmetic.hpp>

#include "ModuleMapping.hpp"

class llvm2nts
{
	private:
		std::string m_name;
		std::vector<NTS::BasicNts> m_bnts;
		ModuleMapping m_modmap;

	public:
		llvm2nts() { ; }
		~llvm2nts() { ; }

		void process_module ( const llvm::Module & m);
		void print ( std::ostream &o ) const;
};


#endif // _LLVM2NTS_HPP_
