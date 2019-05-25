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
		L"(" + SingleKey() + L")(?:" + separator + L" *" + WholeValues() + L")?";
}

std::wstring OptionSyntax::SingleKey() const
{
	// Goal: "(?:no_contain_switch_and_separator.)+"

	auto noSwitch = NotContain(m_symbols->GetSwitch());
	auto noSeparator = NotContain(m_symbols->GetKeyValueSeparator());

	return L"(?:" + noSwitch + noSeparator + L".)+";
}

std::wstring OptionSyntax::WholeValues() const
{
	// Goal: "(?:quoted_values|plane_values)"
	return L"(?:" + QuotedWholeValues() + L"|" + PlaneWholeValues() + L")";
}

std::wstring OptionSyntax::QuotedWholeValues() const
{
	// Goal: R"("not_contain_quote_values")"
	return L"\"" + SerialValues(L"\"") + L"\"";
}

std::wstring OptionSyntax::PlaneWholeValues() const
{
	// Goal: "not_contain_switch_values"
	return SerialValues(m_symbols->GetSwitch());
}

std::wstring OptionSyntax::SerialValues(const std::wstring& excluded) const
{
	// Goal: "(value(?:separator value)*)"

	auto value = SingleValue(excluded);
	auto separator = m_symbols->GetSerialSeparator();

	return L"(" + value + L"(?:" + separator + value + L")*)";
}

std::wstring OptionSyntax::SingleValue(const std::wstring& excluded) const
{
	// Goal: "(?:no_contain_symbols.)+"

	auto noSymbols = NotContain(excluded);
	auto noSeparator = NotContain(m_symbols->GetSerialSeparator());

	return L"(?:" + noSymbols + noSeparator + L".)+";
}

std::wstring OptionSyntax::NotContain(const std::wstring& excluded) const
{
	// Goal: "" or "(?!excluded)"

	if (excluded.empty()) return L"";
	return L"(?!" + excluded + L")";
}
