/*
 * InstBr.cpp
 *
 *  Created on: 15. 2. 2015
 *      Author: jenda
 */

#include <stdexcept>
#include <libNTS/nts.hpp>
#include <libNTS/logic.hpp>
#include <libNTS/sugar.hpp>
#include "InstBr.hpp"

using std::invalid_argument;
using std::logic_error;
using namespace nts;
using namespace nts::sugar;

Formula & InstBr::formula_lbb_havoc (
		const BasicNtsInfo & bntsi,
		StateInfo & sti )
{
	return ( NEXT ( bntsi.lbb_var ) == sti.bb_id ) && havoc ( { bntsi.lbb_var } );
}

/**
 * @pre Q1: All BasicBlocks in current function must have
 *          their records and first states in given 'map' variable.
 */
void InstBr::process (
		const BasicNtsInfo      & bntsi,
		StateInfo               & sti,
		FunctionMapping         & map,
		const llvm::Instruction & i    )
{
	if (i.getOpcode() != llvm::Instruction::Br)
		throw invalid_argument("Not a branch instruction");

	const auto &br = llvm::cast<llvm::BranchInst>(i);

	auto flh = [ & bntsi, & sti ] () -> Formula & {
		return formula_lbb_havoc ( bntsi, sti );
	};

	if ( br.isConditional() )
	{
		llvm::BasicBlock * bb1 = br.getSuccessor ( 0 );
		llvm::BasicBlock * bb2 = br.getSuccessor ( 1 );
		if ( !bb1 || !bb2 )
			throw logic_error ( "Conditional 'br' should have two successors" );

		StateInfo & next_state_1 = map.get_bb_start ( *bb1 );
		StateInfo & next_state_2 = map.get_bb_start ( *bb2 );

		const llvm::Value & v = * br.getCondition();
		Formula & guard_1 = ( boolterm ( map.new_boolleaf ( v ) ) && flh() );
		Formula & guard_2 = ( ( !boolterm ( map.new_boolleaf ( v ) ) ) && flh() );

		Transition & tr_1 = ( *sti.st ->* *next_state_1.st ) ( guard_1 );
		Transition & tr_2 = ( *sti.st ->* *next_state_2.st ) ( guard_2 );

		tr_1.insert_to ( bntsi.bn );
		tr_2.insert_to ( bntsi.bn );
	}
	else
	{
		llvm::BasicBlock * bb = br.getSuccessor ( 0 );
		if ( !bb )
			throw logic_error ( "Unconditional 'br' should have one successor" );
		StateInfo & next_state = map.get_bb_start ( *bb );
		Transition & tr = ( *sti.st ->* *next_state.st ) ( flh() );
		tr.insert_to ( bntsi.bn );
	}
}

