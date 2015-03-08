#include <ostream>
#include <vector>

namespace {
	template < typename T>
	std::ostream & default_print ( std::ostream &o, const T & x )
	{
		o << *x;
		return o;
	}
}

template < typename T, std::ostream & (*Print) ( std::ostream &, const T &) = default_print<T> >
std::ostream & to_csv ( std::ostream &o, const std::vector<T> & list )
{
	if ( list.empty() )
		return o;

	auto last = list.cend();
	last--;

	for ( auto it = list.cbegin(); it != last; it++	)
	{
		Print ( o, *it ) << ", ";
	}
	Print ( o, *last );
	//o << **last;

	return o;
}


