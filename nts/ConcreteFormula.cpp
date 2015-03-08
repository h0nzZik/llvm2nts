/**
 * @file   ConcreteFormula.cpp
 * @brief  An instantiated formula template is a transition rule.
 * @date   8. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 */

#include "ConcreteFormula.hpp"

namespace NTS
{
	ConcreteFormula::ConcreteFormula (
					const Formula &f,
					const std::initializer_list<const IPrint *>  &variables
					)
		: m_f ( f ), m_ctx ( variables )
	{
		;
	}

	ConcreteFormula::ConcreteFormula ( const ConcreteFormula &cf )
		: m_f(cf.m_f), m_ctx(cf.m_ctx)
	{
		;
	}


	ConcreteFormula::ConcreteFormula ( const ConcreteFormula &&cf )
		: m_f(cf.m_f), m_ctx(cf.m_ctx)
	{
		;
	}

	void ConcreteFormula::print ( std::ostream &o ) const
	{
		m_f.print(m_ctx, o);
	}
}
