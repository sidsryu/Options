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
	// EmtpySwitch ��� - commandLine ��ü�� �ϳ��� �ɼ����� ����ϰԵȴ�.
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
		std::wstring arguments = optionResult[2].matched ? optionResult[2].str() : optionResult[3].str();  // [2]�� ����ǥ ���� ��, [3]�� �Ϲ� ��

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

	// look-around ����
	return L"(?!" + text + L")";
}

std::wstring Options::MakeMatchOption(void)
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

std::wstring Options::MakeMatchValue(std::wstring notMatch)
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
