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
	std::wstring NotContain(const std::wstring& text) const;
	std::wstring SerialValues(const std::wstring& excluded) const;
	std::wstring SingleKey() const;
	std::wstring WholeValues() const;
	std::wstring SingleValue(const std::wstring& excluded) const;
	std::wstring QuotedWholeValues() const;
	std::wstring PlaneWholeValues() const;

private:
	std::unique_ptr<OptionSyntaxSymbols> m_symbols;
};
