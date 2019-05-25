#pragma once

#include <string>
#include <memory>

class OptionSyntaxSymbols;

class OptionSyntax
{
public:
	OptionSyntax();
	virtual ~OptionSyntax();

	bool SetSwitch(const std::wstring& newSwitch);
	bool SetKeyValueSeparator(const std::wstring& newKeyValueSeparator);
	bool SetSerialSeparator(const std::wstring& newSerialSeparator);

	std::wstring GetSerialSeparator() const;

	std::wstring WholeCommandLine() const;
	std::wstring SingleOption(void) const;

private:
	std::wstring NotContainRegex(const std::wstring& text) const;
	std::wstring SerialValuesRegex(const std::wstring& excluded) const;
	std::wstring KeyRegex() const;
	std::wstring ValuesRegex() const;
	std::wstring ValueRegex(const std::wstring& excluded) const;
	std::wstring QuotationValuesRegex() const;
	std::wstring NotQuotationValuesRegex() const;

private:
	std::unique_ptr<OptionSyntaxSymbols> m_symbols;
};
