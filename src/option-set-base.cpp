#include "option-set.h"
#include <sstream>

OptionSetBase::OptionSetBase(const std::wstring& key, const std::wstring& description)
	: m_key(key)
	, m_description(description)
{
	// do nothing
}

OptionSetBase::~OptionSetBase()
{
	// do nothing
}

bool OptionSetBase::Match(const std::wstring& key)
{ 
	return m_key == key;
}

bool OptionSetBase::Split(const std::wstring& arguments, const std::wstring& serialSeparator)
{
	std::wstringstream maker;
	maker	<< " ?"									// l-trim
			<< "("
				<< "(?:(?!" << serialSeparator << ").)+"	// an argument, �и��� ���Ծ���.
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
