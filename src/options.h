#pragma once

#include "option-set.h"
#include <string>
#include <vector>
#include <memory>

class OptionSetBase;
class OptionSyntax;

class Options
{
public:
	Options();
	virtual ~Options();

	template<typename T>
	void Add(const std::wstring& key, const std::wstring& description, T& bindingVariable)
	{
		m_listofOption.push_back(std::make_unique<OptionSet<T>>(key, description, bindingVariable));
	}

	bool Parse(const std::wstring& commandLine) const;

	bool SetSigil(const std::wstring& sigil);
	bool SetSeparator(const std::wstring& separator);
	bool SetSerialer(const std::wstring& serialer);

private:
	bool ValidCommandLine(const std::wstring& commandLine) const;
	bool VerifySyntax(const std::wstring& str, const std::wstring& syntax) const;
	bool ParseOptions(const std::wstring& commandLine) const;

private:	
	std::vector<std::unique_ptr<OptionSetBase>> m_listofOption;
	std::unique_ptr<OptionSyntax> m_syntax;
};
