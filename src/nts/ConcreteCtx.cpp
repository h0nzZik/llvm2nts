#include "ConcreteCtx.hpp"

namespace NTS
{

	ConcreteCtx::ConcreteCtx(const std::initializer_list<const IPrint *> &l)
		: m_vars(l)
	{
		;
	}

	void ConcreteCtx::printVar(int i, std::ostream &o) const
	{
		m_vars.at(i)->print(o);
	}

};
