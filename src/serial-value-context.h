#pragma once

#include "option-context-base.h"

namespace options {
class SerialValueContext : public BaseContext
{
public:
	SerialValueContext(const std::wstring& key, const std::wstring& description);
	virtual ~SerialValueContext();

	virtual bool ParseValues(const std::wstring& values, const Syntax& syntax)
		override;

protected:
	virtual bool PushSplitedValue(const std::wstring& value) = 0;
};
}
