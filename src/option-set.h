#pragma once

#include "option-context-base.h"
#include <regex>


template<typename T>
class OptionSet: public OptionContextBase
{
public:
	OptionSet(const std::wstring& key, const std::wstring& description, T& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
	}

	// bool SetArgument() - ��������. ���ø� Ư��ȭ�� �������� ���� Ÿ�� ��� �� ���忡���� ����.

private:
	T& m_outValue;
};


// �Ҹ��� �ɼ� - �ɼǰ��� ���� �ʴ´�. �ɼ� ����ġ�� �־ true ����
template<>
class OptionSet<bool>: public OptionContextBase
{
public:
	OptionSet(const std::wstring& key, const std::wstring& description, bool& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
		m_outValue = false;
	}

	virtual bool SetArgument(const std::wstring& arguments, const std::wstring& serialSeparator)
	{
		// �ɼǰ��� ���ų� true, yes, enable, allow ���� ������ true. �� �� ��� false
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


// ���ڿ� �ɼ� - �ɼǰ��� ��ä�� �д´�.
template<>
class OptionSet<std::wstring>: public OptionContextBase
{
public:
	OptionSet(const std::wstring& key, const std::wstring& description, std::wstring& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
	}

	virtual bool SetArgument(const std::wstring& arguments, const std::wstring& serialSeparator)
	{
		m_outValue = arguments;
		return true;	
	}

private:
	std::wstring& m_outValue;
};


// ���� �ɼ� - ���ڷ� �д´�. ���ڰ� �ƴ� �� ������ 0.
template<>
class OptionSet<int>: public OptionContextBase
{
public:
	OptionSet(const std::wstring& key, const std::wstring& description, int& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
		m_outValue = 0;
	}

	virtual bool SetArgument(const std::wstring& arguments, const std::wstring& serialSeparator)
	{
		m_outValue = _wtoi(arguments.c_str());
		return true;
	}

private:
	int& m_outValue;
};


// ���ڿ� ��� �ɼ� - ���ø��� ������ ����, �ɼǰ��� �и��� �д´�.
template<>
class OptionSet<std::vector<std::wstring>>: public OptionContextBase
{
public:
	OptionSet(const std::wstring& key, const std::wstring& description, std::vector<std::wstring>& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
	}

	virtual bool SetArgument(const std::wstring& arguments, const std::wstring& serialSeparator)
	{
		return Split(arguments, serialSeparator);
	}

	virtual bool SetSplitArgument(const std::wstring& argument)
	{
		m_outValue.push_back(argument);
		return true;
	}

private:
	std::vector<std::wstring>& m_outValue;
};

// ���� ��� �ɼ� - ���ø��� ������ ����, �ɼǰ��� �и��� �д´�.
template<>
class OptionSet<std::vector<int> >: public OptionContextBase
{
public:
	OptionSet(const std::wstring& key, const std::wstring& description, std::vector<int>& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
	}

	virtual bool SetArgument(const std::wstring& arguments, const std::wstring& serialSeparator)
	{
		return Split(arguments, serialSeparator);
	}

	virtual bool SetSplitArgument(const std::wstring& argument)
	{
		m_outValue.push_back(_wtoi(argument.c_str()));
		return true;
	}

private:
	std::vector<int>& m_outValue;
};
