#pragma once

#include <string>

class OptionSyntaxSymbols
{
public:
	OptionSyntaxSymbols();
	virtual ~OptionSyntaxSymbols();

	bool SetSwitch(std::wstring newSwitch);
	bool SetKeyValueSeparator(std::wstring newKeyValueSeparator);
	bool SetSerialSeparator(std::wstring newSerialSeparator);

	std::wstring GetSwitch() const;
	std::wstring GetKeyValueSeparator() const;
	std::wstring GetSerialSeparator() const;

private:
	std::wstring m_switch;
	std::wstring m_keyValueSeparator;
	std::wstring m_serialSeparator;
};
