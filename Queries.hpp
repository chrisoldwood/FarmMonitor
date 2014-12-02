////////////////////////////////////////////////////////////////////////////////
//! \file   Queries.hpp
//! \brief  The Queries class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_QUERIES_HPP
#define APP_QUERIES_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Query.hpp"
#include <XML/Document.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The container of WMI queries to be executed when monitoring.

class Queries
{
private:
	//! The type used to store the collection of queries.
	typedef std::vector<ConstQueryPtr> Collection;

public:
	//! The const iterator type.
	typedef Collection::const_iterator const_iterator;

	//! Default constructor.
	Queries();

	//! Destructor.
	~Queries();
	
	//
	// Properties.
	//

	//! Get the number of queries.
	size_t size() const;

	//! Get the query at a given position.
	const ConstQueryPtr& query(size_t index) const;

	//! Has the collection of queries been modified?
	bool isModified() const;

	//! Get the collection start iterator.
	const_iterator begin() const;

	//! Get the collection end iterator.
	const_iterator end() const;

	//
	// Methods.
	//

	//! Add a query to the collection.
	void append(ConstQueryPtr query);

	//! Add a sequence of queries to the collection.
	void append(const ConstQueryPtr* begin, const ConstQueryPtr* end);

	//! Remove a query from the collection.
	void remove(size_t index);

	//! Swap two queries in the collection by index.
	void swap(size_t first, size_t second);

	//! Load the set of queries from the XML document.
	void load(const XML::DocumentPtr config);

	//! Save the set of queries to the XML document.
	void save(XML::DocumentPtr config);

	//! Create a deep copy of another collection.
	void deepCopy(const Queries& rhs);

	//! Replace the entire collection with another.
	void replaceAll(const Queries& rhs);

private:
	//
	// Members.
	//
	bool		m_modified;		//!< Has the collection been modified?
	Collection	m_queries;		//!< The collection of queries to execute.

	//! Create a deep copy of another collection.
	void deepCopy(const Queries& rhs, bool modified);
};

#endif // APP_QUERIES_HPP
