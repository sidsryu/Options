#pragma once

#include <string>
#include <memory>

namespace options {
class Symbols;

class Syntax
{
public:
	Syntax();
	virtual ~Syntax();

	bool SetSigil(const std::wstring& sigil);
	bool SetSeparator(const std::wstring& separator);
	bool SetSerialer(const std::wstring& serialer);

	std::wstring WholeCommandLine() const;
	std::wstring SingleOption() const;
	std::wstring PopSingleValue() const;
	std::wstring BooleanValue() const;

private:
	std::wstring SingleKey() const;
	std::wstring WholeValues() const;
	std::wstring QuotedWholeValues() const;
	std::wstring PlainWholeValues() const;
	std::wstring SerialValues(const std::wstring& excluded) const;
	std::wstring SingleValue(const std::wstring& excluded) const;
	std::wstring NotContain(const std::wstring& text) const;

private:
	std::unique_ptr<Symbols> m_symbols;
};
}
