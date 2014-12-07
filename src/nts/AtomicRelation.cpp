#include <stdexcept>
#include "AtomicRelation.hpp"


namespace NTS
{

	const char * AtomicRelation::rel_to_sym(Relation rel)
	{
		switch(rel)
		{
			case Relation::Eq:
				return "=";
			case Relation::Neq:
				return "!=";
			case Relation::Le:
				return "<=";
			case Relation::Lt:
				return "<";
			case Relation::Ge:
				return ">=";
			case Relation::Gt:
				return ">";
			default:
				throw std::domain_error("Invalid relation");
		}
	}

	// TODO: optimize brackets
	AtomicRelation::AtomicRelation(Relation rel, ArithmeticTerm *t1, ArithmeticTerm *t2)
		: AtomicProposition(PR_AlwaysBrackets), m_bob(t1, t2, rel_to_sym(rel))
	{
		;
	}

	void AtomicRelation::print(std::ostream &o) const
	{
		m_bob.print(o, Printable::m_prio);
	}

};
