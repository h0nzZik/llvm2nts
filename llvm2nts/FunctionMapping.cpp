#include <stdexcept>
#include <sstream>
#include <llvm/Support/Casting.h>
#include <llvm/IR/GlobalValue.h>
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"


#include "FunctionMapping.hpp"

FunctionMapping::FunctionMapping ( NTS::BasicNts & nts, const ModuleMapping &mod ) :
	m_modmap ( mod ),
	m_nts    ( nts )
{
	;
}

const NTS::Variable * FunctionMapping::ins_variable ( const llvm::Value *llval )
{
	std::stringstream ss;
	ss << "l_" << llval->getName().str();

	const NTS::Variable * v = m_nts.add_variable ( ss.str() ) ;

	ins_iprint ( llval, v );
	return v;
}

const NTS::IPrint * FunctionMapping::get_iprint ( const llvm::Value *llval )
{
	const NTS::IPrint * found = m_values.lookup(llval);
	if ( found )
		return found;

	if (llvm::isa<llvm::ConstantInt>(llval))
	{
		const auto *c = llvm:: cast<llvm::ConstantInt>(llval);
		const auto &v = c->getValue();

		// TODO: Make this more elegant
		std::string s;
		llvm::raw_string_ostream os(s);
		os << v;

		const NTS::Constant * co = m_nts.add_constant ( os.str() );
		ins_iprint ( llval, co );
		return co;
	}


	std::stringstream ss;
	if (llvm::isa<llvm::GlobalValue>(llval))
	{
		throw std::logic_error ( "Access to global variables are not supported" );
	}

	return ins_variable ( llval );
}


void FunctionMapping::ins_iprint ( const llvm::Value *llval, const NTS::IPrint *var )
{
	m_values.insert ( std::make_pair ( llval, var ) );
}

void FunctionMapping::ins_bb_start ( const llvm::BasicBlock *block, const NTS::State *s )
{
	m_block_start.insert ( std::make_pair ( block, s ) );
}

const NTS::State * FunctionMapping::get_bb_start ( const llvm::BasicBlock * block )
{
	auto * s = m_block_start.lookup ( block );
	if ( !s )
		throw std::logic_error ( "Start of basic block not found" );
	return s;
}

