/**
 * @file   NtsModule.hpp
 * @date   17. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 */

#ifndef NTS_NTSMODULE_HPP_
#define NTS_NTSMODULE_HPP_

// C++ headers
#include <string>
#include <ostream>

#include "NTS.hpp"
#include "Variable.hpp"

namespace NTS
{

struct NtsModule final
{
	std::string              name;
	std::vector < BasicNts > bnts;
	std::vector < Variable > vars;

	void print ( std::ostream & o ) const;
};

} /* namespace NTS */

#endif /* NTS_NTSMODULE_HPP_ */
