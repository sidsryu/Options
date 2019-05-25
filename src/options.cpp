#include "options.h"
#include "option-syntax-symbols.h"
#include <sstream>

Options::Options()
	: m_symbols(std::make_unique<OptionSyntaxSymbols>())
{}

Options::~Options()
{}

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
	return m_symbols->SetSwitch(newSwitch);
}

bool Options::SetKeyValueSeparator(std::wstring newKeyValueSeparator)
{
	return m_symbols->SetKeyValueSeparator(newKeyValueSeparator);
}

bool Options::SetSerialSeparator(std::wstring newSerialSeparator)
{
	return m_symbols->SetSerialSeparator(newSerialSeparator);
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

		for (auto& optionSet : m_listofOption)
		{
			if (!optionSet->Match(option))	continue;				
			if (!optionSet->SetArgument(arguments, m_symbols->GetSerialSeparator()))	return false;
		}
	}

	return true;
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
	maker	<< m_symbols->GetSwitch()					// 옵션 스위치
			<< "("
				<< "(?:"
					<< MakeNotMatch(m_symbols->GetSwitch())
					<< MakeNotMatch(m_symbols->GetKeyValueSeparator())
				<< ".)+"
			<< ")"
			<< "(?:"									// 옵션값 시작
				<< m_symbols->GetKeyValueSeparator()
				<< " *"
				<< "(?:"
					<< L"\""
						<< MakeMatchValue(L"\"")	// 따옴표로 시작하면, 스위치 무시
					<< L"\""
				<< "|"
					<< MakeMatchValue(m_symbols->GetSwitch())
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
					<< MakeNotMatch(m_symbols->GetSerialSeparator())					
				<< ".)+"
				<< "(?:"							// 나머지 옵션값들
				<< m_symbols->GetSerialSeparator()
					<< "(?:"														
						<< MakeNotMatch(notMatch)
						<< MakeNotMatch(m_symbols->GetSerialSeparator())						
					<< ".)+"
				<< ")*"
			<< ")";

	return maker.str();
}
