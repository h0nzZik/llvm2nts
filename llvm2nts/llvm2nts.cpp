// C++ STL headers
#include <stdexcept>
#include <ostream>
#include <sstream>
#include <utility>

// LLVM headers
#include <llvm/Pass.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

#include "llvmFunction2nts.hpp"
#include "types.hpp"
#include "util.hpp"

#include "llvm2nts.hpp"

using namespace llvm;
using namespace nts;
using std::string;
using std::make_pair;

struct llvm_2_nts
{
	const Module  & llvm_module;
	Nts           & nts;
	ModuleMapping   modmap;

	llvm_2_nts ( const Module & lm, Nts & n ) :
		llvm_module ( lm ),
		nts         ( n  )
	{
		;
	}

	/*
	 * For each function creates simple BasicNts with no logic.
	 */
	void create_function_prototypes();

	/*
	 * Creates global variables related to llvm global variables
	 */
	void create_global_variables();

	void set_global_variable_initializer (
		const nts::Variable  & var,
		const llvm::Constant & init
	);

	/*
	 * If pthreads are used, creates __thread_create
	 * Numbers thread functions
	 */
	void process_pthreads();

	void convert_functions();
};

void llvm_2_nts::create_function_prototypes ()
{
	for ( const Function & f : llvm_module)
	{
		// Skip declarations without actual body
		if ( f.getBasicBlockList().size() == 0 )
			continue;

		auto *bn = new BasicNts ( f.getName().str() );
		auto *i  = new BasicNtsInfo ( *bn );
		i->orig_function = &f;

		if ( ! f.getReturnType()->isVoidTy() )
		{
			auto *v = new Variable (
					llvm_type_to_nts_type ( *f.getReturnType() ),
					"_ret_var"
			);
			v->insert_param_out_to ( *bn );
			i->ret_var = v;
		}

		// Function parameters
		for ( auto &p : f.getArgumentList() )
		{
			auto *v = new Variable (
					llvm_type_to_nts_type ( *p.getType() ),
					string ( "arg_" ) + p.getName().str() );
			v->insert_param_in_to ( *bn );
			i->args.insert ( make_pair ( &p, v ) );
		}
		

		// FIXME: We assume there are only a few basicblocks
		auto *v = new Variable ( DataType::BitVector(16), "_lbb_var" );
		v->insert_to ( *bn );
		i->lbb_var = v;

		modmap.ins_function ( f, std::unique_ptr<BasicNtsInfo>(i) );


		bn->insert_to ( nts );
	}

}

void llvm_2_nts::process_pthreads()
{
	// Find function 'pthread_create()' (if it is declared)
	const Function * ptc = llvm_module.getFunction ( "pthread_create" );
	if ( !ptc )
		return;

	// Is it 'the' function?
	if ( ptc->getReturnType()->isVoidTy() )
		return;

	if ( ptc->getArgumentList().size() != 4 )
		return;

	if ( ptc->use_empty() )
		return;

	auto * thread_create = new BasicNts ( "__thread_create" );
	{
		auto * param_func_id = new Variable (
				DataType::BitVector ( 16 ), // FIXME
				"func_id" );
		param_func_id->insert_param_in_to ( *thread_create );
	}

	thread_create->insert_to ( nts );

	
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
		modmap.ins_pthread_function ( *callback );
	}
}

void llvm_2_nts::create_global_variables()
{
	for ( auto &g : llvm_module.getGlobalList() )
	{
		if ( !g.getType() || g.getType()->isVoidTy() )
			continue;

		auto *v = new Variable (
				llvm_type_to_nts_type ( *g.getType()->getElementType() ),
				std::string( "g_" ) + g.getName().str()
		);
		v->insert_to ( nts );

		auto *i = new VariableInfo ( *v );
		i->val = &g;

		modmap.ins_variable ( g, std::unique_ptr<VariableInfo>(i) );

		// Create initialization clause
		if ( g.hasInitializer() )
			set_global_variable_initializer ( *v, *g.getInitializer() );
	}
}

void llvm_2_nts::set_global_variable_initializer
(
		const nts::Variable  & var,
		const llvm::Constant & init
)
{
	auto leaf = std::unique_ptr < Leaf > ( new_constant ( init ) );
	if ( ! leaf )
		throw std::domain_error ( "Unsupported initializer of global value" );

	auto vr = std::make_unique <VariableReference> ( var, false );
	auto eq = std::make_unique < Relation > (
			RelationOp::eq,
			move ( vr ),
			move ( leaf )
	);

	std::unique_ptr < Formula > new_formula ;
	if ( nts.init() )
	{
		new_formula = std::make_unique < FormulaBop > (
				BoolOp::And,
				move ( eq ),
				move ( nts.init() )
		);
	}
	else
	{
		new_formula = move ( eq );
	}

	nts.init() = move ( new_formula );
}

void llvm_2_nts::convert_functions()
{
	for ( auto & p : modmap.ntses() )
	{
		convert_function ( *p.getFirst(), *p.getSecond(), modmap );
	}
}

std::unique_ptr<Nts> llvm_to_nts ( const llvm::Module & llvm_module )
{

	Nts *n = new Nts ( llvm_module.getModuleIdentifier() );
	llvm_2_nts l2n ( llvm_module, *n );
	l2n.create_function_prototypes();
	l2n.create_global_variables();
	l2n.process_pthreads();
	l2n.convert_functions();

	return std::unique_ptr<Nts>(n);
}




