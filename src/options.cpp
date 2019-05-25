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

bool Options::ValidCommandLine(std::wstring commandLine)
{
	auto verification = VerificationRegex();
	return VerifyRegex(commandLine, verification);
}

bool Options::VerifyRegex(std::wstring str, std::wstring pattern)
{
	std::wregex p(pattern);

	std::match_results<std::wstring::const_iterator> m;
	return std::regex_match(str, m, p);
}

bool Options::ParseOptions(std::wstring commandLine)
{
	std::wregex optionPattern(OptionRegex());

	std::wsregex_token_iterator begin(commandLine.begin(), commandLine.end(), optionPattern), end;
	for (std::wsregex_token_iterator it = begin; it != end; it++)
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
			if (!optionSet->Match(option)) continue;
			if (!optionSet->SetArgument(arguments, m_symbols->GetSerialSeparator())) return false;
		}
	}

	return true;
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

std::wstring Options::NotContainRegex(std::wstring text)
{
	if (text.empty())
	{
		return L"";
	}

	// look-around 응용
	return L"(?!" + text + L")";
}

std::wstring Options::OptionRegex(void)
{
	// Goal: "switch(key)(separator(values))?"
	// like "-key" or "-key=values"

	auto switch_symbol = m_symbols->GetSwitch();

	auto key = KeyRegex();
	auto values = ValuesRegex();

	std::wstringstream ss;
	ss << switch_symbol << key << values;

	return ss.str();
}

std::wstring Options::ValueRegex(std::wstring notMatch)
{
	// Goal: "((first_value)(rest_value)*)
	// Goal: "((?:no_separator.)+)(?:separator(?:no_separator.)+)*)"

	auto begin = L"(";
	auto end = L")";

	auto no_match = NotContainRegex(notMatch);
	auto no_separator = NotContainRegex(m_symbols->GetSerialSeparator());
	auto first_value = L"(?:" + no_match + no_separator + L".)+";

	auto separator = m_symbols->GetSerialSeparator();
	auto rest_values = L"(?:" + separator + first_value + L")*";


	std::wstringstream ss;
	ss << begin << first_value << rest_values << end;

	return ss.str();
}

std::wstring Options::KeyRegex()
{
	// Goal: "((?:no_switch_no_separator.)+)"
	// switch와 separator가 포함되지 않은 연속된 문자열

	auto begin = L"(";
	auto end = L")";

	auto no_switch = NotContainRegex(m_symbols->GetSwitch());
	auto no_separator = NotContainRegex(m_symbols->GetKeyValueSeparator());
	auto key = L"(?:" + no_switch + no_separator + L".)+";


	std::wstringstream ss;
	ss << begin << key << end;

	return ss.str();
}

std::wstring Options::ValuesRegex()
{
	// Goal: "(?:separator *(?:quotation_string|serial_values))?"

	auto begin = L"(?:"; // 캡처 안함
	auto end = L")?"; // 0 or 1개, key만 있을 수 있음.

	auto separator = m_symbols->GetKeyValueSeparator();
	auto ltrim = L" *";

	auto quotation_string = QuotationStringRegex();
	auto serial_values = ValueRegex(m_symbols->GetSwitch());

	auto values = L"(?:" + quotation_string + L"|" + serial_values + L")";


	std::wstringstream ss;
	ss << begin << separator << ltrim << values << end;

	return ss.str();
}

std::wstring Options::QuotationStringRegex()
{
	return L"\"" + ValueRegex(L"\"") + L"\"";
}

std::wstring Options::VerificationRegex()
{
	// Goal: "((^| +)option)* *"

	auto begin = L"(";
	auto end = L")*"; // 0개 이상의 옵션

	auto delimiter = L"(^| +)"; // 옵션 구분자. 첫 옵션을 제외하곤 공백문자로 구분된다.
	auto option = OptionRegex();
	auto rtrim = L" *";

	std::wstringstream ss;
	ss << begin << delimiter << option << end << rtrim;

	return ss.str();
}
