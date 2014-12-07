#ifndef _NTS_HPP_
#define _NTS_HPP_

#include <vector>
#include "Formula.hpp"
#include "Variable.hpp"

namespace NTS
{

	class State
	{
		private:
			int m_st;
			bool m_final; // FIXME:: Not used

		public:
			State(int st, bool st_final) : m_st(st), m_final(st_final) {;}
			State & operator=(State const &) = delete;

			void print(std::ostream &o) const
			{
				o << "s_" << std::to_string(m_st);
			}
	};

	class Transition
	{
		private:
			const State * m_from;
			const State * m_to;
			const Formula * m_guard;

		public:
			Transition(const State *a, const State *b, const Formula *guard)
				: m_from(a),
				m_to(b),
				m_guard(guard)
		{;}

			void print(std::ostream &o) const
			{
				m_from->print(o);
				o << " -> ";
				m_to->print(o);
				o << "{";
				m_guard->print(o);
				o << "}";
			}
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


			const State & pr_addState(bool st_final)
			{
				const State * s = new State((int)m_states.size(), st_final);
				m_states.push_back(s);
				return *s;
			}

		public:

			BasicNts()
			{

			}

			void setRetVar(const Variable * ret_var)
			{
				m_retvar = ret_var;
			}

			const Variable * getRetVar(void) const
			{
				return m_retvar;
			}

			void addVariable(Variable * var)
			{
				m_variables.push_back(var);
			}

			void addArgument(Variable * arg)
			{
				m_arguments.push_back(arg);
			}

			const State & addState()
			{
				return pr_addState(false);
			}

			const State & addFinalState()
			{
				return pr_addState(true);
			}

			const State & lastState() const
			{
				return *m_states.back();
			}

			void addTransition(const State * from, const State * to, Formula *guard)
			{
				Transition t(from, to, guard);
				m_transitions.push_back(t);
			}

			void print(std::ostream &o) const
			{

				// TODO add types
				o << "in ( ";
				for(const auto *v : m_arguments)
				{
					v->print(o);
					o << " ";
				}
				o << ");\n";
				o << "out ( ";
				m_retvar->print(o);
				o << " );\n";
				o << "-- Variables:\n";
				for (const auto *v : m_variables)
				{
					v->print(o);
					o << "\n";
				}
				o << "Transitions:\n";
				for(const auto & t : m_transitions)
				{
					t.print(o);
					o << "\n";
				}
			}
	};


}

#endif // _NTS_HPP_
