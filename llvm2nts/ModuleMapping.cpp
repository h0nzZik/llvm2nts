#include <stdexcept>

#include "ModuleMapping.hpp"

void ModuleMapping::ins_function ( const llvm::Function *fun, const NTS::NtsRef *nts )
{
	m_functions.insert ( std::make_pair ( fun, nts ) );
}

const NTS::NtsRef * ModuleMapping::get_nts ( const llvm::Function *fun ) const
{
	auto * nts = m_functions.lookup ( fun );
	if ( !nts )
		throw std::logic_error ( "Nts not found" );
	return nts;
}

void ModuleMapping::ins_iprint ( const llvm::GlobalValue *llval, const NTS::IPrint * ipr )
{
	m_values.insert ( std::make_pair ( llval, ipr ) );
}

const NTS::IPrint * ModuleMapping::get_iprint ( const llvm::GlobalValue *llval ) const
{
	const NTS::IPrint * ip = m_values.lookup ( llval );
	if ( !ip )
		throw std::logic_error ( "Global value not found" );
	return ip;
}

void ModuleMapping::ins_pthread_function ( const llvm::Function *f )
{
	m_ptf_id.insert ( std::make_pair ( f, m_ptf.size() ) );
	m_ptf.push_back ( f );
}

unsigned int ModuleMapping::get_pthread_function_id ( const llvm::Function * f ) const
{
	auto it = m_ptf_id.find ( f );
	if ( it == m_ptf_id.end() )
		throw std::logic_error ( "Thread main not found" );

	return it->getSecond();
}

