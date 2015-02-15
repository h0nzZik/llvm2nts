#ifndef _NTS_HPP_
#define _NTS_HPP_

#include <vector>
#include "Formula.hpp"
#include "Variable.hpp"
#include "IPrint.hpp"

namespace NTS
{

	class State
	{
		private:
			int m_bb;
			int m_inst;
			bool m_final; // FIXME:: Not used

		public:
			State(int bb_id, int inst_id, bool st_final);
			State & operator=(State const &) = delete;

			void print(std::ostream &o) const;
	};

	class Transition
	{
		private:
			const State * m_from;
			const State * m_to;
			const ConcreteFormula m_guard;


		public:
			Transition(const State *a, const State *b, const ConcreteFormula &guard);
			Transition(const Transition &other);
			~Transition() {;}

			void print(std::ostream &o) const;
	};


	class BasicNts
	{
		private:
			std::string m_name;
			const Variable * m_retvar;
			std::vector<Variable *> m_variables;
			std::vector<Variable *> m_arguments;
			std::vector<const State *> m_states;
			std::vector<Transition> m_transitions;
	
			const State & pr_addState ( int bb_id, int inst_id, bool st_final );

		public:

			BasicNts();

			~BasicNts();

			void setRetVar(const Variable * ret_var);

			const Variable * getRetVar(void) const;

			void addVariable(Variable * var);

			void addArgument(Variable * arg);

			const State & addState ( int bb_id, int inst_id );

			const State & addFinalState ( int bb_id, int inst_id );

			const State & lastState() const;

			void addTransition(
					const State * from,
					const State * to,
					const ConcreteFormula &guard
					);

			void print(std::ostream &o) const;
	};


}

#endif // _NTS_HPP_
