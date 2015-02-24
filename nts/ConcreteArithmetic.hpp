#ifndef NTS_CONCRETEARITHMETIC_HPP_
#define NTS_CONCRETEARITHMETIC_HPP_

#include "IPrint.hpp"
#include "Variable.hpp"


namespace NTS
{
	class ConArithLiteral : public IPrint
	{

	};

	class ConArithVariableIdentifier : public ConArithLiteral
	{
		private:
			const Variable * m_var;
			bool m_primed;
		public:
			ConArithVariableIdentifier(const Variable *var, bool primed);

			// ConArithLiteral -> IPrint
			virtual void print(std::ostream &o) const;
	};

	class ConPositiveNumeral final : public ConArithLiteral
	{
		private:
			std::string m_str;

		public:
			ConPositiveNumeral(const std::string &s);

			// ConArithLiteral -> IPrint
			virtual void print(std::ostream &o) const;
	};
};







#endif // NTS_CONCRETEARITHMETIC_HPP_
