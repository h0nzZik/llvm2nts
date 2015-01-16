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
			int m_st;
			bool m_final; // FIXME:: Not used

		public:
			State(int st, bool st_final);
			State & operator=(State const &) = delete;

			void print(std::ostream &o) const;
	};

	class Transition
	{
		private:
			const State * m_from;
			const State * m_to;
			const Formula * m_guard;
			const ConcreteCtx m_ctx;

		public:
			Transition(
					const State *a,
					const State *b,
					const Formula *guard,
					const std::initializer_list<const IPrint *>  &variables
					);

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


			const State & pr_addState(bool st_final);

		public:

			BasicNts();

			~BasicNts();

			void setRetVar(const Variable * ret_var);

			const Variable * getRetVar(void) const;

			void addVariable(Variable * var);

			void addArgument(Variable * arg);

			const State & addState();

			const State & addFinalState();

			const State & lastState() const;

			void addTransition(
					const State * from,
					const State * to,
					const Formula *guard,
					const std::initializer_list<const IPrint *> &variables
					);

			void print(std::ostream &o) const;
	};


}

#endif // _NTS_HPP_
