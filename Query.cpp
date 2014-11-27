////////////////////////////////////////////////////////////////////////////////
//! \file   Query.cpp
//! \brief  The Query class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Query.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constants.

const tstring Query::STRING_FORMAT = TXT("%s");
const tstring Query::DATETIME_FORMAT = TXT("%t");
const tstring Query::DEFAULT_FORMAT = STRING_FORMAT;

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Query::Query()
	: m_title()
	, m_wmiClass()
	, m_wmiProperty()
	, m_filterProperty()
	, m_filterValue()
	, m_format(DEFAULT_FORMAT)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

Query::Query(const tstring& title, const tstring& wmiClass, const tstring& wmiProperty)
	: m_title(title)
	, m_wmiClass(wmiClass)
	, m_wmiProperty(wmiProperty)
	, m_filterProperty()
	, m_filterValue()
	, m_format(DEFAULT_FORMAT)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

Query::Query(const tstring& title, const tstring& wmiClass, const tstring& wmiProperty,
				const tstring& filterProperty, const tstring& filterValue, const tstring& format)
	: m_title(title)
	, m_wmiClass(wmiClass)
	, m_wmiProperty(wmiProperty)
	, m_filterProperty(filterProperty)
	, m_filterValue(filterValue)
	, m_format(format)
{
}
