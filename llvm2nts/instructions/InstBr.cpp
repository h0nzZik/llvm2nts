/*
 * InstBr.cpp
 *
 *  Created on: 15. 2. 2015
 *      Author: jenda
 */

#include "InstBr.hpp"

using namespace NTS;

InstBr::InstBr() :
	m_havoc({})
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
	(void) bb_id;
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
		llvm::BasicBlock *b = br.getSuccessor ( 0 );
		to = map.get_bb_start ( b );
	}

	ConcreteFormula cf ( m_havoc, {} );
	n.addTransition ( from, to, cf );

	return NULL;
}
