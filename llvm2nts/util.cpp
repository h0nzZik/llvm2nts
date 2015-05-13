#include <string>
#include <llvm/Support/Casting.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Constants.h>
#include "util.hpp"

using namespace nts;
using std::logic_error;
using std::string;
using std::to_string;

State *  new_state ( unsigned int bb_id, unsigned int inst_id )
{
	return new State (
			string    ( "st_"   ) +
			to_string ( bb_id   ) +
			string    ( "_"     ) +
			to_string ( inst_id )
	);
}

Constant * new_constant ( const llvm::Constant & c )
{
	if ( llvm::isa < llvm::ConstantInt > ( c ) )
	{
		auto & ci = llvm::cast < llvm::ConstantInt > ( c );
		std::string s;
		llvm::raw_string_ostream os ( s );
		os << ci.getValue();
		return new nts::UserConstant (
				DataType ( ScalarType::Integral() ),
				os.str() );
	}

	return 	nullptr;
}

BoolConstant * new_bool_constant ( const llvm::Constant & c )
{
	if ( !c.getType()->isIntegerTy ( 1 ) )
		throw logic_error ( "Not an i1 type" );

	if ( !llvm::isa < llvm::ConstantInt > ( c ) )
		throw logic_error ( "Not a ConstantInt" );

	auto & ci = llvm::cast < llvm::ConstantInt > ( c );
	bool b = ci.getValue().getBoolValue();
	return new BoolConstant ( b );
}
