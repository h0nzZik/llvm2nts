#include <ostream>
#include <sstream>
#include <catch.hpp>

#include <Printable.hpp>

using std::stringstream;
using NTS::Printable;
using NTS::ConcreteCtx;

class SomePrintable : public Printable
{
	private:
		const char *m_s;

	public:
		SomePrintable(Printable::Prio p, const char *s) : Printable(p), m_s(s) {;}
		~SomePrintable() {;}
		
		virtual void print(const ConcreteCtx &ctx, std::ostream &o) const
		{
			(void)ctx;
			o << m_s;
		}
};

TEST_CASE( "AlwaysBrackets", "Tests whether Printable prints brackets")
{
	stringstream ss;
	SomePrintable p(Printable::Prio::PR_AlwaysBrackets, "test");
	ConcreteCtx c{};

	p.wrapped_print(c, ss, Printable::Prio::PR_Equiv);
	REQUIRE(ss.str() == "(test)");
}


