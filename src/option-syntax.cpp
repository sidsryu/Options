#include "option-syntax.h"
#include "option-syntax-symbols.h"

OptionSyntax::OptionSyntax()
	: m_symbols(std::make_unique<OptionSyntaxSymbols>())
{}

OptionSyntax::~OptionSyntax()
{}

bool OptionSyntax::SetSwitch(const std::wstring& newSwitch)
{
	return m_symbols->SetSwitch(newSwitch);
}

bool OptionSyntax::SetKeyValueSeparator(const std::wstring& newKeyValueSeparator)
{
	return m_symbols->SetKeyValueSeparator(newKeyValueSeparator);
}

bool OptionSyntax::SetSerialSeparator(const std::wstring& newSerialSeparator)
{
	return m_symbols->SetSerialSeparator(newSerialSeparator);
}

std::wstring OptionSyntax::GetSerialSeparator() const
{
	return m_symbols->GetSerialSeparator();
}

std::wstring OptionSyntax::WholeCommandLine() const
{
	// Goal: "((^| +)option)* *"
	return L"((^| +)" + SingleOption() + L")* *";
}

std::wstring OptionSyntax::SingleOption(void) const
{
	// Goal: "switch(key)(?:separator *values)?"
	// like "-key" or "-key=values"

	auto switchSymbol = m_symbols->GetSwitch();
	auto separator = m_symbols->GetKeyValueSeparator();

	return switchSymbol +
		L"(" + KeyRegex() + L")(?:" + separator + L" *" + ValuesRegex() + L")?";
}

std::wstring OptionSyntax::NotContainRegex(const std::wstring& excluded) const
{
	// Goal: "" or "(?!excluded)"

	if (excluded.empty()) return L"";
	return L"(?!" + excluded + L")";
}

std::wstring OptionSyntax::SerialValuesRegex(const std::wstring& excluded) const
{
	// Goal: "(value(?:separator value)*)"

	auto value = ValueRegex(excluded);
	auto separator = m_symbols->GetSerialSeparator();

	return L"(" + value + L"(?:" + separator + value + L")*)";
}

std::wstring OptionSyntax::KeyRegex() const
{
	// Goal: "(?:no_contain_switch_and_separator.)+"

	auto noSwitch = NotContainRegex(m_symbols->GetSwitch());
	auto noSeparator = NotContainRegex(m_symbols->GetKeyValueSeparator());

	return L"(?:" + noSwitch + noSeparator + L".)+";
}

std::wstring OptionSyntax::ValuesRegex() const
{
	// Goal: "(?:quotation_values|not_quotation_values)"
	return L"(?:" + QuotationValuesRegex() + L"|" + NotQuotationValuesRegex() + L")";
}

std::wstring OptionSyntax::QuotationValuesRegex() const
{
	// Goal: R"("not_contain_quote_values")"
	return L"\"" + SerialValuesRegex(L"\"") + L"\"";
}

std::wstring OptionSyntax::NotQuotationValuesRegex() const
{
	// Goal: "not_contain_switch_values"
	return SerialValuesRegex(m_symbols->GetSwitch());
}

std::wstring OptionSyntax::ValueRegex(const std::wstring& excluded) const
{
	// Goal: "(?:no_contain_symbols.)+"

	auto noSymbols = NotContainRegex(excluded);
	auto noSeparator = NotContainRegex(m_symbols->GetSerialSeparator());

	return L"(?:" + noSymbols + noSeparator + L".)+";
}

