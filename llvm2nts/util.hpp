#ifndef LLVM2NTS_UTIL_HPP_
#define LLVM2NTS_UTIL_HPP_
#pragma once

#include <llvm/IR/Constant.h>
#include <libNTS/nts.hpp>
#include <libNTS/logic.hpp>

nts::State * new_state ( unsigned int bb_id, unsigned int inst_id );

nts::Constant * new_constant ( const llvm::Constant & c );
nts::BoolConstant * new_bool_constant ( const llvm::Constant & c );






#endif // LLVM2NTS_UTIL_HPP_
