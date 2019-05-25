#include "option-syntax-symbols.h"
#include <regex>

namespace {
	std::wstring EscapeRegex(std::wstring text)
	{
		// default regex ordinary character (ECMAScript only)
		std::wregex pattern{ LR"([()[\]{}|^$\.*+?])" };
		auto t = std::regex_replace(text, pattern, LR"(\$&)");
		return t;
	}
}

OptionSyntaxSymbols::OptionSyntaxSymbols()
	: m_switch(L"-")
	, m_keyValueSeparator(L" ")
	, m_serialSeparator(L" ")
{}

OptionSyntaxSymbols::~OptionSyntaxSymbols()
{}

bool OptionSyntaxSymbols::SetSwitch(std::wstring newSwitch)
{
	// Emtpy switch 허용 - commandLine 전체를 하나의 옵션으로 취급하게된다.
	m_switch = EscapeRegex(newSwitch);
	return true;
}

bool OptionSyntaxSymbols::SetKeyValueSeparator(std::wstring newKeyValueSeparator)
{
	if (newKeyValueSeparator.empty()) return false;

	m_keyValueSeparator = EscapeRegex(newKeyValueSeparator);
	return true;
}

bool OptionSyntaxSymbols::SetSerialSeparator(std::wstring newSerialSeparator)
{
	if (newSerialSeparator.empty()) return false;

	m_serialSeparator = EscapeRegex(newSerialSeparator);
	return true;
}

std::wstring OptionSyntaxSymbols::GetSwitch() const
{
	return m_switch;
}

std::wstring OptionSyntaxSymbols::GetKeyValueSeparator() const
{
	return m_keyValueSeparator;
}

std::wstring OptionSyntaxSymbols::GetSerialSeparator() const
{
	return m_serialSeparator;
}
