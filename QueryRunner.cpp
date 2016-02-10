////////////////////////////////////////////////////////////////////////////////
//! \file   QueryRunner.cpp
//! \brief  The QueryRunner class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "QueryRunner.hpp"
#include <Core/StringUtils.hpp>
#include <WMI/Connection.hpp>
#include <WMI/DateTime.hpp>
#include "Formats.hpp"
#include <WMI/ObjectIterator.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Execute the queries for a host.

QueryRunner::Results QueryRunner::run(WMI::Connection& connection, const ConstQueryPtr* begin, const ConstQueryPtr* end)
{
	std::vector<ConstQueryPtr> queries(begin, end);

	return run(connection, queries.begin(), queries.end());
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the queries for a host.

QueryRunner::Results QueryRunner::run(WMI::Connection& connection, const_iterator begin, const_iterator end)
{
	WMI::ObjectIterator resultsEnd;
	Results             results;

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
		tstring             formattedValue = TXT("");

		if (resultsIt != resultsEnd)
		{
			if (resultsIt->hasProperty(query->m_wmiProperty))
			{
				WCL::Variant value;

				resultsIt->getProperty(query->m_wmiProperty, value);

				formattedValue = formatValue(value, query->m_format);
			}
			else
			{
				formattedValue = TXT("#N/A");
			}
		}

		results.push_back(formattedValue);
	}

	return results;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the string used to separate groups of digits in a number.

static tstring getGroupSeparator()
{
	int    count = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHOUSANDSEP, nullptr, 0);
	tchar* buffer = static_cast<tchar*>(_alloca(Core::numBytes<tchar>(count+1)));

	buffer[0] = TXT('\0');

	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHOUSANDSEP, buffer, count+1) == 0)
		return TXT(",");

	return buffer;
}

////////////////////////////////////////////////////////////////////////////////
//! Format an integer value.

static tstring formatIntegerValue(uint64 value)
{
	typedef tstring::reverse_iterator rev_iter;
	typedef tstring::const_reverse_iterator c_rev_iter;

	const tstring rawResult = Core::format<uint64>(value);

	const size_t  numDigits = (rawResult[0] != TXT('-')) ? rawResult.length() : (rawResult.length()-1);
	const tstring separator = getGroupSeparator();
	const size_t  numSeps = (numDigits-1) / 3;
	const size_t  total = rawResult.length() + (numSeps * separator.length());

	tstring result = tstring(total, TXT(' '));

	c_rev_iter it = rawResult.rbegin();
	c_rev_iter end = rawResult.rend();

	size_t   digits = 0;
	size_t   seps = 0;
	rev_iter output = result.rbegin();

	while (it != end)
	{
		*output++ = *it++;

		if ( ((++digits % 3) == 0) && (seps++ != numSeps) )
			output = std::copy(separator.rbegin(), separator.rend(), output);
	}

	return result;
}

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

tstring QueryRunner::formatValue(const WCL::Variant& value, const tstring& format)
{
	try
	{
		tstring formattedValue = TXT("<unknown format>");

		if (format == Formats::STRING)
		{
			formattedValue = value.format();
		}
		else if (format == Formats::DATETIME)
		{
			if (!tryConvertDateTime(WCL::getValue<tstring>(value), formattedValue))
				formattedValue = TXT("<non-datetime value>");
		}	
		else if (format == Formats::BYTES)
		{
			WCL::Variant uint64Value(value, VT_UI8);

			formattedValue = formatIntegerValue(V_UI8(&uint64Value)) + TXT(" B");
		}
		else if (format == Formats::K_BYTES)
		{
			WCL::Variant uint64Value(value, VT_UI8);

			formattedValue = formatIntegerValue(V_UI8(&uint64Value) / 1024u) + TXT(" KB");
		}
		else if (format == Formats::M_BYTES)
		{
			WCL::Variant uint64Value(value, VT_UI8);

			formattedValue = formatIntegerValue(V_UI8(&uint64Value) / (1024u*1024u)) + TXT(" MB");
		}
		else if (format == Formats::G_BYTES)
		{
			WCL::Variant uint64Value(value, VT_UI8);

			formattedValue = formatIntegerValue(V_UI8(&uint64Value) / (1024u*1024u*1024u)) + TXT(" GB");
		}
		else if (format == Formats::KB_AS_MB)
		{
			WCL::Variant uint64Value(value, VT_UI8);

			formattedValue = formatIntegerValue(V_UI8(&uint64Value) / 1024u) + TXT(" MB");
		}
		else if (format == Formats::KB_AS_GB)
		{
			WCL::Variant uint64Value(value, VT_UI8);

			formattedValue = formatIntegerValue(V_UI8(&uint64Value) / (1024u*1024u)) + TXT(" GB");
		}

		return formattedValue;
	}
	catch (const WCL::ComException& /*e*/)
	{
		return TXT("#ERR");
	}
}
