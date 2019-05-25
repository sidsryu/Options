#pragma once

#include <string>

class OptionSetBase
{
public:
	OptionSetBase(std::wstring key, std::wstring description);
	virtual ~OptionSetBase();

	bool Match(std::wstring option);

	virtual bool SetArgument(std::wstring arguments, std::wstring serialSeparator) = 0;
	
protected:
	bool Split(std::wstring arguments, std::wstring serialSeparator);

	virtual bool SetSplitArgument(std::wstring argument)	{ return false;	}

private:
	std::wstring m_key;
	std::wstring m_description;
};
