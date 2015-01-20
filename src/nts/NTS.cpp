#include "NTS.hpp"

namespace NTS
{

	State::State(int st, bool st_final)
		: m_st(st), m_final(st_final)
	{
		;
	}

	void State::print(std::ostream &o) const
	{
		o << "s_" << std::to_string(m_st);
	}


	Transition::Transition(const State *a, const State *b, const ConcreteFormula &guard)
		: m_from(a), m_to(b), m_guard(guard)
	{
		;
	}

	Transition::Transition(const Transition &other)
		: m_from(other.m_from), m_to(other.m_to), m_guard(other.m_guard)
	{
		;
	}

	void Transition::print(std::ostream &o) const
	{
		m_from->print(o);
		o << " -> ";
		m_to->print(o);
		o << " {";
		m_guard.print(o);
		o << "}";
	}

	BasicNts::BasicNts()
	{
		;
	}

	BasicNts::~BasicNts()
	{
		for(const State * s : m_states)
		{
			delete s;
		}
	}

	const State & BasicNts::pr_addState(bool st_final)
	{
		const State * s = new State((int)m_states.size(), st_final);
		m_states.push_back(s);
		return *s;
	}

	void BasicNts::setRetVar(const Variable * ret_var)
	{
		m_retvar = ret_var;
	}

	const Variable * BasicNts::getRetVar(void) const
	{
		return m_retvar;
	}

	void BasicNts::addVariable(Variable * var)
	{
		m_variables.push_back(var);
	}

	void BasicNts::addArgument(Variable * arg)
	{
		m_arguments.push_back(arg);
	}

	const State & BasicNts::addState()
	{
		return pr_addState(false);
	}

	const State & BasicNts::addFinalState()
	{
		return pr_addState(true);
	}

	const State & BasicNts::lastState() const
	{
		return *m_states.back();
	}


	void BasicNts::addTransition(
			const State * from,
			const State * to,
			const ConcreteFormula &guard)	
	{
		m_transitions.emplace_back(from, to, guard);
	}

	void BasicNts::print(std::ostream &o) const
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
