#include "option-syntax.h"
#include "option-syntax-symbols.h"

namespace options {
Syntax::Syntax()
	: m_symbols(std::make_unique<Symbols>())
{}

Syntax::~Syntax()
{}

bool Syntax::SetSigil(const std::wstring& sigil)
{
	return m_symbols->SetSigil(sigil);
}

bool Syntax::SetSeparator(const std::wstring& separator)
{
	return m_symbols->SetSeparator(separator);
}

bool Syntax::SetSerialer(const std::wstring& serialer)
{
	return m_symbols->SetSerialer(serialer);
}

std::wstring Syntax::WholeCommandLine() const
{
	// Goal: "((^| +)option)* *"
	return L"((^| +)" + SingleOption() + L")* *";
}

std::wstring Syntax::SingleOption() const
{
	// Goal: "sigil(key)(?:separator *values)?"
	// like "-key" or "-key=values"

	auto sigil = m_symbols->GetSigil();
	auto separator = m_symbols->GetSeparator();

	return sigil +
		L"(" + SingleKey() + L")(?:" + separator + L" *" + WholeValues() + L")?";
}

std::wstring Syntax::SingleKey() const
{
	// Goal: "(?:no_contain_sigil_and_separator.)+"

	auto noSigil = NotContain(m_symbols->GetSigil());
	auto noSeparator = NotContain(m_symbols->GetSeparator());

	return L"(?:" + noSigil + noSeparator + L".)+";
}

std::wstring Syntax::WholeValues() const
{
	// Goal: "(?:quoted_values|plain_values)"
	return L"(?:" + QuotedWholeValues() + L"|" + PlainWholeValues() + L")";
}

std::wstring Syntax::QuotedWholeValues() const
{
	// Goal: R"("not_contain_quote_values")"
	return L"\"" + SerialValues(L"\"") + L"\"";
}

std::wstring Syntax::PlainWholeValues() const
{
	// Goal: "not_contain_sigil_values"
	return SerialValues(m_symbols->GetSigil());
}

std::wstring Syntax::SerialValues(const std::wstring& excluded) const
{
	// Goal: "(value(?:serialervalue)*)"

	auto value = SingleValue(excluded);
	auto serialer = m_symbols->GetSerialer();

	return L"(" + value + L"(?:" + serialer + value + L")*)";
}

std::wstring Syntax::SingleValue(const std::wstring& excluded) const
{
	// Goal: "(?:no_contain_symbols.)+"

	auto noMatch = NotContain(excluded);
	auto noSerialer = NotContain(m_symbols->GetSerialer());

	return L"(?:" + noMatch + noSerialer + L".)+";
}

std::wstring Syntax::NotContain(const std::wstring& excluded) const
{
	// Goal: "" or "(?!excluded)"

	if (excluded.empty()) return L"";
	return L"(?!" + excluded + L")";
}

std::wstring Syntax::PopSingleValue() const
{
	// Goal: " ?(value) *(?:serialer)?"
	// sigil, quote 이미 제외되어 있음

	auto serialer = m_symbols->GetSerialer();
	return L" ?(" + SingleValue({}) + L") *(?:" + serialer + L")?";
}

std::wstring Syntax::BooleanValue() const
{
	// 옵션값이 없거나 true, yes, enable, allow 값을 가지면 true. 그 외 모두 false
	return L" *(true|t|yes|y|enable|e|allow|a)? *";
}
}
