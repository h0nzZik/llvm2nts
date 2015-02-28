#include <stdexcept>

#include "ModuleMapping.hpp"

void ModuleMapping::ins_function ( const llvm::Function *fun, const NTS::BasicNts *nts )
{
	m_functions.insert ( std::make_pair ( fun, nts ) );
}

const NTS::BasicNts * ModuleMapping::get_nts ( const llvm::Function *fun )
{
	auto * nts = m_functions.lookup ( fun );
	if ( !nts )
		throw std::logic_error ( "Nts not found" );
	return nts;
}

