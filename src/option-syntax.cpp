#include "option-syntax.h"
#include "option-syntax-symbols.h"

OptionSyntax::OptionSyntax()
	: m_symbols(std::make_unique<OptionSyntaxSymbols>())
{}

OptionSyntax::~OptionSyntax()
{}

bool OptionSyntax::SetSigil(const std::wstring& sigil)
{
	return m_symbols->SetSigil(sigil);
}

bool OptionSyntax::SetSeparator(const std::wstring& separator)
{
	return m_symbols->SetSeparator(separator);
}

bool OptionSyntax::SetSerialer(const std::wstring& serialer)
{
	return m_symbols->SetSerialer(serialer);
}

std::wstring OptionSyntax::GetSerialer() const
{
	return m_symbols->GetSerialer();
}

std::wstring OptionSyntax::WholeCommandLine() const
{
	// Goal: "((^| +)option)* *"
	return L"((^| +)" + SingleOption() + L")* *";
}

std::wstring OptionSyntax::SingleOption(void) const
{
	// Goal: "sigil(key)(?:separator *values)?"
	// like "-key" or "-key=values"

	auto sigil = m_symbols->GetSigil();
	auto separator = m_symbols->GetSeparator();

	return sigil +
		L"(" + SingleKey() + L")(?:" + separator + L" *" + WholeValues() + L")?";
}

std::wstring OptionSyntax::SingleKey() const
{
	// Goal: "(?:no_contain_sigil_and_separator.)+"

	auto noSigil = NotContain(m_symbols->GetSigil());
	auto noSeparator = NotContain(m_symbols->GetSeparator());

	return L"(?:" + noSigil + noSeparator + L".)+";
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
	// Goal: "not_contain_sigil_values"
	return SerialValues(m_symbols->GetSigil());
}

std::wstring OptionSyntax::SerialValues(const std::wstring& excluded) const
{
	// Goal: "(value(?:serialervalue)*)"

	auto value = SingleValue(excluded);
	auto serialer = m_symbols->GetSerialer();

	return L"(" + value + L"(?:" + serialer + value + L")*)";
}

std::wstring OptionSyntax::SingleValue(const std::wstring& excluded) const
{
	// Goal: "(?:no_contain_symbols.)+"

	auto noMatch = NotContain(excluded);
	auto noSerialer = NotContain(m_symbols->GetSerialer());

	return L"(?:" + noMatch + noSerialer + L".)+";
}

std::wstring OptionSyntax::NotContain(const std::wstring& excluded) const
{
	// Goal: "" or "(?!excluded)"

	if (excluded.empty()) return L"";
	return L"(?!" + excluded + L")";
}
