#pragma once

#include "option-set.h"
#include <string>
#include <vector>

class OptionSetBase;

class Options
{
public:
	Options();
	virtual ~Options();

	template<typename T>
	void Add(std::wstring key, std::wstring description, T& bindingVariable)
	{
		m_listofOption.push_back(new OptionSet<T>(key, description, bindingVariable));
	}

	bool Parse(std::wstring commandLine);
	
	bool SetSwitch(std::wstring newSwitch);
	bool SetKeyValueSeparator(std::wstring newKeyValueSeparator);
	bool SetSerialSeparator(std::wstring newSerialSeparator);

private:
	bool ValidCommandLine(std::wstring commandLine);
	bool ParseOptions(std::wstring commandLine);
	
	std::wstring MakeEscape(std::wstring text);
	std::wstring MakeNotMatch(std::wstring text);
	std::wstring MakeMatchOption(void);
	std::wstring MakeMatchValue(std::wstring notMatch);

private:
	std::vector<OptionSetBase*> m_listofOption;
	std::wstring m_switch;
	std::wstring m_keyValueSeparator;
	std::wstring m_serialSeparator;
	std::wstring m_quotation;
};
