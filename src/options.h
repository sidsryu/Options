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
	void Add(const std::wstring& key, const std::wstring& description, T& bindingVariable)
	{
		m_listofOption.push_back(std::make_unique<OptionSet<T>>(key, description, bindingVariable));
	}

	bool Parse(const std::wstring& commandLine);

	bool SetSwitch(const std::wstring& newSwitch);
	bool SetKeyValueSeparator(const std::wstring& newKeyValueSeparator);
	bool SetSerialSeparator(const std::wstring& newSerialSeparator);

private:
	bool ValidCommandLine(const std::wstring& commandLine);
	bool VerifyRegex(const std::wstring& str, const std::wstring& pattern);
	bool ParseOptions(const std::wstring& commandLine);

	std::wstring VerificationRegex();
	std::wstring NotContainRegex(const std::wstring& text);
	std::wstring OptionRegex(void);
	std::wstring ValueRegex(const std::wstring& notMatch);
	std::wstring KeyRegex();
	std::wstring ValuesRegex();
	std::wstring QuotationStringRegex();

private:
	std::vector<std::unique_ptr<OptionSetBase>> m_listofOption;
	std::unique_ptr<OptionSyntaxSymbols> m_symbols;
};
