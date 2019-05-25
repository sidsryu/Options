#pragma once

#include <string>

class OptionContextBase
{
public:
	OptionContextBase(const std::wstring& key, const std::wstring& description);
	virtual ~OptionContextBase();

	bool IsKey(const std::wstring& option) const;
	virtual bool ParseValues(const std::wstring& values, const std::wstring& serialer) = 0;

protected:
	bool Split(const std::wstring& values, const std::wstring& serialer);
	virtual bool PushSplitedValue(const std::wstring& value) { return false; }

private:
	std::wstring m_key;
	std::wstring m_description;
};
