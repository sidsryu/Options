#include "../extern/catch.hpp"
#include "../src/AOptions.h"

TEST_CASE("Parse command line for advanced case", "[advanced][todo][!hide]")
{
	AOptions options;

	SECTION("Print out auto generated help description")
	{}

	SECTION("Check extra arguemnt (aka. dynamic switch)")
	{
		// app.exe extra argument -f
	}

	SECTION("Use shot switch (aka. no separator key-value)")
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
