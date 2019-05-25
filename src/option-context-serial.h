#pragma once

#include "option-context-base.h"

class OptionContextSerial : public OptionContextBase
{
public:
	OptionContextSerial(const std::wstring& key, const std::wstring& description);
	virtual ~OptionContextSerial();

	virtual bool ParseValues(const std::wstring& values, const OptionSyntax& syntax)
		override;

protected:
	virtual bool PushSplitedValue(const std::wstring& value) = 0;
};
