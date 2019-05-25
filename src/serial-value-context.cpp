#include "serial-value-context.h"
#include "syntax.h"
#include <regex>

namespace options {
SerialValueContext::SerialValueContext(
	const std::wstring& key, const std::wstring& description)
	: BaseContext(key, description)
{}

SerialValueContext::~SerialValueContext()
{}

bool SerialValueContext::ParseValues(const std::wstring& values,
	const Syntax& syntax)
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
