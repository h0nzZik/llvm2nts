#ifndef _NTS_BOOLEANCONNECTOR_HPP_
#define _NTS_BOOLEANCONNECTOR_HPP_

#include "Printable.hpp"

namespace NTS
{
	enum class BoolOp
	{
		And,
		Or,
		Impl,
		Equiv
	};

	Printable::Prio BoolOp_toPrio(BoolOp op);
	const char * BoolOp_toString(BoolOp op);	
};

#endif // _NTS_BOOLEANCONNECTOR_HPP_
