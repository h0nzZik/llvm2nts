#include "Formula.hpp"

namespace NTS
{
	FormulaBop::FormulaBop(BoolOp op, Formula *f1, Formula *f2)
		: Formula(BoolOp_toPrio(op)), m_bob(f1, f2, BoolOp_toString(op))
	{
		;
	}

	void FormulaBop::print(const ConcreteCtx &ctx, std::ostream &o) const
	{
		m_bob.print(ctx, o, Printable::m_prio);
	}

	FormulaNot::FormulaNot(Formula *f)
		: Formula(PR_Not), m_f(f)
	{
		;
	}

	void FormulaNot::print(const ConcreteCtx &ctx, std::ostream &o) const
	{
		o << "not ";
		m_f->wrapped_print(ctx, o, PR_Not);
	}


	ConcreteFormula::ConcreteFormula(
					const Formula &f,
					const std::initializer_list<const IPrint *>  &variables
					)
		: m_f(f), m_ctx(variables)
	{
		;
	}

	ConcreteFormula::ConcreteFormula(const ConcreteFormula &cf)
		: m_f(cf.m_f), m_ctx(cf.m_ctx)
	{
		;
	}


	ConcreteFormula::ConcreteFormula(const ConcreteFormula &&cf)
		: m_f(cf.m_f), m_ctx(cf.m_ctx)
	{
		;
	}

	void ConcreteFormula::print(std::ostream &o) const
	{
		m_f.print(m_ctx, o);
	}


};
