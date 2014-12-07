#ifndef _NTS_VARIABLE_HPP_
#define _NTS_VARIABLE_HPP_

#include <string>

namespace NTS
{
	class Variable
	{
		private:
			std::string m_name;

		public:
			Variable(std::string name)
				:m_name(name)
			{
				;
			}

			void print(std::ostream &o) const
			{
				o << m_name;
			}
	};
};


#endif // _NTS_VARIABLE_HPP
