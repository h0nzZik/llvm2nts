
#include <llvm/Pass.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>


#include "llvm2nts.hpp"

// C++ STL headers
#include <stdexcept>
#include <ostream>
#include <sstream>

#include "nts/NtsModule.hpp"
//#include "nts/NTS.hpp"
//#include "nts/VariableIdentifier.hpp"
//#include "nts/AtomicRelation.hpp"
//#include "nts/Havoc.hpp"



#include "llvmFunction2nts.hpp"

using namespace llvm;
using namespace NTS;


llvm2nts::llvm2nts ( NTS::NtsModule &nts_module, const llvm::Module & llvm_module ) :
		m_nts_module ( nts_module ),
		m_llvm_module ( llvm_module )
{

}

llvm2nts::~llvm2nts ()
{
	for ( auto & r : m_extern_nts )
	{
		delete r;
		r = nullptr;
	}
}

void llvm2nts::process_pthreads()
{
	// Find function 'pthread_create()' (if it is declared)
	const Function * ptc = m_llvm_module.getFunction ( "pthread_create" );
	if ( !ptc )
		return;

	// Is it 'the' function?
	if ( ptc->getReturnType()->isVoidTy() )
		return;

	if ( ptc->getArgumentList().size() != 4 )
		return;

	if ( !ptc->use_empty() )
	{
		auto * decl = new NtsDeclaration ( "__thread_create" );
		m_extern_nts.push_back ( decl );
		m_modmap.ins_function ( ptc, decl );
		m_use_pthreads = true;
	}

	// Find all users of function 'pthread_create()'
	for ( const auto &u : ptc->users() )
	{
		if ( !isa<CallInst> ( u ) )
			continue;

		const CallInst * call = cast<CallInst> ( u );
		const Value * arg = call->getArgOperand ( 2 );
		if ( !isa<Function> ( arg ) )
			continue;

		const Function *callback = cast<Function> ( arg );
		this->m_modmap.ins_pthread_function ( callback );
	}
}

void llvm2nts::process (  )
{
	m_nts_module.name = m_llvm_module.getName().str().c_str();

	// Reserve space for global variables and constants
	const auto &gl = m_llvm_module.getGlobalList();
	m_nts_module.vars.reserve ( gl.size() );

	// Create NTS symbols from global variables and constants
	for ( auto &g : gl )
	{
		m_nts_module.vars.emplace_back ( "g_" + g.getName().str() );
		m_modmap.ins_iprint ( &g, &m_nts_module.vars.back() );
	}


	// TODO: What to do with function declarations?
	const auto &fl = m_llvm_module.getFunctionList();
	// Reserve the space now, because appending may invalidate
	// all iterators and reference.
	m_nts_module.bnts.reserve ( fl.size() );

	// Create one NTS for each function
	// and map each function to corresponding NTS
	for ( const Function & f : fl)
	{
		if ( f.getBasicBlockList().size() == 0 )
			continue;

		m_nts_module.bnts.emplace_back ( f.getName().str() );
		BasicNts *n = &m_nts_module.bnts.back();
		const Variable *v = n->add_variable ( "_ret_var" );
		n->add_return_variable ( v );
		v = n->add_variable ( "_lbb_var" );
		n->set_lbb_var ( v );
		m_modmap.ins_function ( &f, n );
	}

	process_pthreads();


	// From now do not insert elements.

	// Convert function to NTS
	// Note that the processing requires valid mapping 'Function -> BasicNts'
	unsigned int id = 0;
	for ( const Function &f : m_llvm_module )
	{
		if ( f.getBasicBlockList().size() == 0 )
			continue;

		llvmFunction2nts conv ( f, m_nts_module.bnts[id], m_modmap );
		conv.process();
		id++;
	}
}




