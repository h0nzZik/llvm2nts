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
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IRReader/IRReader.h>

// libNTS
#include <libNTS/sugar.hpp>

#include "llvmFunction2nts.hpp"
#include "types.hpp"
#include "util.hpp"

#include "llvm2nts.hpp"

using namespace llvm;
using namespace nts;
using namespace nts::sugar;
using std::string;
using std::make_pair;
using std::to_string;
using std::vector;
using std::unique_ptr;


struct llvm_2_nts
{
	const Module  & llvm_module;
	Nts             & nts;
	const llvm2nts_options & opts;

	ModuleMapping   modmap;
	bool need_threads;

	// How many threads there can be in one time
	unsigned int thread_pool_size;
	// Which thread is running which function. Array variable
	Variable * var_thread_pool_selected;
	Variable * var_thread_pool_lock;
	BasicNts * bnts_thread_pool_routine;

	llvm_2_nts ( const Module & lm, Nts & n, const llvm2nts_options & opts ) :
		llvm_module ( lm   ),
		nts         ( n    ),
		opts        ( opts )
	{
		need_threads             = false;
		var_thread_pool_lock     = nullptr;
		var_thread_pool_selected = nullptr;
		bnts_thread_pool_routine = nullptr;
	}

	/*
	 * For each function creates simple BasicNts with no logic.
	 * @pre Thread functions must be found,
	 *      because their parameters are removed (pointers).
	 *      Also their return type should be changed to void
	 *      ( and all ret instructions should return null,
	 *        and all calls and pthread_create passing
	 *        can use only null arguments ).
	 *
	 */
	void create_function_prototypes();

	/*
	 * Creates global variables related to llvm global variables
	 * @pre nothing
	 */
	void create_global_variables();

	void set_global_variable_initializer (
		nts::Variable  & var,
		const llvm::Constant & init
	);

	/*
	 * If pthreads are used, creates __thread_create
	 * Finds and numbers thread functions
	 */
	void process_pthreads();

	/*
	 * @pre 1. There must be some caller to 'pthread_create' function
	 *         - process_pthreads() must be called first
	 */
	void add_thread_create();
	void add_thread_pool_variables();
	/*
	 * @pre 1. Thread pool variables must be created
	 *         - @see add_thread_pool_variables()
	 *      2. Thread routines must have created their prototypes
	 *         - @see create_function_prototypes
	 */
	void add_thread_pool_routine();

	/*
	 * @pre An array of running thread functions must be created.
	 */
	State & add_thread_poll_select_transition ( State & from, unsigned int fun_id );
	void add_thread_poll_call_transition ( State & from, State & to, BasicNts & bn );

	/*
	 * @pre Needs function prototypes and global variables
	 */
	void convert_functions();

	void add_instances();
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

		if ( modmap.is_thread_function ( f ) )
			i->is_ptf = true;

		if ( ( !f.getReturnType()->isVoidTy() ) && ( !i->is_ptf ) )
		{
			auto *v = new Variable (
					llvm_type_to_nts_type ( *f.getReturnType() ),
					"_ret_var"
			);
			v->insert_param_out_to ( *bn );
			i->ret_var = v;
		}

		if ( !i->is_ptf )
		{
			// Function parameters
			for ( auto &p : f.getArgumentList() )
			{
				auto *v = new Variable (
						llvm_type_to_nts_type ( *p.getType() ),
						string ( "arg_" ) + p.getName().str() );
				v->insert_param_in_to ( *bn );
				i->args.insert ( make_pair ( &p, v ) );
			}
		}
		

		// FIXME: We assume there are only a few basicblocks
		auto *v = new Variable (
				DataType( ScalarType::BitVector(16) ),
				"_lbb_var"
		);
		v->insert_to ( *bn );
		i->lbb_var = v;

		modmap.ins_function ( f, std::unique_ptr<BasicNtsInfo>(i) );


		bn->insert_to ( nts );
	}

}

void llvm_2_nts::add_thread_create()
{
	if ( ! need_threads )
		return;

	BasicNts * thread_create = new BasicNts ( "__thread_create" );
	Variable * param_func_id = new Variable (
			DataType ( ScalarType::BitVector ( 32 ) ),
			"func_id"
	);
	param_func_id->insert_param_in_to ( *thread_create );
	

	State & si = * new State ( "si" ); // 'initial'
	State & sf = * new State ( "sf" ); // 'final'
	State & se = * new State ( "se" ); // 'error'
	State & sl = * new State ( "sl" ); // 'locked'
	State & sh = * new State ( "sh" ); // 'have'
	State & sn = * new State ( "sn" ); // 'go to Next'

	si.insert_to ( *thread_create );
	sf.insert_to ( *thread_create );
	se.insert_to ( *thread_create );
	sl.insert_to ( *thread_create );
	sh.insert_to ( *thread_create );
	sn.insert_to ( *thread_create );

	si.is_initial() = true;
	sf.is_final()   = true;
	se.is_error()   = true;

	Variable * var_thread_id = new Variable (
			DataType ( ScalarType::BitVector ( 32 ) ),
			"thread_id"
	);
	var_thread_id->insert_to ( *thread_create );

	ArrRead selected ( * var_thread_pool_selected );

	Formula & si_to_locked_f = (
	(
			( CURR(var_thread_pool_lock) == 0)
		&&
			( NEXT(var_thread_pool_lock) == ( tid() + 1) )
	)
	&&
	(
	 		( NEXT(var_thread_id) == 0)
		&&
			( havoc ({ var_thread_pool_lock, var_thread_id }) )
	));

	Transition & si_to_locked = ( si ->* sl ) ( si_to_locked_f );

	si_to_locked.insert_to ( *thread_create );

	Formula & f_found = (
	(
	 	selected [ CURR(var_thread_id) ] == 0

	)
	&&
	(
		(
			* new ArrayWrite (
				*var_thread_pool_selected,
				{},
				{ & CURR (var_thread_id) },
				{ &( CURR (param_func_id) + 1) }
			)
		)
		&&
			havoc ( { var_thread_pool_selected } )
	)
	);

	Transition & t_found = ( sl ->* sh ) ( f_found );
	t_found.insert_to ( *thread_create );

	Formula & f_not_found =  (
	 (
	  	selected [ CURR(var_thread_id) ] > 0
	)
	&&
	 	havoc()
	);

	Transition & t_not_found = ( sl ->* sn ) ( f_not_found );
	t_not_found.insert_to ( *thread_create );

	Formula & f_goto_next = (
		( CURR(var_thread_id) < (thread_pool_size - 1))
	&&
	(		(NEXT(var_thread_id) == ( CURR(var_thread_id) + 1 ) )
		&&
	 		havoc ( { var_thread_id } )
	)
	);

	Transition & t_goto_next = ( sn ->* sl ) ( f_goto_next );
	t_goto_next.insert_to ( *thread_create );


	Formula & f_error = (
		( CURR(var_thread_id) >= ( thread_pool_size - 1 ))
	&&
		havoc()	
	);

	Transition & t_err = ( sn ->* se ) ( f_error );
	t_err.insert_to ( *thread_create );

	// If we have it, unlock
	Formula & f_unlock = (
		( NEXT(var_thread_pool_lock) == 0)
	&& 
		havoc({var_thread_pool_lock})
	);

	Transition & t_unlock = ( sh ->* sf ) ( f_unlock );
	t_unlock.insert_to ( *thread_create );

	thread_create->insert_to ( nts );

	modmap.bnts_thread_create = thread_create;
}


// s_idle -> s_running_2 { __thread_pool_selected[tid] = 2 && havoc() }
State & llvm_2_nts::add_thread_poll_select_transition ( State & from, unsigned int fun_id )
{
	auto * s_running = new State ( string ( "s_running_" ) + to_string ( fun_id ) );
	s_running->insert_to ( *bnts_thread_pool_routine );

	auto tr_select = new Transition (
		std::make_unique < FormulaTransitionRule > (
			std::make_unique < FormulaBop > (
				BoolOp::And,
				std::make_unique < Relation > (
					RelationOp::eq,
					std::make_unique < ArrayTerm > (
						std::make_unique < VariableReference > (
							*var_thread_pool_selected,
							false
						),
						vector < Term * > {
							new ThreadID()
						}
					),
					make_unique < IntConstant > ( fun_id )
				),
				// </ Relation >
				std::make_unique < Havoc > ( )
			)
			// </FormulaBop >
		),
		from,
		*s_running
	);
	tr_select->insert_to ( *bnts_thread_pool_routine );


	return *s_running;
}

// s_running_2 -> s_stopped { thread_start_routine_2()  }
void llvm_2_nts::add_thread_poll_call_transition (
		State & from,
		State & to,
		BasicNts & bn )
{
	auto tr_stop = new Transition (
		std::make_unique < CallTransitionRule > (
			bn,
			vector < Term * > ( {} ),
			vector < Variable * > ( {} )
		),
		from,
		to
	);

	tr_stop->insert_to ( *bnts_thread_pool_routine );
}

void llvm_2_nts::add_thread_pool_routine()
{
	if ( ! need_threads )
		return;

	bnts_thread_pool_routine = new BasicNts ( "__thread_pool_routine" );

	auto * s_idle    = new State ( "s_idle" );
	auto * s_stopped = new State ( "s_stopped" );

	s_idle   ->insert_to ( *bnts_thread_pool_routine );
	s_stopped->insert_to ( *bnts_thread_pool_routine );

	s_idle->is_initial() = true;

	const auto & funs = modmap.pt_funs();
	for ( unsigned int i = 0; i < funs.size(); i++ )
	{
		unsigned int id = i + 1;

		State & s_running = add_thread_poll_select_transition ( *s_idle, id );
		add_thread_poll_call_transition (
				s_running,
				*s_stopped,
				modmap.get_nts ( *funs[i] ).bn
		);

		// ( __thread_poll_selected'[tid] = 0 ) && havoc(__thread_poll_selected)
		ArrWrite wr_tps ( *var_thread_pool_selected );
		Formula & f_to_idle = ( wr_tps[tid()] == 0) && 
			havoc ( { var_thread_pool_selected } );

		Transition & t = ( *s_stopped ->* *s_idle ) ( f_to_idle );
		t.insert_to ( *bnts_thread_pool_routine );
	}

	bnts_thread_pool_routine->insert_to ( nts );
}

void llvm_2_nts::add_thread_pool_variables()
{
	if ( !need_threads )
		return;

	var_thread_pool_lock = new Variable (
			DataType ( ScalarType::BitVector ( 32 ) ),
			"__thread_pool_lock"
	);
	var_thread_pool_lock->insert_to ( nts );

	var_thread_pool_selected = new Variable (
			DataType (
				ScalarType::BitVector ( 32 ),
				0,
				std::vector < Term * > {
					new IntConstant (thread_pool_size )
				}
			),
			"__thread_pool_selected"
	);
	var_thread_pool_selected->insert_to ( nts );
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

	need_threads = true;
	thread_pool_size = opts.thread_poll_size;
	
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
		nts::Variable  & var,
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
	if ( nts.initial_formula )
	{
		new_formula = std::make_unique < FormulaBop > (
				BoolOp::And,
				move ( eq ),
				move ( nts.initial_formula )
		);
	}
	else
	{
		new_formula = move ( eq );
	}

	nts.initial_formula = move ( new_formula );
	nts.initial_formula->_parent_ptr.nts = & nts;
	nts.initial_formula->_parent_type = Formula::ParentType::NtsInitialFormula;
}

void llvm_2_nts::convert_functions()
{
	for ( auto & p : modmap.ntses() )
	{
		convert_function ( *p.getFirst(), *p.getSecond(), modmap );
	}
}

void llvm_2_nts::add_instances()
{
	if ( need_threads )
	{
		Instance * i = new Instance ( bnts_thread_pool_routine, thread_pool_size );
		i->insert_to ( nts );

		// find main
		BasicNts * main = nullptr;
		for ( BasicNts * bn : nts.basic_ntses() )
		{
			if ( 0 == bn->name.compare ( "main" ) )
			{
				main = bn;
				break;
			}
		}

		// This could be checked sooner, but..
		if ( !main )
			throw std::runtime_error ( "BasicNts 'main' is missing" );

		Instance * i_main = new Instance ( main, 1 );
		i_main->insert_to ( nts );
	}
}


std::unique_ptr<Nts> llvm_to_nts (
		const llvm::Module & llvm_module,
		const llvm2nts_options * opts )
{
	llvm2nts_options defaults;
	defaults.thread_poll_size = 20;
	if ( ! opts )
		opts = & defaults;


	Nts *n = new Nts ( llvm_module.getModuleIdentifier() );
	llvm_2_nts l2n ( llvm_module, *n, *opts );
	//l2n.create_function_prototypes();
	l2n.create_global_variables();
	l2n.process_pthreads();
	l2n.create_function_prototypes();
	l2n.add_thread_pool_variables();
	l2n.add_thread_pool_routine();
	l2n.add_thread_create();
	l2n.add_instances();
	l2n.convert_functions();

	return std::unique_ptr<Nts>(n);
}


std::unique_ptr<Nts> llvm_file_to_nts (
		const string filename,
		const llvm2nts_options * opts )
{
	llvm::SMDiagnostic diag;
	llvm::LLVMContext  ctx;

	unique_ptr<llvm::Module> m = llvm::parseIRFile ( filename, diag, ctx );

	if ( !m )
		return nullptr;


	unique_ptr < nts::Nts > nts;
	try {
		nts = llvm_to_nts ( *m, opts );
	}
	catch ( const std::exception & e )
	{
		return nullptr;
	}

	return nts;
}

