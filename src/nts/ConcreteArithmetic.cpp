#include "ConcreteArithmetic.hpp"

namespace NTS
{
	ConArithVariableIdentifier::ConArithVariableIdentifier(const Variable *var, bool primed)
		: m_var(var), m_primed(primed)
	{
		;
	}

	void ConArithVariableIdentifier::print(std::ostream &o) const
	{
		m_var->print(o);
		if (m_primed)
			o << "'";
	}


	ConPositiveNumeral::ConPositiveNumeral(const std::string &s)
		: m_str(s)
	{
		;
	}

	void ConPositiveNumeral::print(std::ostream &o) const
	{
		o << m_str;
	}

}
