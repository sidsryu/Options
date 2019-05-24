#include "AOptions.h"
#include <sstream>

AOptions::AOptions()
: m_switch(L"-")
, m_keyValueSeparator(L" ")
, m_serialSeparator(L" ")
, m_quotation(L"\"")
{
	// do nothing
}

AOptions::~AOptions()
{
	for (AOptionSetBase* optionSet : m_listOptions)
	{
		delete optionSet;
	}
}

bool AOptions::Parse(std::wstring commandLine)
{
	if (!ValidCommandLine(commandLine))
	{
		return false;
	}

	return ParseOptions(commandLine);
}

bool AOptions::SetSwitch(std::wstring newSwitch)
{
	// EmtpySwitch ��� - commandLine ��ü�� �ϳ��� �ɼ����� ����ϰԵȴ�.
	m_switch = MakeEscape(newSwitch);
	return true;
}

bool AOptions::SetKeyValueSeparator(std::wstring newKeyValueSeparator)
{
	if (newKeyValueSeparator.empty())	return false;

	m_keyValueSeparator = MakeEscape(newKeyValueSeparator);

	return true;
}

bool AOptions::SetSerialSeparator(std::wstring newSerialSeparator)
{
	if (newSerialSeparator.empty())	return false;

	m_serialSeparator = MakeEscape(newSerialSeparator);

	return true;
}

bool AOptions::ValidCommandLine(std::wstring commandLine)
{
	std::wstringstream maker;
	maker	<< "("
				<< "(^| +)"					// �� �ɼ��� �������� ����
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

bool AOptions::ParseOptions(std::wstring commandLine)
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
		std::wstring arguments = optionResult[2].matched ? optionResult[2].str() : optionResult[3].str();  // [2]�� ����ǥ ���� ��, [3]�� �Ϲ� ��

		for (AOptionSetBase* optionSet : m_listOptions)
		{
			if (!optionSet->Match(option))	continue;				
			if (!optionSet->SetArgument(arguments, m_serialSeparator))	return false;				
		}
	}

	return true;
}

std::wstring AOptions::MakeEscape(std::wstring text)
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

std::wstring AOptions::MakeNotMatch(std::wstring text)
{	
	if (text.empty())
	{
		return L"";
	}

	// look-around ����
	return L"(?!" + text + L")";
}

std::wstring AOptions::MakeMatchOption(void)
{
	std::wstringstream maker;
	maker	<< m_switch									// �ɼ� ����ġ
			<< "("
				<< "(?:"
					<< MakeNotMatch(m_switch)
					<< MakeNotMatch(m_keyValueSeparator)
				<< ".)+"
			<< ")"
			<< "(?:"									// �ɼǰ� ����
				<< m_keyValueSeparator							
				<< " *"
				<< "(?:"
					<< m_quotation
						<< MakeMatchValue(m_quotation)	// ����ǥ�� �����ϸ�, ����ġ ����
					<< m_quotation
				<< "|"
					<< MakeMatchValue(m_switch)
				<< ")"
			<< ")?";

	return maker.str();
}

std::wstring AOptions::MakeMatchValue(std::wstring notMatch)
{
	std::wstringstream maker;			
	maker	<< "("
				<< "(?:"							// ù �ɼǰ�
					<< MakeNotMatch(notMatch)
					<< MakeNotMatch(m_serialSeparator)					
				<< ".)+"
				<< "(?:"							// ������ �ɼǰ���
				<< m_serialSeparator
					<< "(?:"														
						<< MakeNotMatch(notMatch)
						<< MakeNotMatch(m_serialSeparator)						
					<< ".)+"
				<< ")*"
			<< ")";

	return maker.str();
}
