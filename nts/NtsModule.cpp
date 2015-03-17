/**
 * @file   NtsModule.cpp
 * @date   17. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 */

#include "NtsModule.hpp"
#include "util.hpp"

namespace NTS
{
	namespace {
		template <typename T>
		std::ostream & print_noderef ( std::ostream &o, const T &x )
		{
			return o << x;
		}
	}

	void NtsModule::print ( std::ostream &o ) const
	{
		o << "NTS " << name << ";\n";

		if ( !vars.empty() )
		{
			to_csv<Variable, print_noderef<Variable>> ( o, vars );
			o << " : int;\n";
		}	

		for (const BasicNts & n : bnts)
		{
			n.print ( o );
		}
	}
}


