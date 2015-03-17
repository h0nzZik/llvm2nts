/**
 * @file   Call.hpp
 * @brief  'Call' transition rule
 * @date   8. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 */

#include "util.hpp"
#include "Call.hpp"

using std::ostream;

namespace NTS
{
	Call::Call ( const BasicNts            * nts,
				 const std::vector
				 	 < const Variable * >  & outs,
				 const std::vector
					 < const IPrint   * >  & ins) :
		m_nts  ( nts ),
		m_ins  ( ins ),
		m_outs ( outs )
	{
		;
	}

	ostream & print_with_prime ( ostream &o, const Variable * const & v)
	{
		o << *v << "'";
		return o;
	};

	void Call::print ( ostream &o ) const
	{

		if ( m_outs.size() > 0 )
		{
			o << "( ";
			to_csv<const Variable *, print_with_prime> ( o, m_outs );
			o << " ) = ";
		}

		o << m_nts->get_name();
		o << " ( ";
		to_csv ( o, m_ins );
		o << " )";
	}
}

