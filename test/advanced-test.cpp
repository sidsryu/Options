#include "../extern/catch.hpp"
#include "../src/options.h"

using namespace options;

TEST_CASE("Parse command line for advanced case", "[advanced][todo][!hide]")
{
	Options options;

	SECTION("Print out auto generated help description")
	{}

	SECTION("Check extra arguemnt (aka. dynamic sigil)")
	{
		// app.exe extra argument -f
	}

	SECTION("Use shot flag (aka. no separator)")
	{
		// -fFilename
	}

	SECTION("Use whole value without quotes")
	{
		// -f an_argument
	}

	SECTION("Use include whitespace values")
	{
		// -file first,second_double,third
	}

	SECTION("Use case sensitive keys")
	{
		// -FILE firname
	}
}
