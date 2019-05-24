#include "Options.h"
#include <sstream>

Options::Options()
: m_switch(L"-")
, m_keyValueSeparator(L" ")
, m_serialSeparator(L" ")
, m_quotation(L"\"")
{
	// do nothing
}

Options::~Options()
{
	for (OptionSetBase* optionSet : m_listOptions)
	{
		delete optionSet;
	}
}

bool Options::Parse(std::wstring commandLine)
{
	if (!ValidCommandLine(commandLine))
	{
		return false;
	}

	return ParseOptions(commandLine);
}

bool Options::SetSwitch(std::wstring newSwitch)
{
	// EmtpySwitch 허용 - commandLine 전체를 하나의 옵션으로 취급하게된다.
	m_switch = MakeEscape(newSwitch);
	return true;
}

bool Options::SetKeyValueSeparator(std::wstring newKeyValueSeparator)
{
	if (newKeyValueSeparator.empty())	return false;

	m_keyValueSeparator = MakeEscape(newKeyValueSeparator);

	return true;
}

bool Options::SetSerialSeparator(std::wstring newSerialSeparator)
{
	if (newSerialSeparator.empty())	return false;

	m_serialSeparator = MakeEscape(newSerialSeparator);

	return true;
}

bool Options::ValidCommandLine(std::wstring commandLine)
{
	std::wstringstream maker;
	maker	<< "("
				<< "(^| +)"					// 각 옵션은 공백으로 구분
				<< MakeMatchOption()	
			<< ")*"
			<< " *";						// l-trim

	std::wregex commandLinePattern(maker.str());

	std::match_results<std::wstring::const_iterator> commandResult;
	if (!std::regex_match(commandLine, commandResult, commandLinePattern))
	{
		return false;
	}

	return true;
}

bool Options::ParseOptions(std::wstring commandLine)
{
	std::wregex optionPattern(MakeMatchOption());

	std::wsregex_token_iterator begin(commandLine.begin(), commandLine.end(), optionPattern), end;
	for(std::wsregex_token_iterator it = begin; it != end; it++)
	{
		std::wstring token = *it;

		std::match_results<std::wstring::const_iterator> optionResult;
		if (!std::regex_match(token, optionResult, optionPattern))
		{
			return false;
		}
		
		std::wstring option = optionResult[1].str();
		std::wstring arguments = optionResult[2].matched ? optionResult[2].str() : optionResult[3].str();  // [2]는 따옴표 묶인 값, [3]은 일반 값

		for (OptionSetBase* optionSet : m_listOptions)
		{
			if (!optionSet->Match(option))	continue;				
			if (!optionSet->SetArgument(arguments, m_serialSeparator))	return false;				
		}
	}

	return true;
}

std::wstring Options::MakeEscape(std::wstring text)
{
	std::wstring escapeText = text;

	std::wstring::size_type pos = 0;
	for(;;)
	{			
		// default regex ordinary character (ECMAScript)
		pos = escapeText.find_first_of(L"^$\\.*+?()[]{}|", pos);	

		if (pos == std::wstring::npos)
		{
			break;
		}

		escapeText.insert(pos, L"\\");
		pos += 2;
	}

	return escapeText;
}

std::wstring Options::MakeNotMatch(std::wstring text)
{	
	if (text.empty())
	{
		return L"";
	}

	// look-around 응용
	return L"(?!" + text + L")";
}

std::wstring Options::MakeMatchOption(void)
{
	std::wstringstream maker;
	maker	<< m_switch									// 옵션 스위치
			<< "("
				<< "(?:"
					<< MakeNotMatch(m_switch)
					<< MakeNotMatch(m_keyValueSeparator)
				<< ".)+"
			<< ")"
			<< "(?:"									// 옵션값 시작
				<< m_keyValueSeparator							
				<< " *"
				<< "(?:"
					<< m_quotation
						<< MakeMatchValue(m_quotation)	// 따옴표로 시작하면, 스위치 무시
					<< m_quotation
				<< "|"
					<< MakeMatchValue(m_switch)
				<< ")"
			<< ")?";

	return maker.str();
}

std::wstring Options::MakeMatchValue(std::wstring notMatch)
{
	std::wstringstream maker;			
	maker	<< "("
				<< "(?:"							// 첫 옵션값
					<< MakeNotMatch(notMatch)
					<< MakeNotMatch(m_serialSeparator)					
				<< ".)+"
				<< "(?:"							// 나머지 옵션값들
				<< m_serialSeparator
					<< "(?:"														
						<< MakeNotMatch(notMatch)
						<< MakeNotMatch(m_serialSeparator)						
					<< ".)+"
				<< ")*"
			<< ")";

	return maker.str();
}
