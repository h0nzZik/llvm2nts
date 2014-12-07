#include "Formula.hpp"

namespace NTS
{
	FormulaBop::FormulaBop(BoolOp op, Formula *f1, Formula *f2)
		: Formula(BoolOp_toPrio(op)), m_bob(f1, f2, BoolOp_toString(op))
	{
		;
	}

	void FormulaBop::print(std::ostream &o) const
	{
		m_bob.print(o, Printable::m_prio);
	}

};
