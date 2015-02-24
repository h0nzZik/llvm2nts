#include <ostream>
#include <sstream>
#include <catch.hpp>

#include <nts/Printable.hpp>
#include <nts/Formula.hpp>

using std::stringstream;
using namespace NTS;

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

class SomeFormula : public Formula
{
	private:
		const char *m_s;

	public:
		SomeFormula(Printable::Prio p, const char *s) : Formula(p), m_s(s) {;}

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

TEST_CASE( "Leaf", "No brackets")
{
	stringstream ss;
	SomePrintable p(Printable::Prio::PR_Leaf, "text");
	ConcreteCtx c{};

	p.wrapped_print(c, ss, Printable::Prio::PR_Equiv);
	REQUIRE(ss.str() == "text");
}

TEST_CASE( "Not", "not (a && b)")
{
	stringstream ss;
	ConcreteCtx c{};
	SomeFormula f_a(Printable::Prio::PR_Leaf, "a");
	SomeFormula f_b(Printable::Prio::PR_Leaf, "b");
	FormulaBop f_c(BoolOp::And, &f_a, &f_b);
	FormulaNot f_d(&f_c);

	f_d.print(c, ss);
	REQUIRE(ss.str() == "not (a && b)");
}

TEST_CASE("NotLeaf", "not leaf")
{
	stringstream ss;
	ConcreteCtx c{};
	SomeFormula f_a(Printable::Prio::PR_Leaf, "leaf");
	FormulaNot f_not(&f_a);

	f_not.print(c, ss);
	REQUIRE(ss.str() == "not leaf");
}
