////////////////////////////////////////////////////////////////////////////////
//! \file   QueryRunner.hpp
//! \brief  The QueryRunner class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_QUERYRUNNER_HPP
#define APP_QUERYRUNNER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Queries.hpp"

// Forward declarations.
namespace WCL
{
class Variant;
}

namespace WMI
{
class Connection;
}

////////////////////////////////////////////////////////////////////////////////
//! The runner for the WMI queries.

class QueryRunner
{
public:
	//! The type used for the query sequence.
	typedef Queries::const_iterator const_iterator;

	//! The type used for the query results.
	typedef std::vector<tstring> Results;

public:
	//
	// Methods.
	//

	//! Execute a single query against a host.
	static tstring run(WMI::Connection& connection, const ConstQueryPtr& query);

	//! Execute the queries for a host.
	static Results run(WMI::Connection& connection, const ConstQueryPtr* begin, const ConstQueryPtr* end);

	//! Execute the queries for a host.
	static Results run(WMI::Connection& connection, const_iterator begin, const_iterator end);

	//
	// Internal methods.
	//
	
	//! Format the value according to the custom format string.
	static tstring formatValue(const WCL::Variant& value, const tstring& format);
};

#endif // APP_QUERYRUNNER_HPP
