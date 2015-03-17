/*
 * InstBr.cpp
 *
 *  Created on: 15. 2. 2015
 *      Author: jenda
 */

#include "InstBr.hpp"

using namespace NTS;

InstBr::InstBr() :
	m_aval_lbb   ( 0, true  ),
	m_aval_bbid  ( 1, false ),
	m_assign_lbb ( AtomicRelation::Relation::Eq, &m_aval_lbb, &m_aval_bbid ),
	m_havoc      ( { 0 } ),
	m_formula    ( BoolOp::And, &m_assign_lbb, &m_havoc )
{

}

InstBr::~InstBr()
{
	
}

bool InstBr::supports(unsigned int opcode) const
{
	return opcode == llvm::Instruction::Br;
}

const State * InstBr::process(
		const NTS::State        * from    ,
		const llvm::Instruction & i       ,
		FunctionMapping         & map     ,
		NTS::BasicNts           & n       ,
		int                       bb_id   ,
		int                       inst_id )
{
	(void) inst_id;

	if (i.getOpcode() != llvm::Instruction::Br)
		throw std::invalid_argument("Unsupported llvm instruction");

	const auto &br = llvm::cast<llvm::BranchInst>(i);
	const NTS::State * to;
	if (br.isConditional())
	{
		return from; // TODO implement
		throw std::logic_error("Conditional branch is not implemented");
	} else {
		const llvm::BasicBlock *b = br.getSuccessor ( 0 );
		to = map.get_bb_start ( b );
	}

	// Variable in which id of last visited basic block is stored
	const IPrint *lbb = n.get_lbb_var();
	// Constant with this basic block id
	const IPrint *val = n.add_constant ( bb_id );
	
	ConcreteFormula cf ( m_formula, { lbb, val } );
	const TransitionRule * r = n.add_transition_rule ( cf );
	n.add_transition ( from, to, r );

	return NULL;
}
