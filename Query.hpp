////////////////////////////////////////////////////////////////////////////////
//! \file   Query.hpp
//! \brief  The Query class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_QUERY_HPP
#define APP_QUERY_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/SharedPtr.hpp>

////////////////////////////////////////////////////////////////////////////////
//! A WMI query.

class Query
{
public:
	//! Default constructor.
	Query();

	//! Constructor.
	Query(const tstring& title, const tstring& wmiClass, const tstring& wmiProperty);
	
	//! Constructor.
	Query(const tstring& title, const tstring& wmiClass, const tstring& wmiProperty, const tstring& filterProperty, const tstring& filterValue);
	
	//
	// Members.
	//
	tstring		m_title;			//!< The title for the query column.
	tstring		m_wmiClass;			//!< The WMI class to query.
	tstring		m_wmiProperty;		//!< The WMI property to use.
	tstring		m_filterProperty;	//!< The (optional) property to filter on.
	tstring		m_filterValue;		//!< The (optional) property value to match.
};

//! The default Query const smart pointer type.
typedef Core::SharedPtr<const Query> ConstQueryPtr;

////////////////////////////////////////////////////////////////////////////////
//! Create a new query.

inline ConstQueryPtr makeQuery(const tstring& title, const tstring& wmiClass, const tstring& wmiProperty)
{
	return ConstQueryPtr(new Query(title, wmiClass, wmiProperty));
}

////////////////////////////////////////////////////////////////////////////////
//! Create a new query.

inline ConstQueryPtr makeQuery(const tstring& title, const tstring& wmiClass, const tstring& wmiProperty, const tstring& filterProperty, const tstring& filterValue)
{
	return ConstQueryPtr(new Query(title, wmiClass, wmiProperty, filterProperty, filterValue));
}

#endif // APP_QUERY_HPP
