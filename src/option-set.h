#pragma once

#include "option-context-base.h"
#include "option-syntax.h"
#include <regex>

template<typename T>
class OptionSet : public OptionContextBase
{
public:
	OptionSet(const std::wstring& key, const std::wstring& description, T& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
	}

	// 구현안함. 템플릿 특수화가 구현되지 않은 타입 사용 시 빌드에러를 낸다.
	 virtual bool ParseValues(const std::wstring& values, const OptionSyntax& syntax) = delete;

private:
	T& m_outValue;
};


// 불리언 옵션 - 옵션값을 읽지 않는다. 옵션 스위치만 있어도 true 설정
template<>
class OptionSet<bool> : public OptionContextBase
{
public:
	OptionSet(const std::wstring& key, const std::wstring& description, bool& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
		m_outValue = false;
	}

	virtual bool ParseValues(const std::wstring& values, const OptionSyntax& syntax) override
	{
		auto booleanValue = syntax.BooleanValue();
		std::wregex pattern(booleanValue, std::regex_constants::icase);

		std::match_results<std::wstring::const_iterator> result;
		if (!std::regex_match(values, result, pattern))
		{
			return false;
		}

		m_outValue = true;
		return true;
	}

private:
	bool& m_outValue;
};


// 문자열 옵션 - 옵션값을 통채로 읽는다.
template<>
class OptionSet<std::wstring> : public OptionContextBase
{
public:
	OptionSet(const std::wstring& key, const std::wstring& description, std::wstring& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
	}

	virtual bool ParseValues(const std::wstring& values, const OptionSyntax& syntax) override
	{
		m_outValue = values;
		return true;
	}

private:
	std::wstring& m_outValue;
};


// 정수 옵션 - 숫자로 읽는다. 숫자가 아닌 값 읽으면 0.
template<>
class OptionSet<int> : public OptionContextBase
{
public:
	OptionSet(const std::wstring& key, const std::wstring& description, int& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
		m_outValue = 0;
	}

	virtual bool ParseValues(const std::wstring& values, const OptionSyntax& syntax) override
	{
		m_outValue = _wtoi(values.c_str());
		return true;
	}

private:
	int& m_outValue;
};


// 문자열 목록 옵션 - 스플리터 설정에 따라, 옵션값을 분리해 읽는다.
template<>
class OptionSet<std::vector<std::wstring>> : public OptionContextBase
{
public:
	OptionSet(const std::wstring& key, const std::wstring& description, std::vector<std::wstring>& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
	}

	virtual bool ParseValues(const std::wstring& values, const OptionSyntax& syntax) override
	{
		return Split(values, syntax);
	}

	virtual bool PushSplitedValue(const std::wstring& value) override
	{
		m_outValue.push_back(value);
		return true;
	}

private:
	std::vector<std::wstring>& m_outValue;
};

// 숫자 목록 옵션 - 스플리터 설정에 따라, 옵션값을 분리해 읽는다.
template<>
class OptionSet<std::vector<int> > : public OptionContextBase
{
public:
	OptionSet(const std::wstring& key, const std::wstring& description, std::vector<int>& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
	}

	virtual bool ParseValues(const std::wstring& values, const OptionSyntax& syntax) override
	{
		return Split(values, syntax);
	}

	virtual bool PushSplitedValue(const std::wstring& value) override
	{
		m_outValue.push_back(_wtoi(value.c_str()));
		return true;
	}

private:
	std::vector<int>& m_outValue;
};
