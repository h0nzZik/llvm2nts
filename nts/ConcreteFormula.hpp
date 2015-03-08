/**
 * @file   ConcreteFormula.hpp
 * @brief  An instantiated formula template is a transition rule.
 * @date   8. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 */

#ifndef NTS_CONCRETEFORMULA_HPP_
#define NTS_CONCRETEFORMULA_HPP_

#include "TransitionRule.hpp"
#include "Formula.hpp"

namespace NTS
{
	class ConcreteFormula : public TransitionRule
	{
		private:
			const Formula & m_f;
			ConcreteCtx m_ctx;

		public:
			ConcreteFormula (
				const Formula &f,
				const std::initializer_list<const IPrint *>  &variables
				);

			ConcreteFormula ( const ConcreteFormula  & cf );
			ConcreteFormula ( const ConcreteFormula && cf );

			~ConcreteFormula() {;}

			virtual void print ( std::ostream &o ) const override;
	};
}


#endif /* NTS_CONCRETEFORMULA_HPP_ */
