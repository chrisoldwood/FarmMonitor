////////////////////////////////////////////////////////////////////////////////
//! \file   Query.cpp
//! \brief  The Query class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Query.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Query::Query()
	: m_wmiClass()
	, m_wmiProperty()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

Query::Query(const tstring& title, const tstring& wmiClass, const tstring& wmiProperty)
	: m_title(title)
	, m_wmiClass(wmiClass)
	, m_wmiProperty(wmiProperty)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

Query::Query(const tstring& title, const tstring& wmiClass, const tstring& wmiProperty, const tstring& filterProperty, const tstring& filterValue)
	: m_title(title)
	, m_wmiClass(wmiClass)
	, m_wmiProperty(wmiProperty)
	, m_filterProperty(filterProperty)
	, m_filterValue(filterValue)
{
}
