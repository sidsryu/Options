#include "../extern/catch.hpp"
#include "../src/AOptions.h"
#include <regex>

TEST_CASE("Parse command line for easy case.", "[basic]")
{
	AOptions options;

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
		int		count;

		options.Add(L"file", L"", filename);
		options.Add(L"count", L"", count);

		REQUIRE(options.Parse(L"-file filename -count 10"));

		CHECK(L"filename" == filename);
		CHECK(count == 10);
	}

	SECTION("Check multiple values")
	{
		std::wstring filename;
		std::vector<std::wstring> fileList;

		options.Add(L"file", L"", filename);
		options.Add(L"file", L"", fileList);

		REQUIRE(options.Parse(L"-file first second third"));

		CHECK(L"first second third" == filename);

		REQUIRE(3 == fileList.size());
		CHECK(L"first" == fileList[0]);
		CHECK(L"second" == fileList[1]);
		CHECK(L"third" == fileList[2]);
	}

	SECTION("Use custom switch")
	{
		bool help;
		
		options.Add(L"help", L"", help);				
		options.SetSwitch(L"/");

		REQUIRE(options.Parse(L"/help"));

		CHECK(help);
	}

	SECTION("Escape switch that regex literal")
	{
		bool help;

		options.Add(L"help", L"", help);				
		options.SetSwitch(L"*");

		REQUIRE(options.Parse(L"*help"));

		CHECK(help);
	}

	SECTION("Use custom key-value separator")
	{
		std::wstring filename;

		options.Add(L"file", L"", filename);
		options.SetSeparator(L"=");

		CHECK(options.Parse(L"-file=filename"));

		CHECK(L"filename" == filename);
	}

	SECTION("Use custom value serial separator")
	{
		std::wstring filename;
		std::vector<std::wstring> fileList;

		options.Add(L"file", L"", fileList);
		options.Add(L"file", L"", filename);		
		options.SetSplitter(L",");

		REQUIRE(options.Parse(L"-file first, second double, third"));

		CHECK(L"first, second double, third" == filename);

		REQUIRE(3 == fileList.size());
		CHECK(L"first" == fileList[0]);
		CHECK(L"second double" == fileList[1]);
		CHECK(L"third" == fileList[2]);
	}

	SECTION("Use multiple character key-value separator")
	{
		std::wstring filename;
		std::vector<std::wstring> fileList;

		options.Add(L"file", L"", fileList);
		options.Add(L"file", L"", filename);		
		options.SetSwitch(L"-[");
		options.SetSeparator(L"]=");
		options.SetSplitter(L".,");

		REQUIRE(options.Parse(L"-[file]=first.,second double.,third"));

		CHECK(L"first.,second double.,third" == filename);

		REQUIRE(3 == fileList.size());
		CHECK(L"first" == fileList[0]);
		CHECK(L"second double" == fileList[1]);
		CHECK(L"third" == fileList[2]);
	}

	SECTION("Use no-switch options")
	{
		std::vector<std::wstring> standaloneList;
		std::vector<std::wstring> launchList;
		
		options.Add(L"standalone", L"", standaloneList);
		options.Add(L"launch", L"", launchList);
		options.SetSwitch(L"");

		REQUIRE(options.Parse(L"launch test localhost 109 1 1 1"));

		CHECK(standaloneList.empty());

		REQUIRE(6 == launchList.size());
		CHECK(L"test" == launchList[0]);
		CHECK(L"localhost" == launchList[1]);
		CHECK(L"109" == launchList[2]);
		CHECK(L"1" == launchList[3]);
		CHECK(L"1" == launchList[4]);
		CHECK(L"1" == launchList[5]);
	}

	SECTION("Escape double quotes value")
	{
		std::wstring filename;
		std::vector<std::wstring> fileList;

		options.Add(L"file", L"", fileList);
		options.Add(L"file", L"", filename);
		options.SetSplitter(L",");

		REQUIRE(options.Parse(L"-file \"first, second double, 3-rd\""));

		CHECK(L"first, second double, 3-rd" == filename);

		REQUIRE(3 == fileList.size());
		CHECK(L"first" == fileList[0]);
		CHECK(L"second double" == fileList[1]);
		CHECK(L"3-rd" == fileList[2]);			// 스위치 포함 문자열
	}
}

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
