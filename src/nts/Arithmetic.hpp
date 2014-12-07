#ifndef _NTS_ARITHMETIC_HPP_
#define _NTS_ARITHMETIC_HPP_


#include "Printable.hpp"
#include "BinaryOpBase.hpp"
#include "VariableIdentifier.hpp"

namespace NTS
{
	class ArithmeticTerm : public Printable
	{
		public:
			ArithmeticTerm(enum Prio pr) : Printable(pr) {;}

			// from Printable
			virtual void print(std::ostream &o) const = 0;
	};

	class ArithmeticTermRelation final : public ArithmeticTerm
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
			ArithmeticTermRelation(Connector ac, ArithmeticTerm *t1, ArithmeticTerm *t2);

			// ArithmeticTerm -> Printable
			virtual void print(std::ostream &o) const
			{
				m_bob.print(o, m_prio);
			}
	};

	class ArithmeticLiteral : public ArithmeticTerm
	{
		public:
			ArithmeticLiteral() : ArithmeticTerm(PR_Leaf) {;}

			// from ArithmeticTerm -> Printable
			virtual void print(std::ostream &o) const = 0;
	};

	class ArithmeticVariableIdentifier final : public ArithmeticLiteral
	{
		private:
			VariableIdentifier m_id;

		public:
			ArithmeticVariableIdentifier(const Variable *var, bool primed)
				: m_id(var,primed) {;}

			// from ArithmeticLiteral -> ArithmeticTerm -> Printable
			virtual void print(std::ostream &o) const
			{
				m_id.print(o);
			}
	};

	class PositiveNumeral final : public ArithmeticLiteral
	{
		private:
			std::string m_s;
		public:
			PositiveNumeral() {;}
			std::string & getStr() { return m_s; }

			// from ArithmeticLiteral -> ArithmeticTerm -> Printable
			virtual void print(std::ostream &o) const
			{
				o << m_s;
			}
	};

	class ThreadId final : public ArithmeticLiteral
	{
		public:
			ThreadId() {;}

			// from ArithmeticLiteral -> ArithmeticTerm -> Printable
			virtual void print(std::ostream &o) const
			{
				o << "tid";
			}
	};
};



#endif //_NTS_ARITHMETIC_HPP_
