#ifndef _NTS_VARIABLE_HPP_
#define _NTS_VARIABLE_HPP_

#include <string>
#include "IPrint.hpp"

namespace NTS
{
	class Variable : public IPrint
	{
		private:
			std::string m_name;

		public:
			Variable(std::string name)
				:m_name(name)
			{
				;
			}

			// implementation of IPrint
			virtual void print(std::ostream &o) const
			{
				o << m_name;
			}
	};

	class Constant : public IPrint
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
