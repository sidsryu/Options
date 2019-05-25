#include "options.h"
#include "option-syntax.h"

Options::Options()
	: m_syntax(std::make_unique<OptionSyntax>())
{}

Options::~Options()
{}

bool Options::Parse(const std::wstring& commandLine) const
{
	if (!ValidCommandLine(commandLine))
	{
		return false;
	}

	return ParseOptions(commandLine);
}

bool Options::ValidCommandLine(const std::wstring& commandLine) const
{
	auto syntax = m_syntax->WholeCommandLine();
	return VerifySyntax(commandLine, syntax);
}

bool Options::VerifySyntax(const std::wstring& str, const std::wstring& syntax) const
{
	std::wregex pattern(syntax);

	std::match_results<std::wstring::const_iterator> match;
	return std::regex_match(str, match, pattern);
}

bool Options::ParseOptions(const std::wstring& commandLine) const
{
	std::wregex optionPattern(m_syntax->SingleOption());

	std::wsregex_token_iterator begin(commandLine.begin(), commandLine.end(), optionPattern), end;
	for (std::wsregex_token_iterator it = begin; it != end; it++)
	{
		const std::wstring& token = *it;

		std::match_results<std::wstring::const_iterator> optionResult;
		if (!std::regex_match(token, optionResult, optionPattern))
		{
			return false;
		}

		const std::wstring& option = optionResult[1].str();
		const std::wstring& arguments = optionResult[2].matched ? optionResult[2].str() : optionResult[3].str();  // [2]는 따옴표 묶인 값, [3]은 일반 값

		for (auto& optionSet : m_listofOption)
		{
			if (!optionSet->Match(option)) continue;
			if (!optionSet->SetArgument(arguments, m_syntax->GetSerialSeparator())) return false;
		}
	}

	return true;
}

bool Options::SetSwitch(const std::wstring& newSwitch)
{
	return m_syntax->SetSwitch(newSwitch);
}

bool Options::SetKeyValueSeparator(const std::wstring& newKeyValueSeparator)
{
	return m_syntax->SetKeyValueSeparator(newKeyValueSeparator);
}

bool Options::SetSerialSeparator(const std::wstring& newSerialSeparator)
{
	return m_syntax->SetSerialSeparator(newSerialSeparator);
}
