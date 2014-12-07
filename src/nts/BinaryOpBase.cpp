#include "BinaryOpBase.hpp"

namespace NTS
{
	BinaryOpBase::BinaryOpBase(const Printable *t1, const Printable *t2, const char *sym)
		:  m_t1(t1), m_t2(t2), m_sym(sym)
	{
		;
	}

	void BinaryOpBase::print(std::ostream &o, enum Printable::Prio prio) const
	{
		m_t1->wrapped_print(o, prio) << " " << m_sym << " ";

		m_t2->wrapped_print(o, prio);
	}
};

