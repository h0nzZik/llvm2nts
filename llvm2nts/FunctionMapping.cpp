#include <stdexcept>
#include <sstream>
#include <llvm/Support/Casting.h>
#include <llvm/IR/GlobalValue.h>
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"

#include "FunctionMapping.hpp"
#include "types.hpp"
#include "util.hpp"

using namespace llvm;
using namespace nts;
using std::logic_error;
using std::make_pair;
using std::unique_ptr;

FunctionMapping::FunctionMapping ( BasicNts & nts, const ModuleMapping &mod ) :
	m_nts    ( nts ),
	m_modmap ( mod )
{
	;
}

FunctionMapping::~FunctionMapping()
{
	for ( auto &p : m_block_start )
	{
		delete p.getSecond();
		p.getSecond() = nullptr;
	}
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
	if ( ! llval.getType()->isIntegerTy() )
		throw logic_error ( "Value must be of integer type" );

	m_vars.insert ( make_pair ( &llval, &var ) );
}

void FunctionMapping::ins_pointer ( const Value & llval, Variable & var )
{
	if ( ! llval.getType()->isPointerTy() )
		throw logic_error ( "Value must be of pointer type" );

	m_pointers.insert ( make_pair ( &llval, &var ) );
}

nts::Variable * FunctionMapping::get_variable_noexcept
( const llvm::Value & value ) const noexcept
{
	auto * found = m_vars.lookup ( & value );
	if ( found )
		return  found;

	return nullptr;
}

Variable * FunctionMapping::get_variable_by_pointer_noexcept
( const Value & value ) const noexcept
{
	if ( isa<GlobalValue> ( value ) )
		return & m_modmap.get_variable ( cast<GlobalValue> ( value ) ).var;

	auto * found = m_pointers.lookup ( & value );
	if ( found )
		return found;

	return nullptr;
}

Variable & FunctionMapping::get_variable ( const Value & value ) const
{
	auto v = get_variable_noexcept ( value );
	if ( v )
		return *v;

	throw logic_error ( "No such variable exists" );
}

Variable & FunctionMapping::get_variable_by_pointer ( const Value & value ) const
{
	if ( ! value.getType()->isPointerTy() )
		throw logic_error ( "Not a pointer type" );

	auto * found = get_variable_by_pointer_noexcept ( value );
	if ( found )
		return *found;

	throw logic_error ( "Pointer does not point to any known variable" );
}



unique_ptr < VariableReference >
FunctionMapping::new_primed ( const Value & value ) const
{
	return unique_ptr < VariableReference > (
			new VariableReference (
				get_variable ( value ),
				true
			)
	);
}

void FunctionMapping::ins_bb_start (
		const BasicBlock         & block,
		unique_ptr < StateInfo >   s     )
{
	m_block_start.insert ( std::make_pair ( &block, s.release() ) );
}

StateInfo & FunctionMapping::get_bb_start ( const BasicBlock & block ) const
{
	const auto s = m_block_start.lookup ( & block );
	if ( !s )
		throw logic_error ( "Start of basic block not found" );

	return *s;
}

unique_ptr < Leaf > FunctionMapping::new_leaf ( const Value & value ) const
{
	if ( isa < llvm::Constant > ( value ) )
	{
		auto * leaf = new_constant (  cast < llvm::Constant > ( value ) );
		if ( leaf )
			return unique_ptr < Leaf > ( leaf );
		throw std::domain_error ( "Unsupported subtype of llvm::Constant" );
	}

	auto var = get_variable_noexcept ( value );
	if ( !var )
		throw std::domain_error ( "Can not create a leaf" );

	auto * ref = new VariableReference ( *var, false );
	return std::unique_ptr < Leaf > ( ref );
}

unique_ptr < Leaf > FunctionMapping::new_boolleaf ( const Value & value ) const
{
	if ( isa < llvm::Constant > ( value ) )
		return unique_ptr < Leaf > (
				new_bool_constant ( cast < llvm::Constant > ( value ) ) );

	auto var = get_variable_noexcept ( value );
	if ( !var )
		throw std::domain_error ( "No such variable" );

	if ( !var->type().is_scalar() || var->type().scalar_type() != ScalarType::Bool() )
		throw TypeError();

	auto * ref = new VariableReference ( *var, false );
	return std::unique_ptr < Leaf > ( ref );
}

