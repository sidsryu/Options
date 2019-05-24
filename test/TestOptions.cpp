#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include "../src/AOptions.h"
#include <regex>

SUITE(Options)
{
	struct TestOptions
	{
		AOptions m_options;
	};

	TEST_FIXTURE(TestOptions, Options)
	{				
		bool help;
		bool debug;S

		m_options.Add(L"help", L"", help);
		m_options.Add(L"debug", L"", debug);

		CHECK(m_options.Parse(L"-help"));

		CHECK(help);
		CHECK(!debug);
	}

	TEST_FIXTURE(TestOptions, ArgumentOptions)
	{
		std::wstring filename;
		int		count;

		m_options.Add(L"file", L"", filename);
		m_options.Add(L"count", L"", count);

		CHECK(m_options.Parse(L"-file filename -count 10"));

		CHECK(L"filename" == filename);
		CHECK_EQUAL(10, count);
	}

	TEST_FIXTURE(TestOptions, Arguments)
	{
		std::wstring filename;
		std::vector<std::wstring> fileList;

		m_options.Add(L"file", L"", filename);
		m_options.Add(L"file", L"", fileList);

		CHECK(m_options.Parse(L"-file first second third"));

		CHECK(L"first second third" == filename);

		ASSERT_EQUAL(3, fileList.size());
		CHECK(L"first" == fileList[0]);
		CHECK(L"second" == fileList[1]);
		CHECK(L"third" == fileList[2]);
	}

	TEST_FIXTURE(TestOptions, CustomSwitch)
	{
		bool help;
		
		m_options.Add(L"help", L"", help);				
		m_options.SetSwitch(L"/");

		CHECK(m_options.Parse(L"/help"));

		CHECK(help);
	}

	TEST_FIXTURE(TestOptions, CustomSwitchWithRegexLetrer)
	{
		bool help;

		m_options.Add(L"help", L"", help);				
		m_options.SetSwitch(L"*");

		CHECK(m_options.Parse(L"*help"));

		CHECK(help);
	}

	TEST_FIXTURE(TestOptions, CustomSeparator)
	{
		std::wstring filename;

		m_options.Add(L"file", L"", filename);
		m_options.SetSeparator(L"=");

		CHECK(m_options.Parse(L"-file=filename"));

		CHECK(L"filename" == filename);
	}

	TEST_FIXTURE(TestOptions, CustomSplitter)
	{
		std::wstring filename;
		std::vector<std::wstring> fileList;

		m_options.Add(L"file", L"", fileList);
		m_options.Add(L"file", L"", filename);		
		m_options.SetSplitter(L",");

		CHECK(m_options.Parse(L"-file first, second double, third"));

		CHECK(L"first, second double, third" == filename);

		ASSERT_EQUAL(3, fileList.size());
		CHECK(L"first" == fileList[0]);
		CHECK(L"second double" == fileList[1]);
		CHECK(L"third" == fileList[2]);
	}

	TEST_FIXTURE(TestOptions, LongSwitchWithEtc)
	{
		std::wstring filename;
		std::vector<std::wstring> fileList;

		m_options.Add(L"file", L"", fileList);
		m_options.Add(L"file", L"", filename);		
		m_options.SetSwitch(L"-[");
		m_options.SetSeparator(L"]=");
		m_options.SetSplitter(L".,");

		CHECK(m_options.Parse(L"-[file]=first.,second double.,third"));

		CHECK(L"first.,second double.,third" == filename);

		ASSERT_EQUAL(3, fileList.size());
		CHECK(L"first" == fileList[0]);
		CHECK(L"second double" == fileList[1]);
		CHECK(L"third" == fileList[2]);
	}

	TEST_FIXTURE(TestOptions, NonSwitchOption)
	{
		std::vector<std::wstring> standaloneList;
		std::vector<std::wstring> launchList;
		
		m_options.Add(L"standalone", L"", standaloneList);
		m_options.Add(L"launch", L"", launchList);
		m_options.SetSwitch(L"");

		CHECK(m_options.Parse(L"launch test localhost 109 1 1 1"));

		CHECK(standaloneList.empty());

		ASSERT_EQUAL(6, launchList.size());
		CHECK(L"test" == launchList[0]);
		CHECK(L"localhost" == launchList[1]);
		CHECK(L"109" == launchList[2]);
		CHECK(L"1" == launchList[3]);
		CHECK(L"1" == launchList[4]);
		CHECK(L"1" == launchList[5]);
	}

	TEST_FIXTURE(TestOptions, DoubleQuotesArgument)
	{
		std::wstring filename;
		std::vector<std::wstring> fileList;

		m_options.Add(L"file", L"", fileList);
		m_options.Add(L"file", L"", filename);
		m_options.SetSplitter(L",");

		CHECK(m_options.Parse(L"-file \"first, second double, 3-rd\""));		

		CHECK(L"first, second double, 3-rd" == filename);

		ASSERT_EQUAL(3, fileList.size());
		CHECK(L"first" == fileList[0]);
		CHECK(L"second double" == fileList[1]);
		CHECK(L"3-rd" == fileList[2]);			// 스위치 포함 문자열
	}


	// Reserve features
	//TEST_FIXTURE(TestOptions, OutputDescription)		{}
	//TEST_FIXTURE(TestOptions, ExtraArgument)			{}		// app.exe extra argument -f
	//TEST_FIXTURE(TestOptions, ShorSwitch)				{}		// -fFilename
	//TEST_FIXTURE(TestOptions, SingleArgument)			{}		// -f an_argument	
	//TEST_FIXTURE(TestOptions, SetWhitespace)			{}		// -file first,second_double,third
	//TEST_FIXTURE(TestOptions, CaseSensitive)			{}		// -FILE firname
}
