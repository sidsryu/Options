#pragma once

#include <string>

class OptionSyntaxSymbols
{
public:
	OptionSyntaxSymbols();
	virtual ~OptionSyntaxSymbols();

	bool SetSwitch(const std::wstring& newSwitch);
	bool SetKeyValueSeparator(const std::wstring& newKeyValueSeparator);
	bool SetSerialSeparator(const std::wstring& newSerialSeparator);

	std::wstring GetSwitch() const;
	std::wstring GetKeyValueSeparator() const;
	std::wstring GetSerialSeparator() const;

private:
	std::wstring m_switch;
	std::wstring m_keyValueSeparator;
	std::wstring m_serialSeparator;
};
