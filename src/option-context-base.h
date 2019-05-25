#pragma once

#include <string>

namespace options {
class Syntax;

class BaseContext
{
public:
	BaseContext(const std::wstring& key, const std::wstring& description);
	virtual ~BaseContext();

	bool IsKey(const std::wstring& option) const;
	virtual bool ParseValues(const std::wstring& values, const Syntax& syntax)
		= 0;

private:
	std::wstring m_key;
	std::wstring m_description;
};
}
