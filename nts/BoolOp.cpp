#include <stdexcept>
#include "BoolOp.hpp"


namespace NTS
{
	Printable::Prio BoolOp_toPrio(BoolOp op)
	{
		switch(op)
		{
			case BoolOp::And:
				return Printable::Prio::PR_And;
			case BoolOp::Or:
				return Printable::Prio::PR_Or;
			case BoolOp::Impl:
				return Printable::Prio::PR_Imply;
			case BoolOp::Equiv:
				return Printable::Prio::PR_Equiv;
			default:
				throw std::domain_error("Invalid connector");
		}
	}

	const char * BoolOp_toString(BoolOp op)
	{
		switch(op)
		{
			case BoolOp::And:
				return "&&";
			case BoolOp::Or:
				return "||";
			case BoolOp::Impl:
				return "->";
			case BoolOp::Equiv:
				return "<->";
			default:
				throw std::domain_error("Invalid connector");
		}
	}
};
