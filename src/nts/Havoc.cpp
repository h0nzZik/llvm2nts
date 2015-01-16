#include "Havoc.hpp"

namespace NTS
{
	Havoc::Havoc(std::initializer_list<int> excluded_vars)
		: NTS::AtomicProposition(PR_Leaf), m_excluded_vars(excluded_vars)
	{
		;
	}

	void Havoc::print(const ConcreteCtx &ctx, std::ostream &o) const
	{
		o << "havoc(";
		if (0 != m_excluded_vars.size())
		{
			ctx.printVar(m_excluded_vars[0], o);
			for (auto it = m_excluded_vars.begin() + 1; it != m_excluded_vars.end(); it++)
			{
				o << ",";
				ctx.printVar(*it, o);
//				(*it)->print(o);
			}
		}
		o << ")";
	}

};
