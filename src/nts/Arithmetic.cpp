#include <stdexcept>
#include "Arithmetic.hpp"

namespace NTS
{
	Printable::Prio ArithmeticTermRelation::ac_to_prio(Connector ac)
	{
		switch(ac)
		{
			case Connector::Add:
				return PR_Add;
			case Connector::Mul:
				return PR_Mul;
			case Connector::Div:
				return PR_Div;
			case Connector::Mod:
				return PR_Mod;
			default:
				throw std::domain_error("Invalid connector");
		}
	}

	const char * ArithmeticTermRelation::ac_to_sym(Connector ac)
	{
		switch(ac)
		{
			case Connector::Add:
				return "+";
			case Connector::Mul:
				return "*";
			case Connector::Div:
				return "/";
			case Connector::Mod:
				return "%";
			default:
				throw std::domain_error("Invalid connector");
		}
	}

	ArithmeticTermRelation::ArithmeticTermRelation(Connector ac, ArithmeticTerm *t1, ArithmeticTerm *t2)
		: ArithmeticTerm(ac_to_prio(ac)), m_ac(ac), m_bob(t1, t2, ac_to_sym(ac))
	{
		;
	}
};
