#include "options.h"
#include "option-syntax-symbols.h"
#include <sstream>

Options::Options()
	: m_symbols(std::make_unique<OptionSyntaxSymbols>())
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
	auto verification = VerificationRegex();
	return VerifyRegex(commandLine, verification);
}

bool Options::VerifyRegex(const std::wstring& str, const std::wstring& pattern) const
{
	std::wregex p(pattern);

	std::match_results<std::wstring::const_iterator> m;
	return std::regex_match(str, m, p);
}

bool Options::ParseOptions(const std::wstring& commandLine) const
{
	std::wregex optionPattern(OptionRegex());

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
			if (!optionSet->SetArgument(arguments, m_symbols->GetSerialSeparator())) return false;
		}
	}

	return true;
}

bool Options::SetSwitch(const std::wstring& newSwitch)
{
	return m_symbols->SetSwitch(newSwitch);
}

bool Options::SetKeyValueSeparator(const std::wstring& newKeyValueSeparator)
{
	return m_symbols->SetKeyValueSeparator(newKeyValueSeparator);
}

bool Options::SetSerialSeparator(const std::wstring& newSerialSeparator)
{
	return m_symbols->SetSerialSeparator(newSerialSeparator);
}

std::wstring Options::NotContainRegex(const std::wstring& excluded) const
{
	// Goal: "" or "(?!excluded)"

	if (excluded.empty()) return L"";
	return L"(?!" + excluded + L")";
}

std::wstring Options::OptionRegex(void) const
{
	// Goal: "switch(key)(?:separator *values)?"
	// like "-key" or "-key=values"

	auto switchSymbol = m_symbols->GetSwitch();
	auto separator = m_symbols->GetKeyValueSeparator();

	return switchSymbol +
		L"(" + KeyRegex() + L")(?:" + separator + L" *" + ValuesRegex() + L")?";
}

std::wstring Options::SerialValuesRegex(const std::wstring& excluded) const
{
	// Goal: "(value(?:separator value)*)"

	auto value = ValueRegex(excluded);
	auto separator = m_symbols->GetSerialSeparator();

	return L"(" + value + L"(?:" + separator + value + L")*)";
}

std::wstring Options::KeyRegex() const
{
	// Goal: "(?:no_contain_switch_and_separator.)+"

	auto noSwitch = NotContainRegex(m_symbols->GetSwitch());
	auto noSeparator = NotContainRegex(m_symbols->GetKeyValueSeparator());

	return L"(?:" + noSwitch + noSeparator + L".)+";
}

std::wstring Options::ValuesRegex() const
{
	// Goal: "(?:quotation_values|not_quotation_values)"
	return L"(?:" + QuotationValuesRegex() + L"|" + NotQuotationValuesRegex() + L")";
}

std::wstring Options::QuotationValuesRegex() const
{
	// Goal: R"("not_contain_quote_values")"
	return L"\"" + SerialValuesRegex(L"\"") + L"\"";
}

std::wstring Options::NotQuotationValuesRegex() const
{
	// Goal: "not_contain_switch_values"
	return SerialValuesRegex(m_symbols->GetSwitch());
}

std::wstring Options::ValueRegex(const std::wstring& excluded) const
{
	// Goal: "(?:no_contain_symbols.)+"

	auto noSymbols = NotContainRegex(excluded);
	auto noSeparator = NotContainRegex(m_symbols->GetSerialSeparator());

	return L"(?:" + noSymbols + noSeparator + L".)+";
}

std::wstring Options::VerificationRegex() const
{
	// Goal: "((^| +)option)* *"
	return L"((^| +)" + OptionRegex() + L")* *";
}
