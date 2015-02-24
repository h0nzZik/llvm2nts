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


	Transition::Transition ( const State *a, const State *b, const ConcreteFormula &guard )
		: m_from(a), m_to(b), m_guard(guard)
	{
		;
	}

	Transition::Transition(const Transition &other) :
		m_from  ( other.m_from ),
		m_to    ( other.m_to ),
		m_guard ( other.m_guard )
	{
		;
	}

	Transition::Transition ( Transition && old) :
		m_from  ( old.m_from ),
		m_to    ( old.m_to ),
		m_guard ( std::move ( old.m_guard ) )
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
		m_final_st = new FinalState ();
		m_retvar   = new Variable ( "ret_var" );
	}

	BasicNts::BasicNts ( BasicNts && old ) :
		m_name        ( std::move ( old.m_name        ) ),
		m_variables   ( std::move ( old.m_variables   ) ),
		m_constants   ( std::move ( old.m_constants   ) ),
		m_arguments   ( std::move ( old.m_arguments   ) ),
		m_states      ( std::move ( old.m_states      ) ),
		m_transitions ( std::move ( old.m_transitions ) ),
		m_final_st    ( std::move ( old.m_final_st    ) ),
		m_retvar      ( std::move ( old.m_retvar      ) )
	{
		old.m_name = "( moved out )";
		old.m_final_st = nullptr;
		old.m_retvar = nullptr;
	}

	BasicNts::~BasicNts()
	{
		delete m_final_st;
		m_final_st = nullptr;

		delete m_retvar;
		m_retvar = nullptr;

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
	}

	const Variable * BasicNts::getRetVar(void) const
	{
		return m_retvar;
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
		Variable *v = new Variable ( "var_" + name );
		m_variables.push_back ( v );
		return v;
	}

	const Constant * BasicNts::add_constant ( const std::string & value )
	{
		Constant *c = new Constant ( value );
		m_constants.push_back ( c );
		return c;
	}

	const Variable * BasicNts::add_argument ( const std::string & name )
	{
		Variable * arg = new Variable ( "arg_" + name );
		m_arguments.push_back ( arg );
		return arg;
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
		if ( !m_variables.empty() )
		{
			o << " // Variables\n";
			to_csv ( o, m_variables );
			o << " : int\n";
		}

		// TODO add types
		o << "in ( ";
		to_csv ( o, m_arguments );
		o << " );\n";

		o << "out ( " << *m_retvar << " );\n";


		o << "Transitions:\n";
		for ( const Transition & t : m_transitions )
		{
			t.print ( o );
			o << "\n";
		}
	}


};
