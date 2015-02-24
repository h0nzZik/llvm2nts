#ifndef _NTS_VARIABLEIDENTIFIER_HPP_
#define _NTS_VARIABLEIDENTIFIER_HPP_

#include "Variable.hpp"
#include "ConcreteCtx.hpp"

namespace NTS
{
	class VariableIdentifier
	{
		private:
			int m_varid;
			bool m_primed;

		public:
			VariableIdentifier(int var_id, bool primed);

			void print(const ConcreteCtx &ctx, std::ostream &o) const;
	};
};

#endif // _NTS_VARIABLEIDENTIFIER_HPP
