#include "VariableIdentifier.hpp"

namespace NTS
{
	VariableIdentifier::VariableIdentifier(int var_id, bool primed)
		: m_varid(var_id), m_primed(primed)

	{
		;
	}

	void VariableIdentifier::print(const ConcreteCtx &ctx, std::ostream &o) const
	{
		ctx.printVar(this->m_varid, o);
		if (m_primed)
			o << "'";
	}

};
