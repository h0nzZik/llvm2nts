#include <stdexcept>
#include "sugar.hpp"

//using namespace sugar::op;
using namespace nts;
using std::unique_ptr;
using std::domain_error;

#if 0
std::unique_ptr < nts::Relation > sugar::op::eq (
		std::unique_ptr < nts::Term > & t1,
		std::unique_ptr < nts::Term > & t2 )
{
	return std::make_unique < Relation > ( RelationOp::eq, move ( t1 ), move ( t2 ) );
}
#endif

namespace sugar
{
unique_ptr < Relation > operator== (
		unique_ptr < Term > & t1,
		unique_ptr < Term > & t2)
{
	return std::make_unique < Relation > (
			RelationOp::eq,
			move ( t1 ),
			move ( t2 )
	);
}

unique_ptr < Relation > operator< (
		unique_ptr < Term > & t1,
		unique_ptr < Term > & t2)
{
	return std::make_unique < Relation > (
			RelationOp::lt,
			move ( t1 ),
			move ( t2 )
	);
}

unique_ptr < Relation > operator<= (
		unique_ptr < Term > & t1,
		unique_ptr < Term > & t2)
{
	return std::make_unique < Relation > (
			RelationOp::leq,
			move ( t1 ),
			move ( t2 )
	);
}

unique_ptr < Relation > operator> (
		unique_ptr < Term > & t1,
		unique_ptr < Term > & t2)
{
	return std::make_unique < Relation > (
			RelationOp::gt,
			move ( t1 ),
			move ( t2 )
	);
}

unique_ptr < Relation > operator>= (
		unique_ptr < Term > & t1,
		unique_ptr < Term > & t2)
{
	return std::make_unique < Relation > (
			RelationOp::geq,
			move ( t1 ),
			move ( t2 )
	);
}

unique_ptr < Relation > operator!= (
		unique_ptr < Term > & t1,
		unique_ptr < Term > & t2)
{
	return std::make_unique < Relation > (
			RelationOp::neq,
			move ( t1 ),
			move ( t2 )
	);
}

unique_ptr < Relation > operator< (
		unique_ptr < Term > & t,
		int n )
{
	if ( n != 0 )
		throw domain_error ( "Only zero is supported" );

	if ( t->type().is_bitvector() )
	{
		unsigned int w = t->type().bitwidth();
		if ( w < 1 || w > 8 * sizeof(int) )
			throw domain_error ( "Bitwidth is too large" );

		unique_ptr < Term > n = std::make_unique < IntConstant > ( 1 << ( w - 1 ) );
		
		// If it is greater than this, it is negative
		return t >= n;
	}

	if ( ( t->type() == DataType::Integer() ) || ( t->type() == DataType::Real() ) )
	{
		unique_ptr < Term > n = std::make_unique < IntConstant > ( 0 );
		return t < n;
	}

	throw domain_error ( "Negativity test not supported on this type" );

}

nts::FormulaBop & operator== ( nts::Formula & f1, nts::Formula &f2 )
{
	return * new FormulaBop ( BoolOp::Equiv,
			unique_ptr<Formula> ( &f1 ),
			unique_ptr<Formula> ( &f2 ) );
}

FormulaBop & operator&& ( nts::Formula & f1, nts::Formula & f2 )
{
	return * new FormulaBop (
			BoolOp::And,
			unique_ptr < Formula > ( &f1 ),
			unique_ptr < Formula > ( &f2 )
	);
}

unique_ptr < FormulaBop > operator&& (
		unique_ptr < Formula > && f1,
		unique_ptr < Formula > && f2  )
{
	return unique_ptr < FormulaBop > ( & (*f1.release() && *f2.release() ) );
}


unique_ptr < FormulaBop > operator== (
		unique_ptr < Formula > && f1,
		unique_ptr < Formula > && f2 )
{
	return unique_ptr < FormulaBop > ( & ( *f1.release() == *f2.release() ) );
}

unique_ptr < FormulaBop > operator== (
		unique_ptr < Formula > & f1,
		unique_ptr < Formula > & f2 )
{
	return move ( f1 ) == move ( f2 );
}


unique_ptr < FormulaBop > equally_negative (
		unique_ptr < Term > & t1,
		unique_ptr < Term > & t2 )
{
	unique_ptr < Formula > ap1 = t1 < 0;
	unique_ptr < Formula > ap2 = t2 < 0;
	return ap1 == ap2;
}

}
#if 0

unique_ptr < FormulaBop > sign_lt (
		unique_ptr < Term > & t1,
		unique_ptr < Term > & t2 )
{
	unique_ptr < Term > t1_c ( t1->clone() );
	unique_ptr < Term > t2_c ( t2->clone() );

	unique_ptr < Formula > en = equally_negative ( t1_c, t2_c );



}
#endif




