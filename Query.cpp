////////////////////////////////////////////////////////////////////////////////
//! \file   Query.cpp
//! \brief  The Query class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Query.hpp"
#include "Formats.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Query::Query()
	: m_title()
	, m_wmiClass()
	, m_wmiProperty()
	, m_filterProperty()
	, m_filterValue()
	, m_format(Formats::DEFAULT)
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
	, m_format(Formats::DEFAULT)
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
