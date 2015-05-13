/**
 * @file   InstCall. hpp
 * @date   8. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 *
 * Implements 'call' instruction.
 */

#include <stdexcept>
#include <llvm/IR/Instructions.h>
#include <libNTS/nts.hpp>

#include "../util.hpp"
#include "InstCall.hpp"

using namespace nts;
using namespace llvm;

void InstCall::process (
				const BasicNtsInfo & bntsi,
				StateInfo          & sti,
				FunctionMapping    & map,
				const Instruction  & i    )
{
	auto st_next = new_state ( sti.bb_id, sti.inst_id );
	st_next->insert_after ( *sti.st );

	const auto &call     = llvm::cast < llvm::CallInst > ( i );

	// Called function
	const Function * dest_function = call.getCalledFunction();
	if (!dest_function)
		throw std::logic_error ( "Indirect function call is not supported" );

	
	// Call operands
	std::vector < Term * > args;

	bool is_ptc = dest_function->getName().equals ( "pthread_create" );

	Variable * ret_var = nullptr;

	BasicNts * dest_nts;

	if ( is_ptc )
	{
		// int pthread_create (
		// 		pthread_t *thread,
		// 		const pthread_attr_t *attr,
		// 		void *(*start_routine) (void *),
		// 		void *arg
		// );

		// Find 'thread_create' BasicNts
		if ( ! map.m_modmap.bnts_thread_create )
			throw std::logic_error ( "BasicNts 'thread_create' does not exist" );
		dest_nts = map.m_modmap.bnts_thread_create;

		// Find ID of given thread function
		const auto * arg = call.getArgOperand ( 2 );
		if ( !llvm::isa<llvm::Function> ( arg ) )
			throw std::logic_error ( "invalid argument type of pthread_create()" );
		const llvm::Function *thread_main = llvm::cast<llvm::Function> ( arg );
		unsigned id = map.m_modmap.get_pthread_function_id ( *thread_main );
		if ( id > INT_MAX )
			throw std::logic_error ( "thread_main function id too big" );

		// thread_create takes one argument - id of thread function
		args.push_back ( new nts::IntConstant ( id ) );

		// return variable should be the first argument
		ret_var = & map.get_variable_by_pointer ( *call.getArgOperand ( 0 ) );
	}
	else
	{
		BasicNtsInfo & dest_ntsi = map.m_modmap.get_nts ( *dest_function );
		dest_nts = & dest_ntsi.bn;

		if ( !dest_function->getReturnType()->isVoidTy() )
		{
			ret_var = map.new_variable ( call ).release();
			ret_var->insert_to ( bntsi.bn );
		}

		for (const llvm::Value * a : call.arg_operands())
		{
			args.push_back ( map.new_leaf ( *a ).release() );
		}
	}

	// now dest_nts points to "__thread_create" BasicNts

	CallTransitionRule * ctr;
	if ( ret_var )
		ctr = new CallTransitionRule ( *dest_nts, args, { ret_var } );
	else
		ctr = new CallTransitionRule ( *dest_nts, args, {} );

	auto transition = new Transition
		( std::unique_ptr < TransitionRule > ( ctr ),
			*sti.st, *st_next );

	transition->insert_to ( bntsi.bn );
	sti.st = st_next;
}
