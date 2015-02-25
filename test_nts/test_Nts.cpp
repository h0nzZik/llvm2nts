#include <ostream>
#include <sstream>
#include <catch.hpp>
#include <utility>

#include <nts/NTS.hpp>

using namespace NTS;

TEST_CASE ( "BasicNts_Move", "Test BasicNts move semantics" )
{
	BasicNts nts_1 ("Named");

	BasicNts nts_2 ( std::move ( nts_1 ) );

}
