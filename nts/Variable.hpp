#ifndef _NTS_VARIABLE_HPP_
#define _NTS_VARIABLE_HPP_

#include <utility>
#include <string>
#include "IPrint.hpp"

namespace NTS
{
	class Variable final : public IPrint
	{
		private:
			const std::string m_name;

		public:
			Variable ( const std::string & name ) :
				m_name(name)
			{
				;
			}

			Variable ( std::string && name) :
				m_name ( std::move ( name ) )
			{
				;
			}

			// implementation of IPrint
			virtual void print ( std::ostream &o ) const
			{
				o << m_name;
			}
	};

	class Constant final : public IPrint
	{
		private:
			std::string m_name;

		public:
			Constant(std::string name)
				:m_name(name)
			{
				;
			}

			virtual void print(std::ostream &o) const
			{
				o << m_name;
			}
	};
};


#endif // _NTS_VARIABLE_HPP
