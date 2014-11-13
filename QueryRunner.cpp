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

QueryRunner::Results QueryRunner::run(const tstring& host, const Queries& queries)
{
	WMI::ObjectIterator end;

	WMI::Connection connection(host);

	Results results;

	for (Queries::const_iterator queryIt = queries.begin(); queryIt != queries.end(); ++queryIt)
	{
		const ConstQueryPtr& query = *queryIt;

		tstring text = Core::fmt(TXT("SELECT * FROM %s"), query->m_wmiClass.c_str());

		if (!query->m_filterProperty.empty())
		{
			text += Core::fmt(TXT(" WHERE %s = '%s'"), query->m_filterProperty.c_str(),
														query->m_filterValue.c_str());
		}

		WMI::ObjectIterator objectIt = connection.execQuery(text.c_str());

		if (objectIt != end)
			results.push_back(objectIt->getProperty<tstring>(query->m_wmiProperty));
		else
			results.push_back(TXT(""));
	}

	return results;
}
