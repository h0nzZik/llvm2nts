#include <stdexcept>

#include "ModuleMapping.hpp"

using namespace nts;


ModuleMapping::~ModuleMapping()
{
	for ( auto &p : m_functions )
	{
		delete p.getSecond();
	}

	for ( auto &p : m_vars )
	{
		delete p.getSecond();
	}
}

void ModuleMapping::ins_function (
		const llvm::Function &fun,
		std::unique_ptr<BasicNtsInfo> i )
{
	m_functions.insert ( std::make_pair ( &fun, i.release() ) );
}

BasicNtsInfo & ModuleMapping::get_nts ( const llvm::Function &fun ) const
{
	auto * bn = m_functions.lookup ( &fun );
	if ( !bn )
		throw std::logic_error ( "BasicNts not found" );
	return *bn;
}

void ModuleMapping::ins_variable (
		const llvm::GlobalValue & llval,
		std::unique_ptr<VariableInfo> v )
{
	m_vars.insert ( std::make_pair ( &llval, v.release() ) );
}

VariableInfo & ModuleMapping::get_variable ( const llvm::GlobalValue & llval ) const
{
	auto v = m_vars.lookup ( &llval );
	if ( !v )
		throw std::logic_error ( "Global value not found" );
	return *v;
}

void ModuleMapping::ins_pthread_function ( const llvm::Function & f )
{
	auto it = m_ptf_id.find ( & f );
	if ( it != m_ptf_id.end() )
		return;

	m_ptf_id.insert ( std::make_pair ( &f, m_ptf.size() ) );
	m_ptf.push_back ( &f );
}

unsigned int ModuleMapping::get_pthread_function_id ( const llvm::Function & f ) const
{
	auto it = m_ptf_id.find ( &f );
	if ( it == m_ptf_id.end() )
		throw std::logic_error ( "Thread main not found" );

	return it->getSecond();
}

