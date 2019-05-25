#pragma once

#include "option-set-base.h"
#include <regex>


template<typename T>
class OptionSet: public OptionSetBase
{
public:
	OptionSet(std::wstring key, std::wstring description, T& outValue)
		: OptionSetBase(key, description)
		, m_outValue(outValue)
	{
	}

	// bool SetArgument() - 구현안함. 템플릿 특수화가 구현되지 않은 타입 사용 시 빌드에러를 낸다.

private:
	T& m_outValue;
};


// 불리언 옵션 - 옵션값을 읽지 않는다. 옵션 스위치만 있어도 true 설정
template<>
class OptionSet<bool>: public OptionSetBase
{
public:
	OptionSet(std::wstring key, std::wstring description, bool& outValue)
		: OptionSetBase(key, description)
		, m_outValue(outValue)
	{
		m_outValue = false;
	}

	virtual bool SetArgument(std::wstring arguments, std::wstring serialSeparator)
	{
		// 옵션값이 없거나 true, yes, enable, allow 값을 가지면 true. 그 외 모두 false
		std::wregex pattern(L" *(true|t|yes|y|enable|e|allow|a)? *", std::regex_constants::icase);

		std::match_results<std::wstring::const_iterator> result;
		if (!std::regex_match(arguments, result, pattern))
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
class OptionSet<std::wstring>: public OptionSetBase
{
public:
	OptionSet(std::wstring key, std::wstring description, std::wstring& outValue)
		: OptionSetBase(key, description)
		, m_outValue(outValue)
	{
	}

	virtual bool SetArgument(std::wstring arguments, std::wstring serialSeparator)
	{
		m_outValue = arguments;
		return true;	
	}

private:
	std::wstring& m_outValue;
};


// 정수 옵션 - 숫자로 읽는다. 숫자가 아닌 값 읽으면 0.
template<>
class OptionSet<int>: public OptionSetBase
{
public:
	OptionSet(std::wstring key, std::wstring description, int& outValue)
		: OptionSetBase(key, description)
		, m_outValue(outValue)
	{
		m_outValue = 0;
	}

	virtual bool SetArgument(std::wstring arguments, std::wstring serialSeparator)
	{
		m_outValue = _wtoi(arguments.c_str());
		return true;
	}

private:
	int& m_outValue;
};


// 문자열 목록 옵션 - 스플리터 설정에 따라, 옵션값을 분리해 읽는다.
template<>
class OptionSet<std::vector<std::wstring> >: public OptionSetBase
{
public:
	OptionSet(std::wstring key, std::wstring description, std::vector<std::wstring>& outValue)
		: OptionSetBase(key, description)
		, m_outValue(outValue)
	{
	}

	virtual bool SetArgument(std::wstring arguments, std::wstring serialSeparator)
	{
		return Split(arguments, serialSeparator);
	}

	virtual bool SetSplitArgument(std::wstring argument)
	{
		m_outValue.push_back(argument);
		return true;
	}

private:
	std::vector<std::wstring>& m_outValue;
};

// 숫자 목록 옵션 - 스플리터 설정에 따라, 옵션값을 분리해 읽는다.
template<>
class OptionSet<std::vector<int> >: public OptionSetBase
{
public:
	OptionSet(std::wstring key, std::wstring description, std::vector<int>& outValue)
		: OptionSetBase(key, description)
		, m_outValue(outValue)
	{
	}

	virtual bool SetArgument(std::wstring arguments, std::wstring serialSeparator)
	{
		return Split(arguments, serialSeparator);
	}

	virtual bool SetSplitArgument(std::wstring argument)
	{
		m_outValue.push_back(_wtoi(argument.c_str()));
		return true;
	}

private:
	std::vector<int>& m_outValue;
};
