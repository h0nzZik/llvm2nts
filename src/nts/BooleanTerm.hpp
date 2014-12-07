#ifndef _NTS_BOOLTERM_HPP_
#define _NTS_BOOLTERM_HPP_

#include "AtomicProposition.hpp"
#include "BinaryOpBase.hpp"
#include "VariableIdentifier.hpp"
#include "BoolOp.hpp"

namespace NTS
{
	class BooleanTerm : public AtomicProposition
	{
		public:
			BooleanTerm(enum Prio pr) : AtomicProposition(pr) {;}

			// from AtomicProposition -> Printable
			virtual void print(std::ostream &o) const = 0;
	};

	class BoolTermRelation : public BooleanTerm
	{
		// data types
		public:
			// private members
		private:
			BinaryOpBase m_bob;
			BoolOp m_op;

			// public interface
		public:
			BoolTermRelation(BoolOp lc, BooleanTerm *t1, BooleanTerm *t2);

			// from BooleanTerm -> AtomicProposition -> Printable
			virtual void print(std::ostream &o) const final;
	};

	class BooleanLiteral : public BooleanTerm
	{
		public:
			BooleanLiteral() : BooleanTerm(PR_Leaf) {;}

			// from BooleanTerm -> AtomicProposition -> Printable
			virtual void print(std::ostream &o) const = 0;
	};

	class BooleanConstant final : public BooleanLiteral
	{
		private:
			bool m_bool;

		public:
			BooleanConstant(bool b) : m_bool(b) {;}

			// from BooleanLiteral -> BooleanTerm -> AtomicProposition -> Printable
			virtual void print(std::ostream &o) const
			{
				if (m_bool)
					o << "true";
				else
					o << "false";
			}
	};

	class BooleanVariableIdentifier final : public BooleanLiteral
	{
		private:
			VariableIdentifier m_id;

		public:
			BooleanVariableIdentifier(const Variable *var, bool primed)
				: m_id(var,primed) {;}

			virtual void print(std::ostream &o) const
			{
				m_id.print(o);
			}
	};


	
};

#endif // _NTS_BOOLTERM_HPP_
