#include "option-context-base.h"
#include "option-syntax.h"
#include <sstream>
#include <regex>

OptionContextBase::OptionContextBase(const std::wstring& key, const std::wstring& description)
	: m_key(key)
	, m_description(description)
{}

OptionContextBase::~OptionContextBase()
{}

bool OptionContextBase::IsKey(const std::wstring& key) const
{
	return m_key == key;
}

bool OptionContextBase::Split(const std::wstring& values, const OptionSyntax& syntax)
{
	auto popSingleValue = syntax.PopSingleValue();
	std::wregex pattern(popSingleValue);

	std::wsregex_token_iterator begin(values.begin(), values.end(), pattern, 1), end;
	for (auto it = begin; it != end; it++)
	{
		if (!PushSplitedValue(*it))	return false;
	}

	return true;
}
