#include <stdexcept>
#include <sstream>
#include <llvm/Support/Casting.h>
#include <llvm/IR/GlobalValue.h>
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"

#include "FunctionMapping.hpp"
#include "types.hpp"

using namespace llvm;
using namespace nts;
using std::make_pair;
using std::unique_ptr;

FunctionMapping::FunctionMapping ( BasicNts & nts, const ModuleMapping &mod ) :
	m_nts    ( nts ),
	m_modmap ( mod )
{
	;
}

unique_ptr<Variable> FunctionMapping::new_variable ( const Value &llval )
{
	std::stringstream ss;
	ss << "l_" << llval.getName().str();

	auto *v = new Variable (
			llvm_type_to_nts_type ( *llval.getType() ),
			ss.str()
	);

	m_vars.insert ( make_pair ( &llval, v ) );
	return unique_ptr<Variable> ( v );
}

void FunctionMapping::ins_variable ( const Value & llval, Variable & var )
{
	m_vars.insert ( make_pair ( &llval, &var ) );
}

Variable & FunctionMapping::get_variable ( const Value & value ) const
{
	if ( isa<GlobalValue> ( value ) )
		return m_modmap.get_variable ( cast<GlobalValue> ( value ) ).var;

	auto * found = m_vars.lookup ( & value );
	if ( found )
		return * found;

	throw std::logic_error ( "No such variable exists" );
}

#if 0
const NTS::IPrint * FunctionMapping::get_iprint ( int n )
{
	return m_nts.add_constant ( n );
}

const NTS::IPrint * FunctionMapping::get_iprint ( const llvm::Value *llval )
{
	const NTS::IPrint * found = m_values.lookup(llval);
	if ( found )
		return found;


	if (llvm::isa<llvm::GlobalValue>(llval))
		return m_modmap.get_iprint ( llvm:: cast<llvm::GlobalValue> ( llval ) );

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
#endif

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

unique_ptr < Leaf > FunctionMapping::new_leaf ( const Value & value ) const
{
	// We use variables for both arguments and results of instructions
	if ( isa < Argument >  ( value ) || isa < Instruction > ( value ) )
	{
		return nullptr;
	}

	if ( isa < User > ( value ) )
	{

		return nullptr;
	}

	throw std::domain_error ( "Given type is not supported as a leaf" );
}

