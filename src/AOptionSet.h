#pragma once

#include "AOptionSetBase.h"
#include <regex>


template<typename T>
class AOptionSet: public AOptionSetBase
{
public:
	AOptionSet(std::wstring option, std::wstring description, T& outValue)
		: AOptionSetBase(option, description)
		, m_outValue(outValue)
	{
	}

	// bool SetArgument() - ��������. ���ø� Ư��ȭ�� �������� ���� Ÿ�� ��� �� ���忡���� ����.

private:
	T& m_outValue;
};


// �Ҹ��� �ɼ� - �ɼǰ��� ���� �ʴ´�. �ɼ� ����ġ�� �־ true ����
template<>
class AOptionSet<bool>: public AOptionSetBase
{
public:
	AOptionSet(std::wstring option, std::wstring description, bool& outValue)
		: AOptionSetBase(option, description)
		, m_outValue(outValue)
	{
		m_outValue = false;
	}

	virtual bool SetArgument(std::wstring arguments, std::wstring splitter)
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
class AOptionSet<std::wstring>: public AOptionSetBase
{
public:
	AOptionSet(std::wstring option, std::wstring description, std::wstring& outValue)
		: AOptionSetBase(option, description)
		, m_outValue(outValue)
	{
	}

	virtual bool SetArgument(std::wstring arguments, std::wstring splitter)
	{
		m_outValue = arguments;
		return true;	
	}

private:
	std::wstring& m_outValue;
};


// ���� �ɼ� - ���ڷ� �д´�. ���ڰ� �ƴ� �� ������ 0.
template<>
class AOptionSet<int>: public AOptionSetBase
{
public:
	AOptionSet(std::wstring option, std::wstring description, int& outValue)
		: AOptionSetBase(option, description)
		, m_outValue(outValue)
	{
		m_outValue = 0;
	}

	virtual bool SetArgument(std::wstring arguments, std::wstring splitter)
	{
		m_outValue = _wtoi(arguments.c_str());
		return true;
	}

private:
	int& m_outValue;
};


// ���ڿ� ��� �ɼ� - ���ø��� ������ ����, �ɼǰ��� �и��� �д´�.
template<>
class AOptionSet<std::vector<std::wstring> >: public AOptionSetBase
{
public:
	AOptionSet(std::wstring option, std::wstring description, std::vector<std::wstring>& outValue)
		: AOptionSetBase(option, description)
		, m_outValue(outValue)
	{
	}

	virtual bool SetArgument(std::wstring arguments, std::wstring splitter)
	{
		return Split(arguments, splitter);
	}

	virtual bool SetSplitArgument(std::wstring argument)
	{
		m_outValue.push_back(argument);
		return true;
	}

private:
	std::vector<std::wstring>& m_outValue;
};

// ���� ��� �ɼ� - ���ø��� ������ ����, �ɼǰ��� �и��� �д´�.
template<>
class AOptionSet<std::vector<int> >: public AOptionSetBase
{
public:
	AOptionSet(std::wstring option, std::wstring description, std::vector<int>& outValue)
		: AOptionSetBase(option, description)
		, m_outValue(outValue)
	{
	}

	virtual bool SetArgument(std::wstring arguments, std::wstring splitter)
	{
		return Split(arguments, splitter);
	}

	virtual bool SetSplitArgument(std::wstring argument)
	{
		m_outValue.push_back(_wtoi(argument.c_str()));
		return true;
	}

private:
	std::vector<int>& m_outValue;
};
