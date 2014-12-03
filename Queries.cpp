////////////////////////////////////////////////////////////////////////////////
//! \file   Queries.cpp
//! \brief  The Queries class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Queries.hpp"
#include <XML/XPathIterator.hpp>
#include <XML/TextNode.hpp>
#include <Core/ConfigurationException.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Queries::Queries()
	: m_modified(false)
	, m_queries()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

Queries::~Queries()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the number of queries.

size_t Queries::size() const
{
	return m_queries.size();
}
	
////////////////////////////////////////////////////////////////////////////////
//! Get the query at a given position.

const ConstQueryPtr& Queries::query(size_t index) const
{
	ASSERT(index < m_queries.size());

	return m_queries[index];
}

////////////////////////////////////////////////////////////////////////////////
//! Has the collection of queries been modified?

bool Queries::isModified() const
{
	return m_modified;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the collection start iterator.

Queries::const_iterator Queries::begin() const
{
	return m_queries.begin();
}

////////////////////////////////////////////////////////////////////////////////
//! Get the collection end iterator.

Queries::const_iterator Queries::end() const
{
	return m_queries.end();
}

////////////////////////////////////////////////////////////////////////////////
//! Add a query to the collection.

void Queries::append(ConstQueryPtr query)
{
	m_queries.push_back(query);
	m_modified = true;
}

////////////////////////////////////////////////////////////////////////////////
//! Add a sequence of queries to the collection.

void Queries::append(const ConstQueryPtr* begin, const ConstQueryPtr* end)
{
	for (const ConstQueryPtr* it = begin; it != end; ++it)
		m_queries.push_back(*it);

	m_modified = true;
}

////////////////////////////////////////////////////////////////////////////////
//! Replace a query in the collection.

void Queries::replace(size_t index, ConstQueryPtr query)
{
	ASSERT(index < m_queries.size());

	m_queries[index] = query;
	m_modified = true;
}

////////////////////////////////////////////////////////////////////////////////
//! Remove a query from the collection.

void Queries::remove(size_t index)
{
	ASSERT(index < m_queries.size());

	m_queries.erase(m_queries.begin() + index);
	m_modified = true;
}

////////////////////////////////////////////////////////////////////////////////
//! Swap two queries in the collection by index.

void Queries::swap(size_t first, size_t second)
{
	ASSERT(first < m_queries.size());
	ASSERT(second < m_queries.size());

	ConstQueryPtr temp = m_queries[first];
	m_queries[first] = m_queries[second];
	m_queries[second] = temp;
	m_modified = true;
}

////////////////////////////////////////////////////////////////////////////////
//! Load the set of queries from the XML document.

void Queries::load(const XML::DocumentPtr config)
{
	ASSERT(config->hasRootElement());

	Collection queries;

	XML::XPathIterator it(TXT("/FarmMonitor/Queries/Query"), config->getRootElement());
	XML::XPathIterator end;

	for(; it != end; ++it)
	{
		XML::ElementNodePtr node = Core::dynamic_ptr_cast<XML::ElementNode>(*it);

		const tstring title = node->findFirstElement(TXT("Title"))->getTextValue();

		if (title.empty())
			throw Core::ConfigurationException(TXT("The title for a query cannot be empty"));

		const tstring wmiClass = node->findFirstElement(TXT("WMIClass"))->getTextValue();
		
		if (wmiClass.empty())
			throw Core::ConfigurationException(TXT("The WMI class for a query cannot be empty"));

		const tstring wmiProperty = node->findFirstElement(TXT("WMIProperty"))->getTextValue();
		
		if (wmiProperty.empty())
			throw Core::ConfigurationException(TXT("The WMI property for a query cannot be empty"));

		const tstring filterProperty = node->findFirstElement(TXT("FilterProperty"))->getTextValue();
		const tstring filterValue = node->findFirstElement(TXT("FilterValue"))->getTextValue();

		const tstring format = node->findFirstElement(TXT("Format"))->getTextValue();

		if (format.empty())
			throw Core::ConfigurationException(TXT("The format for a query cannot be empty"));

		queries.push_back(makeQuery(title, wmiClass, wmiProperty, filterProperty, filterValue, format));
	}

	std::swap(m_queries, queries);
	m_modified = false;
}

////////////////////////////////////////////////////////////////////////////////
//! Save the set of queries to the XML document.

void Queries::save(XML::DocumentPtr config)
{
	XML::XPathIterator  it(TXT("/FarmMonitor/Queries"), config->getRootElement());
	XML::ElementNodePtr queries(Core::dynamic_ptr_cast<XML::ElementNode>(*it));

	for (size_t i = 0; i != m_queries.size(); ++i)
	{
		XML::ElementNodePtr	title = XML::makeElement(TXT("Title"), XML::makeText(m_queries[i]->m_title));
		XML::ElementNodePtr	wmiClass = XML::makeElement(TXT("WMIClass"), XML::makeText(m_queries[i]->m_wmiClass));
		XML::ElementNodePtr	wmiProperty = XML::makeElement(TXT("WMIProperty"), XML::makeText(m_queries[i]->m_wmiProperty));
		XML::ElementNodePtr	filterProperty = XML::makeElement(TXT("FilterProperty"), XML::makeText(m_queries[i]->m_filterProperty));
		XML::ElementNodePtr	filterValue = XML::makeElement(TXT("FilterValue"), XML::makeText(m_queries[i]->m_filterValue));
		XML::ElementNodePtr	format = XML::makeElement(TXT("Format"), XML::makeText(m_queries[i]->m_format));

		XML::ElementNodePtr	query = XML::makeElement(TXT("Query"));

		query->appendChild(title);
		query->appendChild(wmiClass);
		query->appendChild(wmiProperty);
		query->appendChild(filterProperty);
		query->appendChild(filterValue);
		query->appendChild(format);
		queries->appendChild(query);
	}

	m_modified = false;
}

////////////////////////////////////////////////////////////////////////////////
//! Create a deep copy of another collection.

void Queries::deepCopy(const Queries& rhs)
{
	deepCopy(rhs, false);
}

////////////////////////////////////////////////////////////////////////////////
//! Replace the entire collection with another.

void Queries::replaceAll(const Queries& rhs)
{
	deepCopy(rhs, true);
}

////////////////////////////////////////////////////////////////////////////////
//! Create a deep copy of another collection.

void Queries::deepCopy(const Queries& rhs, bool modified)
{
	Collection queries;

	for (const_iterator it = rhs.begin(); it != rhs.end(); ++it)
		queries.push_back(makeQuery(*(*it)));

	std::swap(m_queries, queries);
	m_modified = modified;
}
