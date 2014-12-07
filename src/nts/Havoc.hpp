#ifndef _NTS_HAVOC_HPP_
#define _NTS_HAVOC_HPP_

#include <vector>

#include "AtomicProposition.hpp"
#include "Variable.hpp"

namespace NTS
{
	class Havoc : public AtomicProposition
	{
		private:
			std::vector<const Variable *> m_excluded_vars;

		public:
			Havoc(std::initializer_list<const Variable *> excluded_vars);

			virtual void print(std::ostream &o) const;
	};
};


#endif
