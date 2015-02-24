#ifndef _NTS_FORMULA_HPP_
#define _NTS_FORMULA_HPP_

#include "Printable.hpp"
#include "BinaryOpBase.hpp"
#include "BoolOp.hpp"

namespace NTS
{
	class Formula : public Printable
	{
		public:
			Formula(Prio pr) : Printable(pr) {;}

			// Printable
			virtual void print(const ConcreteCtx &ctx, std::ostream &o) const = 0;
	};

	class FormulaBop final : public Formula
	{
		private:
			BinaryOpBase m_bob;

		public:
			FormulaBop(BoolOp op, Formula *f1, Formula *f2);

			virtual void print(const ConcreteCtx &ctx, std::ostream &o) const;

	};

	class FormulaNot final : public Formula
	{
		private:
			Formula * m_f;

		public:
			FormulaNot(Formula *f);

			virtual void print(const ConcreteCtx &ctx, std::ostream &o) const;
	};


	class ConcreteFormula
	{
		private:
			const Formula & m_f;
			ConcreteCtx m_ctx;

		public:
			ConcreteFormula(
					const Formula &f,
					const std::initializer_list<const IPrint *>  &variables
					);

			ConcreteFormula(const ConcreteFormula &cf);
			ConcreteFormula(const ConcreteFormula &&cf);

			~ConcreteFormula() {;}

			void print(std::ostream &o) const;
	};
};

#endif // _NTS_FORMULA_HPP_
