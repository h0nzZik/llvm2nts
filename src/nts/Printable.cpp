#include "Printable.hpp"

namespace NTS
{
	Printable::Printable(Prio prio) : m_prio(prio) {;}

	std::ostream & Printable::wrapped_print(std::ostream &o, Prio prio) const
	{
		const bool br = m_prio <= prio;
		if(br)
			o << "(";
		print(o);
		if (br)
			o << ")";
		return o;
	}
};
