#include "option-context-serial.h"
#include "option-syntax.h"
#include <regex>

namespace options {
OptionContextSerial::OptionContextSerial(
	const std::wstring& key, const std::wstring& description)
	: OptionContextBase(key, description)
{}

OptionContextSerial::~OptionContextSerial()
{}

bool OptionContextSerial::ParseValues(const std::wstring& values,
	const OptionSyntax& syntax)
{
	auto popSingleValue = syntax.PopSingleValue();
	std::wregex pattern(popSingleValue);

	std::wsregex_token_iterator begin(values.begin(), values.end(), pattern, 1);
	std::wsregex_token_iterator end;
	for (auto pos = begin; pos != end; pos++)
	{
		if (!PushSplitedValue(*pos)) return false;
	}

	return true;
}
}
