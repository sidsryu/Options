#include "option-context-base.h"
#include <sstream>
#include <regex>

OptionContextBase::OptionContextBase(const std::wstring& key, const std::wstring& description)
	: m_key(key)
	, m_description(description)
{
	// do nothing
}

OptionContextBase::~OptionContextBase()
{
	// do nothing
}

bool OptionContextBase::Match(const std::wstring& key)
{ 
	return m_key == key;
}

bool OptionContextBase::Split(const std::wstring& arguments, const std::wstring& serialSeparator)
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
