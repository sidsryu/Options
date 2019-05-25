#pragma once

#include <string>

class OptionSetBase
{
public:
	OptionSetBase(const std::wstring& key, const std::wstring& description);
	virtual ~OptionSetBase();

	bool Match(const std::wstring& option);

	virtual bool SetArgument(const std::wstring& arguments, const std::wstring& serialSeparator) = 0;
	
protected:
	bool Split(const std::wstring& arguments, const std::wstring& serialSeparator);

	virtual bool SetSplitArgument(const std::wstring& argument)	{ return false;	}

private:
	std::wstring m_key;
	std::wstring m_description;
};
