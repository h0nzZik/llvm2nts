#include <ostream>
#include <vector>

template < typename T >
std::ostream & to_csv ( std::ostream &o, const std::vector<T *> & list )
{
	if ( list.empty() )
		return o;

	auto last = list.cend();
	last--;

	for ( auto it = list.cbegin(); it != last; it++	)
	{
		o << **it << ", ";
	}
	o << **last;

	return o;
}


