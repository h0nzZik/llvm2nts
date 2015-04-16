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

	const BasicNtsInfo & dest_ntsi = map.m_modmap.get_nts ( *dest_function );
	
	// Call operands
	std::vector < Term * > args;

	bool is_ptc = dest_function->getName().equals ( "pthread_create" );

	Variable * ret_var = nullptr;

	if ( is_ptc )
	{
		throw std::domain_error ( "pthread_create() not implemented" );
#if 0
		// now dest_nts points to "__thread_create" NTS 
		const auto * arg = call.getArgOperand ( 2 );
		if ( !llvm::isa<llvm::Function> ( arg ) )
			throw std::logic_error ( "invalid argument type of pthread_create()" );
		
		const llvm::Function *thread_main = llvm::cast<llvm::Function> ( arg );
		unsigned id = map.m_modmap.get_pthread_function_id ( thread_main );
		if ( id > INT_MAX )
			throw std::logic_error ( "thread_main function id too big" );

		const auto * ip = map.get_iprint ( (int)id );
		args.push_back ( ip );
#endif
	}
	else
	{
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

	CallTransitionRule * ctr;
	if ( ret_var )
		ctr = new CallTransitionRule ( dest_ntsi.bn, args, { ret_var } );
	else
		ctr = new CallTransitionRule ( dest_ntsi.bn, args, {} );

	auto transition = new Transition
		( std::unique_ptr < TransitionRule > ( ctr ),
			*sti.st, *st_next );

	transition->insert_to ( bntsi.bn );
	sti.st = st_next;
}
