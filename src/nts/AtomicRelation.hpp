#ifndef _NTS_ATOMICRELATION_HPP_
#define _NTS_ATOMICRELATION_HPP_

#include "BinaryOpBase.hpp"
#include "AtomicProposition.hpp"
#include "AbstractArithmetic.hpp"

namespace NTS
{
	class AtomicRelation final : public AtomicProposition
	{
		public:
			enum class Relation
			{
				Eq,
				Neq,
				Le,
				Lt,
				Ge,
				Gt
			};

		private:
			Relation m_rel;
			BinaryOpBase m_bob;

			const char * rel_to_sym(Relation r);

		public:
			AtomicRelation(Relation rel, AbstArithTerm *t1, AbstArithTerm *t2);

			virtual void print(const ConcreteCtx &ctx, std::ostream &o) const;
	};
};


#endif // _NTS_ATOMICRELATION_HPP_
