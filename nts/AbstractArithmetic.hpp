#ifndef _NTS_ABSTRACT_ARITHMETIC_HPP_
#define _NTS_ABSTRACT_ARITHMETIC_HPP_


#include "Printable.hpp"
#include "BinaryOpBase.hpp"
#include "VariableIdentifier.hpp"
#include "ConcreteCtx.hpp"

namespace NTS
{
	class AbstArithTerm : public Printable
	{
		public:
			AbstArithTerm(enum Prio pr) : Printable(pr) {;}

			// from Printable
			virtual void print(const ConcreteCtx &ctx, std::ostream &o) const = 0;
	};

	class AbstArithTermRelation final : public AbstArithTerm
	{
		public:
			enum class Connector
			{
				Mul,
				Add,
				//Sub,
				Mod,
				Div
			};

		private:
			enum Connector m_ac;
			BinaryOpBase m_bob;

			static enum Printable::Prio ac_to_prio(Connector ac);
			static const char * ac_to_sym(Connector ac);

		public:
			AbstArithTermRelation(Connector ac, AbstArithTerm *t1, AbstArithTerm *t2);

			// AbstArithTerm -> Printable
			virtual void print(const ConcreteCtx &ctx, std::ostream &o) const;

	};

	class AbstArithLiteral : public AbstArithTerm
	{
		public:
			AbstArithLiteral() : AbstArithTerm(PR_Leaf) {;}

			// from AbstArithTerm -> Printable
			virtual void print(const ConcreteCtx &ctx, std::ostream &o) const = 0;
	};

	// Variable or constant. Not in NTS language itself.
	class AbstArithValue final : public AbstArithLiteral
	{
		private:
			int m_id;
			bool m_future;

		public:
			AbstArithValue(int id, bool future);

			// from AbstArithLiteral -> AbstArithTerm -> Printable
			virtual void print(const ConcreteCtx &ctx, std::ostream &o) const;

	};

	class ThreadId final : public AbstArithLiteral
	{
		public:
			ThreadId() {;}

			// from AbstArithLiteral -> AbstArithTerm -> Printable
			virtual void print(const ConcreteCtx &ctx, std::ostream &o) const
			{
				(void)ctx;
				o << "tid";
			}
	};
};



#endif //_NTS_ABSTRACT_ARITHMETIC_HPP_
