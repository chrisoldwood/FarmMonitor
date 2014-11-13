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

#include "Query.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The runner for the WMI queries.

class QueryRunner
{
public:
	//! The type used for the collection of queries.
	typedef std::vector<ConstQueryPtr> Queries;

	//! The type used for the query results.
	typedef std::vector<tstring> Results;

public:
	//! Execute the queries for a host.
	static Results run(const tstring& host, const Queries& queries);
};

#endif // APP_QUERYRUNNER_HPP
