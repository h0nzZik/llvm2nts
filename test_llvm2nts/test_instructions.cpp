#include <ostream>
#include <sstream>
#include <catch.hpp>

#include <nts/NTS.hpp>

#include <llvm2nts/FunctionMapping.hpp>

using namespace NTS;

TEST_CASE( "Trivial", "Tests nothing")
{
	BasicNts nts;
	REQUIRE(0 == 0);
}

