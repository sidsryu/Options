#include "options.h"
#include "option-syntax.h"

namespace options {
Options::Options()
	: m_syntax(std::make_unique<Syntax>())
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

	std::match_results<std::wstring::const_iterator> result;
	return std::regex_match(str, result, pattern);
}

bool Options::ParseOptions(const std::wstring& commandLine) const
{
	auto singleOption = m_syntax->SingleOption();
	std::wregex pattern(singleOption);

	std::wsregex_token_iterator end;
	std::wsregex_token_iterator begin(commandLine.begin(), commandLine.end(),
		pattern);
	for (auto pos = begin; pos != end; pos++)
	{
		const std::wstring& option = *pos;

		std::match_results<std::wstring::const_iterator> result;
		if (!std::regex_match(option, result, pattern))
		{
			return false;
		}

		// [1]: key, [2]: quoted values, [3]: plain values
		std::wstring key = result[1].str();
		std::wstring values = result[2].matched ? result[2].str() : result[3].str();

		for (auto& it : m_listofOption)
		{
			if (!it->IsKey(key)) continue;
			if (!it->ParseValues(values, *m_syntax.get())) return false;
		}
	}

	return true;
}

bool Options::SetSigil(const std::wstring& sigil)
{
	return m_syntax->SetSigil(sigil);
}

bool Options::SetSeparator(const std::wstring& separator)
{
	return m_syntax->SetSeparator(separator);
}

bool Options::SetSerialer(const std::wstring& serialer)
{
	return m_syntax->SetSerialer(serialer);
}
}
