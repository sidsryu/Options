#include "option-syntax-symbols.h"
#include <regex>

namespace options {
namespace {
std::wstring EscapeRegex(std::wstring text)
{
	// default regex ordinary character (ECMAScript only)
	std::wregex pattern{ LR"([()[\]{}|^$\.*+?])" };
	return std::regex_replace(text, pattern, LR"(\$&)");
}
}

OptionSyntaxSymbols::OptionSyntaxSymbols()
	: m_sigil(L"-")
	, m_separator(L" ")
	, m_serialer(L" ")
{}

OptionSyntaxSymbols::~OptionSyntaxSymbols()
{}

bool OptionSyntaxSymbols::SetSigil(const std::wstring& sigil)
{
	// Emtpy sigil 허용 - commandLine 전체를 하나의 옵션으로 취급하게된다.
	m_sigil = EscapeRegex(sigil);
	return true;
}

bool OptionSyntaxSymbols::SetSeparator(const std::wstring& separator)
{
	if (separator.empty()) return false;

	m_separator = EscapeRegex(separator);
	return true;
}

bool OptionSyntaxSymbols::SetSerialer(const std::wstring& serialer)
{
	if (serialer.empty()) return false;

	m_serialer = EscapeRegex(serialer);
	return true;
}

std::wstring OptionSyntaxSymbols::GetSigil() const
{
	return m_sigil;
}

std::wstring OptionSyntaxSymbols::GetSeparator() const
{
	return m_separator;
}

std::wstring OptionSyntaxSymbols::GetSerialer() const
{
	return m_serialer;
}
}
