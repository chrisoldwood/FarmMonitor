////////////////////////////////////////////////////////////////////////////////
//! \file   QueryRunner.cpp
//! \brief  The QueryRunner class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "QueryRunner.hpp"
#include <Core/StringUtils.hpp>
#include <WMI/Connection.hpp>

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
			results.push_back(resultsIt->getProperty<tstring>(query->m_wmiProperty));
		else
			results.push_back(TXT(""));
	}

	return results;
}
