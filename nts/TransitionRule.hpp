/**
 * @file   TransitionRule.hpp
 * @brief  Transition rules between states.
 * @date   8. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 *
 * According tu specification, transition rule can formula call or no rule at all:
 * <rule> ::= <formula> | <call> | \eps
 * In our case, TransitionRule can be ConcreteFormula (i.e. instantiated formula template)
 * or call.
 */

#ifndef NTS_TRANSITIONRULE_HPP_
#define NTS_TRANSITIONRULE_HPP_

#include <ostream>

namespace NTS
{
	class TransitionRule
	{
		public:
			TransitionRule() {};
			virtual ~TransitionRule() {};
			virtual void print ( std::ostream &o ) const = 0;
	};
}

#endif /* NTS_TRANSITIONRULE_HPP_ */
