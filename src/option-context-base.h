#pragma once

#include <string>

namespace options {
class OptionSyntax;

class OptionContextBase
{
public:
	OptionContextBase(const std::wstring& key, const std::wstring& description);
	virtual ~OptionContextBase();

	bool IsKey(const std::wstring& option) const;
	virtual bool ParseValues(const std::wstring& values, const OptionSyntax& syntax)
		= 0;

private:
	std::wstring m_key;
	std::wstring m_description;
};
}
