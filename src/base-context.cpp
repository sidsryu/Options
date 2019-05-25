#include "base-context.h"

namespace options {
BaseContext::BaseContext(const std::wstring& key,
	const std::wstring& description)
	: m_key(key)
	, m_description(description)
{}

BaseContext::~BaseContext()
{}

bool BaseContext::IsKey(const std::wstring& key) const
{
	return m_key == key;
}
}
