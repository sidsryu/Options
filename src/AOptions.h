#pragma once

#include "AOptionSet.h"
#include <string>
#include <list>

/*
	커맨드라인 파서

	기본형식
		app.exe -log -file filename1 filename2 -path foldername

	사용법
		bool            isLog;
		std::vector<std::wstring> fileList;
		std::wstring         foldername;


		AOptions options;		
		
		options.Add(L"log",  L"로그 출력여부",     isLog);
		options.Add(L"file", L"복사대상 파일목록", fileList);
		options.Add(L"path", L"복사목표 폴더명",   foldername);


		options.Parser(commandLine);

		if (isLog)					{ 로그 처리 }
		if (0 < fileList.size())	{ 파일 처리 }
		if (!foldername.empty())	{ 폴더 처리 }
*/

class AOptionSetBase;

class AOptions
{
public:
	AOptions();
	virtual ~AOptions();

	template<typename T>
	void Add(std::wstring option, std::wstring description, T& outValue)
	{
		m_listOptions.push_back(new AOptionSet<T>(option, description, outValue));
	}

	bool Parse(std::wstring commandLine);
	
	bool SetSwitch(std::wstring newSwitch);
	bool SetSeparator(std::wstring newSeparator);
	bool SetSplitter(std::wstring newSplitter);

private:
	bool ValidCommandLine(std::wstring commandLine);
	bool ParseOptions(std::wstring commandLine);
	
	std::wstring MakeEscape(std::wstring text);
	std::wstring MakeNotMatch(std::wstring text);
	std::wstring MakeMatchOption(void);
	std::wstring MakeMatchValue(std::wstring notMatch);

private:
	std::list<AOptionSetBase*> m_listOptions;
	std::wstring m_switch;
	std::wstring m_separator;
	std::wstring m_splitter;
	std::wstring m_quotation;
};
