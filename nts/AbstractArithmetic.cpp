#include <stdexcept>
#include "AbstractArithmetic.hpp"

namespace NTS
{
	Printable::Prio AbstArithTermRelation::ac_to_prio(Connector ac)
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

	const char * AbstArithTermRelation::ac_to_sym(Connector ac)
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

	AbstArithTermRelation::AbstArithTermRelation(Connector ac, AbstArithTerm *t1, AbstArithTerm *t2)
		: AbstArithTerm(ac_to_prio(ac)), m_ac(ac), m_bob(t1, t2, ac_to_sym(ac))
	{
		;
	}

	void AbstArithTermRelation::print(const ConcreteCtx &ctx, std::ostream &o) const
	{
		m_bob.print(ctx, o, m_prio);
	}

	AbstArithValue::AbstArithValue(int id, bool future)
		: m_id(id), m_future(future)
	{
		;
	}

	void AbstArithValue::print(const NTS::ConcreteCtx &ctx, std::ostream &o) const
	{
		ctx.printVar(m_id, o);
		if (m_future)
			o << "'";
	}
};
