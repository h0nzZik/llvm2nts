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

			friend std::ostream & operator<< ( std::ostream & s, const IPrint & ip );
	};

	inline std::ostream & operator<< ( std::ostream & s, const NTS::IPrint &p)
	{
		p.print ( s );
		return s;
	}
};

#endif // _NTS_IPRINT_HPP
