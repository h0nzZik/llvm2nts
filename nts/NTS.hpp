#ifndef _NTS_HPP_
#define _NTS_HPP_

#include <vector>
#include "Formula.hpp"
#include "Variable.hpp"
#include "IPrint.hpp"
#include "TransitionRule.hpp"
#include "ConcreteFormula.hpp"
#include "Call.hpp"
#include "NtsRef.hpp"

namespace NTS
{

	class State
	{
		public:
			virtual void print ( std::ostream &o ) const = 0;
	};

	class CommonState final : public State
	{
		private:
			int m_bb;
			int m_inst;

		public:
			CommonState ( int bb_id, int inst_id );
			CommonState & operator= ( State const & src ) = delete;

			virtual void print ( std::ostream &o ) const;
	};

	class FinalState final : public State
	{
		public:
			virtual void print ( std::ostream &o ) const;
	};

	class Transition final
	{
		private:
			const State * m_from;
			const State * m_to;
			const TransitionRule * m_rule;


		public:
			Transition ( const State *a, const State *b, const TransitionRule * rule );
			Transition ( const Transition &other );
			Transition ( Transition && old );
			~Transition() {;}

			void print(std::ostream &o) const;
	};

	// Forward declaration
	class Call;

	class BasicNts : public NtsRef
	{
		private:
			std::string                       m_name;
			std::vector < Variable        * > m_variables;
			std::vector < Constant        * > m_constants;
			std::vector < Variable        * > m_arguments;
			std::vector < CommonState     * > m_states;
			std::vector < ConcreteFormula * > m_cformulas;
			std::vector < Call            * > m_calls;
			std::vector < Transition        > m_transitions;
			FinalState                      * m_final_st;
			std::vector < const Variable  * > m_retvars;
			const Variable                  * m_var_lbb;
	
		public:

			BasicNts ( const std::string & name );
			BasicNts ( const BasicNts & cp ) = delete;
			BasicNts ( BasicNts && old);

			~BasicNts();

			BasicNts & operator= ( const BasicNts & orig) = delete;

			virtual const std::string & get_name () const override;


			void set_lbb_var ( const Variable * var );
			const Variable * get_lbb_var () const;

			const Variable * add_variable ( const std::string & name );

			size_t add_return_variable ( const Variable * var );

			const Variable * get_return_variable ( size_t id = 0) const;


			const Variable * add_argument ( const std::string & name );

			const Constant * add_constant ( int value );
			const Constant * add_constant ( const std::string & value );

			const CommonState * addState ( int bb_id, int inst_id );

			const CommonState * lastState() const;

			const FinalState * final_state() const;

			const TransitionRule * add_transition_rule ( const ConcreteFormula & cf );

			const TransitionRule * add_transition_rule ( const Call & call );
	
			void add_transition (
					const State          * from,
					const State          * to,
					const TransitionRule * rule
					);

			void print(std::ostream &o) const;
	};


}

#endif // _NTS_HPP_
