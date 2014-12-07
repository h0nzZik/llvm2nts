#include "BooleanTerm.hpp"

namespace NTS
{
	
	BoolTermRelation::BoolTermRelation(BoolOp op, BooleanTerm *t1, BooleanTerm *t2)
		: BooleanTerm(BoolOp_toPrio(op)), m_bob(t1, t2, BoolOp_toString(op)), m_op(op)
	{
		;
	}

	void BoolTermRelation::print(std::ostream &o) const
	{
		m_bob.print(o, m_prio);
	}

};
