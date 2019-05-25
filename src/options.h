#pragma once

#include "option-set.h"
#include <string>
#include <vector>
#include <memory>

class OptionSetBase;
class OptionSyntaxSymbols;

class Options
{
public:
	Options();
	virtual ~Options();

	template<typename T>
	void Add(std::wstring key, std::wstring description, T& bindingVariable)
	{
		m_listofOption.push_back(std::make_unique<OptionSet<T>>(key, description, bindingVariable));
	}

	bool Parse(std::wstring commandLine);
	
	bool SetSwitch(std::wstring newSwitch);
	bool SetKeyValueSeparator(std::wstring newKeyValueSeparator);
	bool SetSerialSeparator(std::wstring newSerialSeparator);

private:
	bool ValidCommandLine(std::wstring commandLine);
	bool ParseOptions(std::wstring commandLine);
	
	std::wstring MakeNotMatch(std::wstring text);
	std::wstring MakeMatchOption(void);
	std::wstring MakeMatchValue(std::wstring notMatch);
	std::wstring MakeMatchKey();
	std::wstring MakeMatchValues();
	std::wstring MakeMatchString();

private:
	std::vector<std::unique_ptr<OptionSetBase>> m_listofOption;
	std::unique_ptr<OptionSyntaxSymbols> m_symbols;
};
