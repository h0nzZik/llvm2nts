#ifndef _NTS_BINARYOPBASE_HPP_
#define _NTS_BINARYOPBASE_HPP_

#include "Printable.hpp"

namespace NTS
{
	class BinaryOpBase
	{
		public:
			BinaryOpBase(const Printable *t1, const Printable *t2, const char *sym);
			void print(const ConcreteCtx &ctx, std::ostream &o, enum Printable::Prio prio) const;

		private:
			const Printable * const m_t1;
			const Printable * const m_t2;
			const char * const m_sym;
	};
};



#endif // _NTS_BINARYOPBASE_HPP_
