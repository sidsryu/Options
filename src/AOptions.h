#pragma once

#include "AOptionSet.h"
#include <string>
#include <list>

/*
	Ŀ�ǵ���� �ļ�

	�⺻����
		app.exe -log -file filename1 filename2 -path foldername

	����
		bool            isLog;
		std::vector<std::wstring> fileList;
		std::wstring         foldername;


		AOptions options;		
		
		options.Add(L"log",  L"�α� ��¿���",     isLog);
		options.Add(L"file", L"������ ���ϸ��", fileList);
		options.Add(L"path", L"�����ǥ ������",   foldername);


		options.Parser(commandLine);

		if (isLog)					{ �α� ó�� }
		if (0 < fileList.size())	{ ���� ó�� }
		if (!foldername.empty())	{ ���� ó�� }
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
