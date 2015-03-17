/**
 * @file   NtsRef.hpp
 * @date   17. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 */

#ifndef NTS_NTSREF_HPP_
#define NTS_NTSREF_HPP_

#include <string>
#include <ostream>

namespace NTS
{
	class NtsRef
	{
		public:
			virtual ~NtsRef() {};
			virtual const std::string & get_name() const = 0;

			friend std::ostream & operator<< ( std::ostream & s, const NtsRef & n );
	};

	inline std::ostream & operator<< ( std::ostream & s, const NtsRef &n )
	{
		s << n.get_name();
		return s;
	}

};



#endif /* NTS_NTSREF_HPP_ */
