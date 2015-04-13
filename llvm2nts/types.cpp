#include <llvm/Support/Casting.h>
#include <llvm/IR/DerivedTypes.h>
#include "types.hpp"

nts::DataType llvm_type_to_nts_type ( const llvm::Type & t )
{
	if ( t.isIntegerTy() )
	{
		auto & it = llvm::cast<llvm::IntegerType> ( t );
		return nts::DataType::BitVector ( it.getBitWidth() );
	} else if ( t.isPointerTy() ) {
		return nts::DataType::BitVector ( 64 ); // 64 bit pointers
	} else {
		throw std::logic_error ( "Only integer types are supported" );
	}
}

