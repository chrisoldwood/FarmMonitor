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
	//! Execute the queries for a host.
	static Results run(const tstring& host, const ConstQueryPtr* begin, const ConstQueryPtr* end);

	//! Execute the queries for a host.
	static Results run(const tstring& host, const_iterator begin, const_iterator end);
};

#endif // APP_QUERYRUNNER_HPP
