#include <ostream>
#include <string>
#include <sstream>
#include <catch.hpp>
#include <utility>

#include <nts/NTS.hpp>
#include <nts/Call.hpp>

using namespace NTS;

TEST_CASE ( "BasicNts_Move", "Test BasicNts move semantics" )
{
	std::string s("Named");
	BasicNts nts_1 ( s );

	BasicNts nts_2 ( std::move ( nts_1 ) );
	REQUIRE ( nts_2.get_name() == s );
}

TEST_CASE ( "Nts_call_formula", "Test formatting of NTS call" )
{
	BasicNts nts_1 ( "function" );

	Variable v1 ( "var1" ), v2 ( "var2" );
	Constant a ( "15" ), b ( "65539" );
	Call c ( &nts_1, {&v1, &v2}, {&a, &b, &v1});

	std::stringstream o;
	c.print ( o );

	std::string expected = "( var1', var2' ) = function ( 15, 65539, var1 )";
	REQUIRE ( expected == o.str() );
}
