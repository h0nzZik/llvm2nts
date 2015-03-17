#include <stdexcept>

#include "ModuleMapping.hpp"

void ModuleMapping::ins_function ( const llvm::Function *fun, const NTS::BasicNts *nts )
{
	m_functions.insert ( std::make_pair ( fun, nts ) );
}

const NTS::BasicNts * ModuleMapping::get_nts ( const llvm::Function *fun ) const
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

