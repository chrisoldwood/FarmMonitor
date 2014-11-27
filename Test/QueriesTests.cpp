////////////////////////////////////////////////////////////////////////////////
//! \file   QueriesTests.cpp
//! \brief  The unit tests for the Queries class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "Queries.hpp"
#include <XML/XPathIterator.hpp>
#include <XML/TextNode.hpp>
#include "DataDocument.hpp"

namespace
{

XML::DocumentPtr createDocument()
{
	return createDocumentShell();
}

XML::DocumentPtr createDocument(const tstring& title, const tstring& wmiClass, const tstring& wmiProperty,
									const tstring& filterProperty, const tstring& filterValue,
									const tstring& format)
{
	XML::DocumentPtr    document(createDocumentShell());
	XML::XPathIterator  it(TXT("/FarmMonitor/Queries"), document);
	XML::ElementNodePtr queries(Core::dynamic_ptr_cast<XML::ElementNode>(*it));

	XML::NodePtr properties[] =
	{
		XML::makeElement(TXT("Title"), XML::makeText(title)),
		XML::makeElement(TXT("WMIClass"), XML::makeText(wmiClass)),
		XML::makeElement(TXT("WMIProperty"), XML::makeText(wmiProperty)),
		XML::makeElement(TXT("FilterProperty"), XML::makeText(filterProperty)),
		XML::makeElement(TXT("FilterValue"), XML::makeText(filterValue)),
		XML::makeElement(TXT("Format"), XML::makeText(format)),
	};

	queries->appendChild(XML::makeElement
	(
		TXT("Query"), properties, properties+ARRAY_SIZE(properties)
	));

	return document;
}

XML::DocumentPtr createDocument(const tstring& title, const tstring& wmiClass, const tstring& wmiProperty)
{
	return createDocument(title, wmiClass, wmiProperty, TXT(""), TXT(""), TXT(""));
}

const tstring SAVED_TITLE = TXT("saved title");
const tstring SAVED_WMI_CLASS = TXT("saved WMI class");
const tstring SAVED_WMI_PROPERTY = TXT("saved WMI property");
const tstring SAVED_FILTER_PROPERTY = TXT("saved filter property");
const tstring SAVED_FILTER_VALUE = TXT("saved filter value");
const tstring SAVED_FORMAT = TXT("%t");

}

TEST_SET(Queries)
{
	const tstring TEST_TITLE = TXT("test title");
	const tstring TEST_WMI_CLASS = TXT("test WMI class");
	const tstring TEST_WMI_PROPERTY = TXT("test WMI property");
	const tstring TEST_FILTER_PROPERTY = TXT("test filter property");
	const tstring TEST_FILTER_VALUE = TXT("test filter value");
	const tstring TEST_FORMAT = TXT("%t");

TEST_CASE("By default the container has no items and is not modified")
{
	Queries queries;

	TEST_TRUE(queries.size() == 0);
	TEST_FALSE(queries.isModified());
}
TEST_CASE_END

TEST_CASE("Appending a query increases the size and marks the container as modified")
{
	Queries queries;

	queries.append(makeQuery(TEST_TITLE, TEST_WMI_CLASS, TEST_WMI_PROPERTY));

	TEST_TRUE(queries.size() == 1);
	TEST_TRUE(queries.isModified());
}
TEST_CASE_END

TEST_CASE("A set of queries can be loaded from an XML document")
{
	XML::DocumentPtr config = createDocument(SAVED_TITLE, SAVED_WMI_CLASS, SAVED_WMI_PROPERTY,
												SAVED_FILTER_PROPERTY, SAVED_FILTER_VALUE, SAVED_FORMAT);
	Queries queries;

	queries.load(config);

	TEST_FALSE(queries.isModified());
	TEST_TRUE(queries.size() == 1);
	TEST_TRUE(queries.query(0)->m_title == SAVED_TITLE);
	TEST_TRUE(queries.query(0)->m_wmiClass == SAVED_WMI_CLASS);
	TEST_TRUE(queries.query(0)->m_wmiProperty == SAVED_WMI_PROPERTY);
	TEST_TRUE(queries.query(0)->m_filterProperty == SAVED_FILTER_PROPERTY);
	TEST_TRUE(queries.query(0)->m_filterValue == SAVED_FILTER_VALUE);
	TEST_TRUE(queries.query(0)->m_format == SAVED_FORMAT);
}
TEST_CASE_END

TEST_CASE("Loading a query with no title throws")
{
	const tstring EMPTY_TITLE;

	Queries queries;

	XML::DocumentPtr config = createDocument(EMPTY_TITLE, SAVED_WMI_CLASS, SAVED_WMI_PROPERTY,
												SAVED_FILTER_PROPERTY, SAVED_FILTER_VALUE, SAVED_FORMAT);

	TEST_THROWS(queries.load(config));
}
TEST_CASE_END

TEST_CASE("Loading a query with no WMI class throws")
{
	const tstring EMPTY_WMI_CLASS;

	Queries queries;

	XML::DocumentPtr config = createDocument(SAVED_TITLE, EMPTY_WMI_CLASS, SAVED_WMI_PROPERTY,
												SAVED_FILTER_PROPERTY, SAVED_FILTER_VALUE, SAVED_FORMAT);

	TEST_THROWS(queries.load(config));
}
TEST_CASE_END

TEST_CASE("Loading a query with no WMI property throws")
{
	const tstring EMPTY_WMI_PROPERTY;

	Queries queries;

	XML::DocumentPtr config = createDocument(SAVED_TITLE, SAVED_WMI_CLASS, EMPTY_WMI_PROPERTY,
												SAVED_FILTER_PROPERTY, SAVED_FILTER_VALUE, SAVED_FORMAT);

	TEST_THROWS(queries.load(config));
}
TEST_CASE_END

TEST_CASE("Loading a query with no format throws")
{
	const tstring EMPTY_FORMAT;

	Queries queries;

	XML::DocumentPtr config = createDocument(SAVED_TITLE, SAVED_WMI_CLASS, SAVED_WMI_PROPERTY,
												SAVED_FILTER_PROPERTY, SAVED_FILTER_VALUE, EMPTY_FORMAT);

	TEST_THROWS(queries.load(config));
}
TEST_CASE_END

TEST_CASE("Loading a set of queries from an XML document replaces the existing set")
{
	XML::DocumentPtr config = createDocument(SAVED_TITLE, SAVED_WMI_CLASS, SAVED_WMI_PROPERTY,
												SAVED_FILTER_PROPERTY, SAVED_FILTER_VALUE, SAVED_FORMAT);
	Queries queries;

	queries.append(makeQuery(TXT("query 1"), TXT("WMI class 1"), TXT("WMI property 1")));
	queries.append(makeQuery(TXT("query 2"), TXT("WMI class 2"), TXT("WMI property 2")));

	queries.load(config);

	TEST_FALSE(queries.isModified());
	TEST_TRUE(queries.size() == 1);
	TEST_TRUE(queries.query(0)->m_title == SAVED_TITLE);
}
TEST_CASE_END

TEST_CASE("A set of queries can be saved to an XML document")
{
	XML::DocumentPtr config = createDocument();
	Queries          queries;

	queries.append(makeQuery(TEST_TITLE, TEST_WMI_CLASS, TEST_WMI_PROPERTY,
								TEST_FILTER_PROPERTY, TEST_FILTER_VALUE, TEST_FORMAT));

	queries.save(config);

	TEST_FALSE(queries.isModified());

	XML::XPathIterator it(TXT("/FarmMonitor/Queries"), config->getRootElement());
	XML::XPathIterator end;

	TEST_TRUE(it != end);

	XML::ElementNodePtr savedQueries = Core::dynamic_ptr_cast<XML::ElementNode>(*it);

	TEST_TRUE(savedQueries->getChildCount() == 1);

	XML::ElementNodePtr query = savedQueries->getChild<XML::ElementNode>(0);

	TEST_TRUE(query->name() == TXT("Query"));

	XML::ElementNodePtr property = query->getChild<XML::ElementNode>(0);
	XML::TextNodePtr value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(property->name() == TXT("Title"));
	TEST_TRUE(value->text() == TEST_TITLE);

	property = query->getChild<XML::ElementNode>(1);
	value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(property->name() == TXT("WMIClass"));
	TEST_TRUE(value->text() == TEST_WMI_CLASS);

	property = query->getChild<XML::ElementNode>(2);
	value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(property->name() == TXT("WMIProperty"));
	TEST_TRUE(value->text() == TEST_WMI_PROPERTY);

	property = query->getChild<XML::ElementNode>(3);
	value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(property->name() == TXT("FilterProperty"));
	TEST_TRUE(value->text() == TEST_FILTER_PROPERTY);

	property = query->getChild<XML::ElementNode>(4);
	value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(property->name() == TXT("FilterValue"));
	TEST_TRUE(value->text() == TEST_FILTER_VALUE);

	property = query->getChild<XML::ElementNode>(5);
	value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(property->name() == TXT("Format"));
	TEST_TRUE(value->text() == TEST_FORMAT);
}
TEST_CASE_END

TEST_CASE("Writing an unmodified set of queries should still write to the XML document")
{
	Queries queries;

	queries.load(createDocument(SAVED_TITLE, SAVED_WMI_CLASS, SAVED_WMI_PROPERTY,
								SAVED_FILTER_PROPERTY, SAVED_FILTER_VALUE, SAVED_FORMAT));

	TEST_FALSE(queries.isModified());

	XML::DocumentPtr config = createDocument();

	queries.save(config);

	XML::XPathIterator it(TXT("/FarmMonitor/Queries/Query"), config->getRootElement());
	XML::XPathIterator end;

	TEST_FALSE(it == end);
	TEST_FALSE(queries.isModified());
}
TEST_CASE_END

}
TEST_SET_END
