#pragma once

#include <string>

class OptionSyntaxSymbols
{
public:
	OptionSyntaxSymbols();
	virtual ~OptionSyntaxSymbols();

	bool SetSigil(const std::wstring& sigil);
	bool SetSeparator(const std::wstring& separator);
	bool SetSerialer(const std::wstring& serialer);

	std::wstring GetSigil() const;
	std::wstring GetSeparator() const;
	std::wstring GetSerialer() const;

private:
	std::wstring m_sigil;
	std::wstring m_separator;
	std::wstring m_serialer;
};
