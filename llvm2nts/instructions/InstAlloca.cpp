#include <llvm/IR/Instructions.h>

#include "InstAlloca.hpp"
#include "../types.hpp"

using namespace nts;
using namespace llvm;


void InstAlloca::process (
				const BasicNtsInfo & bntsi,
				StateInfo          & sti,
				FunctionMapping    & map,
				const Instruction  & i    )
{
	if ( i.getOpcode() != Instruction::Alloca )
		throw std::logic_error ( "Unsupported llvm instruction" );

	auto & alloca = cast < AllocaInst > ( i );

	if ( alloca.isArrayAllocation() )
		throw std::domain_error ( "Alloca: array allocation is not supported" );

	const auto & pt = * alloca.getType();

	auto * v = new Variable (
			llvm_type_to_nts_type ( * pt.getElementType() ),
			alloca.getName().str() );

	v->insert_to ( bntsi.bn );
	map.ins_pointer ( alloca, *v );
}

