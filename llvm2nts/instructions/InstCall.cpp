/**
 * @file   InstCall. hpp
 * @date   8. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 *
 * Implements 'call' instruction.
 */

#include <stdexcept>
#include "InstCall.hpp"

using namespace NTS;

InstCall::InstCall (  )
{

}

InstCall::~InstCall()
{
	
}

bool InstCall::supports(unsigned int opcode) const
{
	return opcode == llvm::Instruction::Call;
}

const State * InstCall::process(
		const NTS::State        * from    ,
		const llvm::Instruction & i       ,
		FunctionMapping         & map     ,
		NTS::BasicNts           & n       ,
		int                       bb_id   ,
		int                       inst_id )
{
	if (i.getOpcode() != llvm::Instruction::Call)
		throw std::invalid_argument("Unsupported llvm instruction");

	const auto &call     = llvm::cast < llvm::CallInst > ( i );

	// Call operands
	std::vector < const NTS::IPrint * > args;
	for (const llvm::Value * a : call.arg_operands())
	{
		args.push_back ( map.get_iprint ( a ) );
	}

	// Called function
	const llvm::Function * dest_function = call.getCalledFunction();
	if (!dest_function)
		throw std::logic_error ( "Indirect function call is not supported" );
	const NTS::BasicNts * dest_nts = map.m_modmap.get_nts ( dest_function );

	// Return variables
	std::vector < const NTS::Variable *> results ;
	if ( !dest_function->getReturnType()->isVoidTy() )
	{
		const NTS::Variable *result = map.ins_variable ( &llvm::cast<llvm::Value>(i) );
		results.push_back ( result );
	}

	NTS::Call c ( dest_nts, results, args );
	const TransitionRule *r  = n.add_transition_rule ( c );
	const NTS::State * st_to = n.addState ( bb_id, inst_id );
	n.add_transition ( from, st_to, r );
	return st_to;
}
