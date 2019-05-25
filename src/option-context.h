#pragma once

#include "option-context-base.h"
#include "option-context-serial.h"
#include "option-syntax.h"
#include <regex>


// �⺻ ���ø� - ���ø� Ư��ȭ�� �������� ���� Ÿ�� ��� ��, ���忡���� ����.
template<typename T>
class OptionContext : public OptionContextBase
{
public:
	OptionContext(const std::wstring& key, const std::wstring& description,
		T& outValue)
	{}

	// ParseValues() ���忡���� �������� ��������.
};


// �Ҹ��� �ɼ� - �ɼǰ��� ���� �ʴ´�. �ɼ� ����ġ�� �־ true ����
template<>
class OptionContext<bool> : public OptionContextBase
{
public:
	OptionContext(const std::wstring& key, const std::wstring& description,
		bool& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
		m_outValue = false;
	}

	virtual bool ParseValues(const std::wstring& values, const OptionSyntax& syntax)
		override
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


// ���ڿ� �ɼ� - �ɼǰ��� ��ä�� �д´�.
template<>
class OptionContext<std::wstring> : public OptionContextBase
{
public:
	OptionContext(const std::wstring& key, const std::wstring& description,
		std::wstring& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
		m_outValue = {};
	}

	virtual bool ParseValues(const std::wstring& values, const OptionSyntax& syntax)
		override
	{
		m_outValue = values;
		return true;
	}

private:
	std::wstring& m_outValue;
};


// ���� �ɼ� - ���ڷ� �д´�. ���ڰ� �ƴ� �� ������ 0.
template<>
class OptionContext<int> : public OptionContextBase
{
public:
	OptionContext(const std::wstring& key, const std::wstring& description,
		int& outValue)
		: OptionContextBase(key, description)
		, m_outValue(outValue)
	{
		m_outValue = 0;
	}

	virtual bool ParseValues(const std::wstring& values, const OptionSyntax& syntax)
		override
	{
		m_outValue = std::stoi(values.c_str());
		return true;
	}

private:
	int& m_outValue;
};


// ���ڿ� ��� �ɼ� - �ø��� ������ ����, �ɼǰ��� �и��� �д´�.
template<>
class OptionContext<std::vector<std::wstring>> : public OptionContextSerial
{
public:
	OptionContext(const std::wstring& key, const std::wstring& description,
		std::vector<std::wstring>& outValue)
		: OptionContextSerial(key, description)
		, m_outValue(outValue)
	{
		m_outValue = {};
	}

	virtual bool PushSplitedValue(const std::wstring& value) override
	{
		m_outValue.push_back(value);
		return true;
	}

private:
	std::vector<std::wstring>& m_outValue;
};


// ���� ��� �ɼ� - �ø��� ������ ����, �ɼǰ��� �и��� �д´�.
template<>
class OptionContext<std::vector<int> > : public OptionContextSerial
{
public:
	OptionContext(const std::wstring& key, const std::wstring& description,
		std::vector<int>& outValue)
		: OptionContextSerial(key, description)
		, m_outValue(outValue)
	{
		m_outValue = {};
	}

	virtual bool PushSplitedValue(const std::wstring& value) override
	{
		m_outValue.push_back(std::stoi(value.c_str()));
		return true;
	}

private:
	std::vector<int>& m_outValue;
};
