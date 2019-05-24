#include "option-set.h"
#include <sstream>

OptionSetBase::OptionSetBase(std::wstring option, std::wstring description)
: m_option(option)
, m_description(description)
{
	// do nothing
}

OptionSetBase::~OptionSetBase()
{
	// do nothing
}

bool OptionSetBase::Match(std::wstring option)			
{ 
	return m_option == option;
}

bool OptionSetBase::Split(std::wstring arguments, std::wstring serialSeparator)
{
	std::wstringstream maker;
	maker	<< " ?"									// l-trim
			<< "("
				<< "(?:(?!" << serialSeparator << ").)+"	// an argument, 분리자 포함안함.
			<< ")"
			<< " *"									// r-trim
			<< "(?:" << serialSeparator << ")?";

	std::wregex argumentPattern(maker.str());

	std::wsregex_token_iterator begin(arguments.begin(), arguments.end(), argumentPattern, 1), end;
	for(std::wsregex_token_iterator it = begin; it != end; it++)
	{
		if (!SetSplitArgument(*it))	return false;			
	}

	return true;		
}
