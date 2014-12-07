#ifndef _NTS_VARIABLEIDENTIFIER_HPP_
#define _NTS_VARIABLEIDENTIFIER_HPP_

#include "Variable.hpp"

namespace NTS
{
	class VariableIdentifier
	{
		private:
			const Variable *m_var;
			bool m_primed;

		public:
			VariableIdentifier(const Variable *var, bool primed)
				: m_var(var), m_primed(primed) {;}

			void print(std::ostream &o) const
			{
				m_var->print(o);
				if (m_primed)
					o << "'";
			}
	};
};

#endif // _NTS_VARIABLEIDENTIFIER_HPP
