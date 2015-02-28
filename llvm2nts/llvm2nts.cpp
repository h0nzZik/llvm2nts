
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
	m_name = m.getName().str().c_str();

	// Reserve the space now, because appending may invalidate
	// all iterators and reference.
	m_bnts.reserve ( m.getFunctionList().size() );


	// Create one NTS for each function
	// and map each function to corresponding NTS
	for ( const Function & f : m)
	{
		m_bnts.emplace_back ( f.getName().str() );
		m_modmap.ins_function ( &f, &m_bnts.back() );
	}

	// From now do not insert elements.

	// Convert function to NTS
	// Note that the processing requires valid mapping 'Function -> BasicNts'
	unsigned int id = 0;
	for ( const Function &f : m )
	{
		llvmFunction2nts conv ( f, m_bnts[id], m_modmap );
		conv.process();
		id++;
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



