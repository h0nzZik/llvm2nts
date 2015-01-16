#ifndef _NTS_IPRINT_HPP_
#define _NTS_IPRINT_HPP_

#include <ostream>

namespace NTS
{
	class IPrint
	{
		public:
			virtual void print(std::ostream &o) const = 0;
			virtual ~IPrint() {};
	};
};

#endif // _NTS_IPRINT_HPP
