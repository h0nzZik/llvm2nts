#ifndef _NTS_CONCRETECTX_HPP_
#define _NTS_CONCRETECTX_HPP_

#include <ostream>
#include <vector>
#include <initializer_list>

#include "IPrint.hpp"

namespace NTS
{
	class ConcreteCtx
	{
		private:
			std::vector<const IPrint *> m_vars;

		public:
			ConcreteCtx(const std::initializer_list<const IPrint *>  &l);
			void printVar(int var, std::ostream &o) const;
	};
};



#endif // _NTS_CONCRETECTX_HPP_
