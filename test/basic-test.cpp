#include "../extern/catch.hpp"
#include "../src/options.h"
#include <regex>

TEST_CASE("Parse command line for easy case.", "[basic]")
{
	Options options;

	SECTION("Check exist keys")
	{
		bool help;
		bool debug;

		options.Add(L"help", L"", help);
		options.Add(L"debug", L"", debug);

		REQUIRE(options.Parse(L"-help"));

		CHECK(help);
		CHECK(!debug);
	}

	SECTION("Check values")
	{
		std::wstring filename;
		int count;

		options.Add(L"file", L"", filename);
		options.Add(L"count", L"", count);

		REQUIRE(options.Parse(L"-file filename -count 10"));

		CHECK(filename == L"filename");
		CHECK(count == 10);
	}

	SECTION("Check multiple values")
	{
		std::vector<std::wstring> fileList;
		options.Add(L"file", L"", fileList);

		REQUIRE(options.Parse(L"-file first second third"));

		REQUIRE(3 == fileList.size());
		CHECK(fileList[0] == L"first");
		CHECK(fileList[1] == L"second");
		CHECK(fileList[2] == L"third");
	}

	SECTION("Check multiple values with a single binded variable")
	{
		std::wstring filenames;
		options.Add(L"file", L"", filenames);

		REQUIRE(options.Parse(L"-file first second third"));

		CHECK(filenames == L"first second third");
	}

	SECTION("Use custom sigil")
	{
		bool help;

		options.Add(L"help", L"", help);
		options.SetSigil(L"/");

		REQUIRE(options.Parse(L"/help"));

		CHECK(help);
	}

	SECTION("Escape sigil that regex literal")
	{
		bool help;

		options.Add(L"help", L"", help);
		options.SetSigil(L"*");

		REQUIRE(options.Parse(L"*help"));

		CHECK(help);
	}

	SECTION("Use custom separator")
	{
		std::wstring filename;

		options.Add(L"file", L"", filename);
		options.SetSeparator(L"=");

		REQUIRE(options.Parse(L"-file=filename"));

		CHECK(filename == L"filename");
	}

	SECTION("Use custom serialer")
	{
		std::vector<std::wstring> fileList;

		options.Add(L"file", L"", fileList);
		options.SetSerialer(L",");

		REQUIRE(options.Parse(L"-file first, second double, third"));

		REQUIRE(3 == fileList.size());
		CHECK(fileList[0] == L"first");
		CHECK(fileList[1] == L"second double");
		CHECK(fileList[2] == L"third");
	}

	SECTION("Use custom serialer with a single binded variable")
	{
		std::wstring filenames;

		options.Add(L"file", L"", filenames);
		options.SetSerialer(L",");

		REQUIRE(options.Parse(L"-file first, second double, third"));

		CHECK(filenames == L"first, second double, third");
	}

	SECTION("Use multiple character sigil")
	{
		bool help;

		options.Add(L"help", L"", help);
		options.SetSigil(L"--");

		REQUIRE(options.Parse(L"--help"));

		CHECK(help);
	}

	SECTION("Use multiple character separator")
	{
		std::wstring filename;

		options.Add(L"file", L"", filename);
		options.SetSeparator(L"=>");

		REQUIRE(options.Parse(L"-file=>filename"));

		CHECK(filename == L"filename");
	}

	SECTION("Use multiple character serialer")
	{
		std::vector<std::wstring> fileList;

		options.Add(L"file", L"", fileList);
		options.SetSerialer(L";;");

		REQUIRE(options.Parse(L"-file first;;second double;;third"));

		REQUIRE(3 == fileList.size());
		CHECK(fileList[0] == L"first");
		CHECK(fileList[1] == L"second double");
		CHECK(fileList[2] == L"third");
	}

	SECTION("Use multiple character serialer with a single binded variable")
	{
		std::wstring filenames;

		options.Add(L"file", L"", filenames);
		options.SetSerialer(L";;");

		REQUIRE(options.Parse(L"-file first;;second double;;third"));

		CHECK(filenames == L"first;;second double;;third");
	}

	SECTION("Use multiple character all-in-one")
	{
		std::vector<std::wstring> fileList;

		options.Add(L"file", L"", fileList);
		options.SetSigil(L"-[");
		options.SetSeparator(L"]=");
		options.SetSerialer(L";;");

		REQUIRE(options.Parse(L"-[file]=first;;second double;;third"));

		REQUIRE(3 == fileList.size());
		CHECK(fileList[0] == L"first");
		CHECK(fileList[1] == L"second double");
		CHECK(fileList[2] == L"third");
	}

	SECTION("Use multiple character all-in-one with a single binded variable")
	{
		std::wstring filenames;

		options.Add(L"file", L"", filenames);
		options.SetSigil(L"-[");
		options.SetSeparator(L"]=");
		options.SetSerialer(L";;");

		REQUIRE(options.Parse(L"-[file]=first;;second double;;third"));

		CHECK(filenames == L"first;;second double;;third");
	}

	SECTION("Use no-sigil options")
	{
		std::vector<std::wstring> launchList;

		options.Add(L"launch", L"", launchList);
		options.SetSigil(L"");

		REQUIRE(options.Parse(L"launch test localhost 109 1 1 1"));

		REQUIRE(6 == launchList.size());
		CHECK(launchList[0] == L"test");
		CHECK(launchList[1] == L"localhost");
		CHECK(launchList[2] == L"109");
		CHECK(launchList[3] == L"1");
		CHECK(launchList[4] == L"1");
		CHECK(launchList[5] == L"1");
	}

	SECTION("Use no-sigil options not matching")
	{
		std::vector<std::wstring> noexitList;

		options.Add(L"noexit", L"", noexitList);
		options.SetSigil(L"");

		REQUIRE(options.Parse(L"launch test localhost 109 1 1 1"));

		CHECK(noexitList.empty());
	}

	SECTION("Use no-sigil options first argument not matching")
	{
		std::vector<std::wstring> localhostList;

		options.Add(L"localhost", L"", localhostList);
		options.SetSigil(L"");

		REQUIRE(options.Parse(L"launch test localhost 109 1 1 1"));

		CHECK(localhostList.empty());
	}

	SECTION("Escape double quotes value")
	{
		std::vector<std::wstring> fileList;

		options.Add(L"file", L"", fileList);
		options.SetSerialer(L",");

		REQUIRE(options.Parse(L"-file \"first, second double, 3-rd\""));

		REQUIRE(3 == fileList.size());
		CHECK(fileList[0] == L"first");
		CHECK(fileList[1] == L"second double");
		CHECK(fileList[2] == L"3-rd"); // 스위치 포함 문자열
	}

	SECTION("Escape double quotes value with a single binded variable")
	{
		std::wstring filenames;

		options.Add(L"file", L"", filenames);
		options.SetSerialer(L",");

		REQUIRE(options.Parse(L"-file \"first, second double, 3-rd\""));

		CHECK(filenames == L"first, second double, 3-rd");
	}
}
