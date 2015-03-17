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
	std::vector < Constant > consts;

	void print ( std::ostream & o ) const;
};

void NtsModule::print(std::ostream &o) const
{
	o << "NTS " << name << ";\n";
	for (const BasicNts & n : bnts)
	{
		n.print ( o );
	}
}


} /* namespace NTS */

#endif /* NTS_NTSMODULE_HPP_ */
