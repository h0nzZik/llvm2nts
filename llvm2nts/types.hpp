#ifndef LLVM2NTS_TYPES_HPP_
#define LLVM2NTS_TYPES_HPP_


#include <llvm/IR/Type.h>
#include <libNTS/data_types.hpp>


nts::DataType llvm_type_to_nts_type ( const llvm::Type &t );






#endif // LLVM2NTS_TYPES_HPP_
