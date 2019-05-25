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

	bool Parse(const std::wstring& commandLine) const;

	bool SetSwitch(const std::wstring& newSwitch);
	bool SetKeyValueSeparator(const std::wstring& newKeyValueSeparator);
	bool SetSerialSeparator(const std::wstring& newSerialSeparator);

private:
	bool ValidCommandLine(const std::wstring& commandLine) const;
	bool VerifyRegex(const std::wstring& str, const std::wstring& pattern) const;
	bool ParseOptions(const std::wstring& commandLine) const;

	std::wstring VerificationRegex() const;
	std::wstring NotContainRegex(const std::wstring& text) const;	
	std::wstring OptionRegex(void) const;
	std::wstring SerialValuesRegex(const std::wstring& excluded) const;
	std::wstring KeyRegex() const;
	std::wstring ValuesRegex() const;
	std::wstring ValueRegex(const std::wstring& excluded) const;
	std::wstring QuotationValuesRegex() const;
	std::wstring NotQuotationValuesRegex() const;

private:
	std::vector<std::unique_ptr<OptionSetBase>> m_listofOption;
	std::unique_ptr<OptionSyntaxSymbols> m_symbols;
};
