#pragma once

#include <string>

class OptionSetBase
{
public:
	OptionSetBase(std::wstring option, std::wstring description);
	~OptionSetBase();

	bool Match(std::wstring option);

	virtual bool SetArgument(std::wstring arguments, std::wstring serialSeparator) = 0;
	
protected:
	bool Split(std::wstring arguments, std::wstring serialSeparator);

	virtual bool SetSplitArgument(std::wstring argument)	{ return false;	}

private:
	std::wstring m_option;
	std::wstring m_description;
};
