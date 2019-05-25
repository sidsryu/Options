#include "option-context-base.h"

OptionContextBase::OptionContextBase(const std::wstring& key,
	const std::wstring& description)
	: m_key(key)
	, m_description(description)
{}

OptionContextBase::~OptionContextBase()
{}

bool OptionContextBase::IsKey(const std::wstring& key) const
{
	return m_key == key;
}
