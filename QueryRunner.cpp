////////////////////////////////////////////////////////////////////////////////
//! \file   QueryRunner.cpp
//! \brief  The QueryRunner class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "QueryRunner.hpp"
#include <Core/StringUtils.hpp>
#include <WMI/Connection.hpp>
#include <WMI/DateTime.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Try and convert a string into a datetime. The format of a WMI datetime is:-
//! YYYYMMDDHHMMSS.FFFFFF+TZO e.g. 20101008181758.546000+060

bool tryConvertDateTime(const tstring& value, tstring& datetime)
{
	CDateTime parsed;
	tstring   offset;

	if (!WMI::tryParseDateTime(value, parsed, offset))
		return false;

	datetime = Core::fmt(TXT("%s %s"), parsed.ToString().c_str(), offset.c_str());

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Format the value according to the custom format string.

static tstring formatValue(const WMI::ObjectIterator& it, const tstring& property, const tstring& format)
{
	WCL::Variant wmiValue;

	it->getProperty(property, wmiValue);

	tstring formattedValue = TXT("<unknown format>");

	if (format == TXT(""))
	{
		formattedValue = wmiValue.format();
	}
	else if (format == TXT("%s"))
	{
		formattedValue = wmiValue.format();
	}
	else if (format == TXT("%t"))
	{
		if (!tryConvertDateTime(wmiValue.format(), formattedValue))
			formattedValue = TXT("<non-datetime value>");
	}	

	return formattedValue;
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the queries for a host.

QueryRunner::Results QueryRunner::run(const tstring& host, const ConstQueryPtr* begin, const ConstQueryPtr* end)
{
	std::vector<ConstQueryPtr> queries(begin, end);

	return run(host, queries.begin(), queries.end());
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the queries for a host.

QueryRunner::Results QueryRunner::run(const tstring& host, const_iterator begin, const_iterator end)
{
	WMI::ObjectIterator resultsEnd;

	WMI::Connection connection(host);

	Results results;

	for (const_iterator queryIt = begin; queryIt != end; ++queryIt)
	{
		const ConstQueryPtr& query = *queryIt;

		tstring text = Core::fmt(TXT("SELECT * FROM %s"), query->m_wmiClass.c_str());

		if (!query->m_filterProperty.empty())
		{
			text += Core::fmt(TXT(" WHERE %s = '%s'"), query->m_filterProperty.c_str(),
														query->m_filterValue.c_str());
		}

		WMI::ObjectIterator resultsIt = connection.execQuery(text.c_str());

		if (resultsIt != resultsEnd)
			results.push_back(formatValue(resultsIt, query->m_wmiProperty, query->m_format));
		else
			results.push_back(TXT(""));
	}

	return results;
}
