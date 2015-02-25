
#include <llvm/Pass.h>
//#include <llvm/IR/ValueMap.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Constants.h>
#include <llvm/Support/raw_ostream.h>


#include "llvm2nts.hpp"

// C++ STL headers
#include <stdexcept>
#include <ostream>
#include <sstream>

#include "nts/NTS.hpp"
#include "nts/VariableIdentifier.hpp"
#include "nts/AtomicRelation.hpp"
#include "nts/Havoc.hpp"



#include "llvmFunction2nts.hpp"

using namespace llvm;
using namespace NTS;


void llvm2nts::process_module ( const Module &m )
{
//	llvm::errs() << m.getName().str() << "\n";
	m_name = m.getName().str().c_str();
	for ( const Function & f : m)
	{
		m_bnts.emplace_back ( f.getName().str() );
		llvmFunction2nts conv ( f, m_bnts.back() );
		conv.process();
	}
}

void llvm2nts::print(std::ostream &o) const
{
	o << "NTS " << m_name << ";\n";
	for (const BasicNts & n : m_bnts)
	{
		n.print ( o );
	}
}



