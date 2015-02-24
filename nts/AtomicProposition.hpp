#ifndef _NTS_ATOMICPROPOSITION_HPP_
#define _NTS_ATOMICPROPOSITION_HPP_

#include "Formula.hpp"
#include "ConcreteCtx.hpp"

namespace NTS
{
	class AtomicProposition : public Formula
	{
		public:
			AtomicProposition(enum Prio pr) : Formula(pr) {;}

			// Printable
			virtual void print(const ConcreteCtx &ctx, std::ostream &o) const = 0;
	};
};

#endif // _NTS_ATOMICPROPOSITION_HPP_
