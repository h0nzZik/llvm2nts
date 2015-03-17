#include <utility>
#include "util.hpp"
#include "NTS.hpp"

namespace NTS
{
	CommonState::CommonState ( int bb_id, int inst_id ) :
		m_bb   ( bb_id ),
		m_inst ( inst_id )
	{
		;
	}

	void CommonState::print ( std::ostream &o ) const
	{
		o << "s_" << std::to_string(m_bb) << "_" << std::to_string( m_inst );
	}

	void FinalState::print ( std::ostream & o ) const
	{
		o << "st_final";
	}


	Transition::Transition ( const State *a, const State *b, const TransitionRule *rule ) :
			m_from ( a    ),
			m_to   ( b    ),
			m_rule ( rule )
	{
		;
	}

	Transition::Transition(const Transition &other) :
		m_from  ( other.m_from ),
		m_to    ( other.m_to   ),
		m_rule  ( other.m_rule )
	{
		;
	}

	Transition::Transition ( Transition && old) :
		m_from  ( old.m_from ),
		m_to    ( old.m_to   ),
		m_rule  ( old.m_rule )
	{
		;
	}

	void Transition::print(std::ostream &o) const
	{
		m_from->print ( o );
		o << " -> ";
		m_to->print ( o );
		o << " {";
		m_rule->print ( o );
		o << "}";
	}

	BasicNts::BasicNts ( const std::string & name ) :
		m_name ( name )
	{
		m_final_st = new FinalState ();
	}

	BasicNts::BasicNts ( BasicNts && old ) :
		m_name        ( std::move ( old.m_name        ) ),
		m_variables   ( std::move ( old.m_variables   ) ),
		m_constants   ( std::move ( old.m_constants   ) ),
		m_arguments   ( std::move ( old.m_arguments   ) ),
		m_states      ( std::move ( old.m_states      ) ),
		m_cformulas   ( std::move ( old.m_cformulas   ) ),
		m_calls       ( std::move ( old.m_calls       ) ),
		m_transitions ( std::move ( old.m_transitions ) ),
		m_final_st    ( std::move ( old.m_final_st    ) ),
		m_retvars     ( std::move ( old.m_retvars     ) ),
		m_var_lbb     ( std::move ( old.m_var_lbb     ) )
	{
		old.m_final_st = nullptr;
		old.m_var_lbb = nullptr;
	}

	BasicNts::~BasicNts()
	{
		delete m_final_st;
		m_final_st = nullptr;
		m_var_lbb = nullptr;

		for ( CommonState * st : m_states )
		{
			delete st;
		}
		m_states.clear();

		for ( Variable * v : m_variables )
		{
			delete v;
		}
		m_variables.clear();

		for ( Constant *c : m_constants )
		{
			delete c;
		}
		m_constants.clear();

		for ( Variable *v : m_arguments )
		{
			delete v;
		}
		m_arguments.clear();

		for ( ConcreteFormula *cf : m_cformulas )
		{
			delete cf;
		}
		m_cformulas.clear();

		for ( Call * c : m_calls )
		{
			delete c;
		}
		m_calls.clear();
	}

	const std::string & BasicNts::get_name () const
	{
		return m_name;
	}

	const Variable * BasicNts::get_return_variable ( size_t id ) const
	{
		return m_retvars.at(id);
	}

	void BasicNts::set_lbb_var ( const Variable *var )
	{
		m_var_lbb = var;
	}

	const Variable * BasicNts::get_lbb_var() const
	{
		return m_var_lbb;
	}

	const CommonState * BasicNts::addState( int bb_id, int inst_id )
	{
		CommonState * st = new CommonState ( bb_id, inst_id );
		m_states.push_back ( st );
		return st;
	}

	const CommonState * BasicNts::lastState() const
	{
		return m_states.back();
	}

	const FinalState * BasicNts::final_state () const
	{
		return m_final_st;
	}

	const Variable * BasicNts::add_variable ( const std::string & name )
	{
		Variable *v = new Variable ( name );
		m_variables.push_back ( v );
		return v;
	}

	size_t BasicNts::add_return_variable ( const Variable * var )
	{
		m_retvars.push_back ( var );
		return m_retvars.size();
	}

	const Constant * BasicNts::add_constant ( int value )
	{
		return add_constant ( std::to_string ( value ) );
	}

	const Constant * BasicNts::add_constant ( const std::string & value )
	{
		Constant *c = new Constant ( value );
		m_constants.push_back ( c );
		return c;
	}

	const Variable * BasicNts::add_argument ( const std::string & name )
	{
		Variable * arg = new Variable ( name );
		m_arguments.push_back ( arg );
		return arg;
	}

	const TransitionRule * BasicNts::add_transition_rule ( const ConcreteFormula & cf )
	{
		ConcreteFormula *f = new ConcreteFormula ( cf );
		m_cformulas.push_back ( f );
		return f;
	}

	const TransitionRule * BasicNts::add_transition_rule ( const Call & call )
	{
		Call *c = new Call ( call );
		m_calls.push_back ( c );
		return c;
	}

	void BasicNts::add_transition(
			const State          * from,
			const State          * to,
			const TransitionRule * rule )
	{
		m_transitions.emplace_back(from, to, rule);
	}

	void BasicNts::print(std::ostream &o) const
	{
		o << m_name << " {\n";
		if ( !m_variables.empty() )
		{
			o << "\t// Variables\n\t";
			to_csv ( o, m_variables );
			o << " : int;\n";
		}

		o << "\tin ( ";
		to_csv ( o, m_arguments );
		o << " );\n";

		if ( !m_retvars.empty() )
		{
			o << "\tout ( ";
			to_csv ( o, m_retvars );
			o << " );\n";
		}

		if ( !m_states.empty() )
		{
			o << "\tinitial ";
			m_states[0]->print(o);
			o << ";\n";
		}

		if ( m_final_st != nullptr)
		{
			o << "\tfinal ";
			m_final_st->print ( o );
			o << ";\n";
		}

		o << "\t// Transitions:\n";
		for ( const Transition & t : m_transitions )
		{
			o << "\t";
			t.print ( o );
			o << "\n";
		}

		o << "}\n";
	}


};
