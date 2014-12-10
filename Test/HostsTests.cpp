////////////////////////////////////////////////////////////////////////////////
//! \file   HostsTests.cpp
//! \brief  The unit tests for the Hosts class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "Hosts.hpp"
#include <XML/Document.hpp>
#include <XML/XPathIterator.hpp>
#include <XML/TextNode.hpp>
#include "DataDocument.hpp"

namespace
{

static tstring SAVED_HOST = TXT("saved host");
static tstring SAVED_ENVIRONMENT = TXT("saved environment");
static tstring SAVED_DESCRIPTION = TXT("saved description");
static bool    SAVED_MONITOR = true;
static tstring SAVED_LOGON = TXT("saved logon");

XML::DocumentPtr createDocument()
{
	return createDocumentShell();
}

XML::DocumentPtr createDocument(const tstring& host, const tstring& environment, const tstring& description,
								bool monitor, const tstring& logon)
{
	XML::DocumentPtr    document(createDocumentShell());
	XML::XPathIterator  it(TXT("/FarmMonitor/Hosts"), document);
	XML::ElementNodePtr hosts(Core::dynamic_ptr_cast<XML::ElementNode>(*it));

	XML::NodePtr properties[] =
	{
		XML::makeElement(TXT("Name"), XML::makeText(host)),
		XML::makeElement(TXT("Environment"), XML::makeText(environment)),
		XML::makeElement(TXT("Description"), XML::makeText(description)),
		XML::makeElement(TXT("Monitor"), XML::makeText(Core::format(monitor))),
		XML::makeElement(TXT("Logon"), XML::makeText(logon)),
	};

	hosts->appendChild(XML::makeElement
	(
		TXT("Host"), properties, properties+ARRAY_SIZE(properties)
	));

	return document;
}

}

TEST_SET(Hosts)
{
	const tstring TEST_HOST = TXT("test host");
	const tstring TEST_ENV = TXT("test environment");
	const tstring TEST_DESC = TXT("test description");
	const bool    TEST_MON = true;
	const tstring TEST_LOGON = TXT("test logon");

	const tstring TEST_HOST_2 = TXT("another host");
	const tstring TEST_ENV_2 = TXT("another environment");
	const tstring TEST_DESC_2 = TXT("another description");
	const bool    TEST_MON_2 = false;
	const tstring TEST_LOGON_2 = TXT("another logon");

TEST_CASE("By default the container has no items and is not modified")
{
	Hosts hosts;

	TEST_TRUE(hosts.size() == 0);
	TEST_FALSE(hosts.isModified());
}
TEST_CASE_END

TEST_CASE("A set of hosts can be loaded from an XML document")
{
	XML::DocumentPtr config = createDocument(SAVED_HOST, SAVED_ENVIRONMENT, SAVED_DESCRIPTION,
												SAVED_MONITOR, SAVED_LOGON);
	Hosts            hosts;

	hosts.load(config);

	TEST_FALSE(hosts.isModified());
	TEST_TRUE(hosts.size() == 1);
	TEST_TRUE(hosts.host(0)->m_name == SAVED_HOST);
	TEST_TRUE(hosts.host(0)->m_environment == SAVED_ENVIRONMENT);
	TEST_TRUE(hosts.host(0)->m_description == SAVED_DESCRIPTION);
	TEST_TRUE(hosts.host(0)->m_monitor == SAVED_MONITOR);
	TEST_TRUE(hosts.host(0)->m_logon == SAVED_LOGON);
}
TEST_CASE_END

TEST_CASE("Loading a set of hosts from an XML document replaces the existing set")
{
	Hosts hosts;

	hosts.add(makeHost(TXT("host 1")));
	hosts.add(makeHost(TXT("host 2")));

	XML::DocumentPtr config = createDocument(SAVED_HOST, SAVED_ENVIRONMENT, SAVED_DESCRIPTION,
												SAVED_MONITOR, SAVED_LOGON);

	hosts.load(config);

	TEST_FALSE(hosts.isModified());
	TEST_TRUE(hosts.size() == 1);
	TEST_TRUE(hosts.host(0)->m_name == SAVED_HOST);
	TEST_TRUE(hosts.host(0)->m_environment == SAVED_ENVIRONMENT);
	TEST_TRUE(hosts.host(0)->m_description == SAVED_DESCRIPTION);
	TEST_TRUE(hosts.host(0)->m_monitor == SAVED_MONITOR);
	TEST_TRUE(hosts.host(0)->m_logon == SAVED_LOGON);
}
TEST_CASE_END

TEST_CASE("Loading a set of hosts throws when a duplicate name is encountered")
{
	XML::DocumentPtr config = createDocument();

	XML::XPathIterator  it(TXT("/FarmMonitor/Hosts"), config);
	XML::ElementNodePtr parent(Core::dynamic_ptr_cast<XML::ElementNode>(*it));

	{
		XML::NodePtr properties[] =
		{
			XML::makeElement(TXT("Name"),        XML::makeText(TXT("duplicate"))),
			XML::makeElement(TXT("Environment"), XML::makeText(TXT(""))),
			XML::makeElement(TXT("Description"), XML::makeText(TXT(""))),
		};

		parent->appendChild(XML::makeElement(TXT("Host"), properties, properties+ARRAY_SIZE(properties)));
	}

	{
		XML::NodePtr properties[] =
		{
			XML::makeElement(TXT("Name"),        XML::makeText(TXT("duplicate"))),
			XML::makeElement(TXT("Environment"), XML::makeText(TXT(""))),
			XML::makeElement(TXT("Description"), XML::makeText(TXT(""))),
		};

		parent->appendChild(XML::makeElement(TXT("Host"), properties, properties+ARRAY_SIZE(properties)));
	}

	Hosts hosts;

	TEST_THROWS(hosts.load(config));
}
TEST_CASE_END

TEST_CASE("A set of hosts can be saved to an XML document")
{
	XML::DocumentPtr config = createDocument();
	Hosts            hosts;

	hosts.add(makeHost(TEST_HOST,   TEST_ENV,   TEST_DESC,   TEST_MON,   TEST_LOGON));
	hosts.add(makeHost(TEST_HOST_2, TEST_ENV_2, TEST_DESC_2, TEST_MON_2, TEST_LOGON_2));

	hosts.save(config);

	TEST_FALSE(hosts.isModified());

	XML::XPathIterator it(TXT("/FarmMonitor/Hosts"), config->getRootElement());
	XML::XPathIterator end;

	TEST_TRUE(it != end);

	XML::ElementNodePtr savedHosts = Core::dynamic_ptr_cast<XML::ElementNode>(*it);

	TEST_TRUE(savedHosts->getChildCount() == 2);

	XML::ElementNodePtr host = savedHosts->getChild<XML::ElementNode>(0);

	TEST_TRUE(host->name() == TXT("Host"));

	XML::ElementNodePtr property = host->getChild<XML::ElementNode>(0);

	TEST_TRUE(property->name() == TXT("Name"));

	XML::TextNodePtr value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(value->text() == TEST_HOST);

	host = savedHosts->getChild<XML::ElementNode>(1);

	TEST_TRUE(host->name() == TXT("Host"));

	property = host->getChild<XML::ElementNode>(0);
	value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(property->name() == TXT("Name"));
	TEST_TRUE(value->text() == TEST_HOST_2);

	property = host->getChild<XML::ElementNode>(1);
	value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(property->name() == TXT("Environment"));
	TEST_TRUE(value->text() == TEST_ENV_2);

	property = host->getChild<XML::ElementNode>(2);
	value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(property->name() == TXT("Description"));
	TEST_TRUE(value->text() == TEST_DESC_2);

	property = host->getChild<XML::ElementNode>(3);
	value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(property->name() == TXT("Monitor"));
	TEST_TRUE(value->text() == Core::format<bool>(TEST_MON_2));

	property = host->getChild<XML::ElementNode>(4);
	value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(property->name() == TXT("Logon"));
	TEST_TRUE(value->text() == TEST_LOGON_2);
}
TEST_CASE_END

TEST_CASE("Writing an unmodified set of hosts should still write to the XML document")
{
	Hosts hosts;

	hosts.load(createDocument(SAVED_HOST, SAVED_ENVIRONMENT, SAVED_DESCRIPTION,
								SAVED_MONITOR, SAVED_LOGON));

	TEST_FALSE(hosts.isModified());

	XML::DocumentPtr config = createDocument();

	hosts.save(config);

	XML::XPathIterator it(TXT("/FarmMonitor/Hosts/Host"), config->getRootElement());
	XML::XPathIterator end;

	TEST_FALSE(it == end);
	TEST_FALSE(hosts.isModified());
}
TEST_CASE_END

TEST_CASE("Loading a host with no name throws")
{
	const tstring EMPTY_NAME;

	Hosts hosts;

	XML::DocumentPtr config = createDocument(EMPTY_NAME, SAVED_ENVIRONMENT, SAVED_DESCRIPTION,
												SAVED_MONITOR, SAVED_LOGON);

	TEST_THROWS(hosts.load(config));
}
TEST_CASE_END

TEST_CASE("Adding a host increases the size and marks the container as modified")
{
	Hosts hosts;

	hosts.add(makeHost(TEST_HOST));

	TEST_TRUE(hosts.size() == 1);
	TEST_TRUE(hosts.isModified());
}
TEST_CASE_END

TEST_CASE("Removing a host decreses the size and marks the container as modified")
{
	XML::DocumentPtr config = createDocument(SAVED_HOST, SAVED_ENVIRONMENT, SAVED_DESCRIPTION,
												SAVED_MONITOR, SAVED_LOGON);
	Hosts            hosts;

	hosts.load(config);
	hosts.remove(0);

	TEST_TRUE(hosts.isModified());
	TEST_TRUE(hosts.size() == 0);
}
TEST_CASE_END

TEST_CASE("Renaming a host replaces the item and marks the container as modified")
{
	const tstring RENAMED_HOST = TXT("renamed host");
	const tstring RENAMED_ENVIRONMENT = TXT("renamed environment");
	const tstring RENAMED_DESCRIPTION = TXT("renamed description");
	const bool    RENAMED_MONITOR = !SAVED_MONITOR;
	const tstring RENAMED_LOGON = TXT("renamed logon");

	XML::DocumentPtr config = createDocument(SAVED_HOST, SAVED_ENVIRONMENT, SAVED_DESCRIPTION,
												SAVED_MONITOR, SAVED_LOGON);
	Hosts            hosts;

	hosts.load(config);
	hosts.replace(0, makeHost(RENAMED_HOST, RENAMED_ENVIRONMENT, RENAMED_DESCRIPTION,
								RENAMED_MONITOR, RENAMED_LOGON));

	TEST_TRUE(hosts.size() == 1);
	TEST_TRUE(hosts.host(0)->m_name == RENAMED_HOST);
	TEST_TRUE(hosts.host(0)->m_environment == RENAMED_ENVIRONMENT);
	TEST_TRUE(hosts.host(0)->m_description == RENAMED_DESCRIPTION);
	TEST_TRUE(hosts.host(0)->m_monitor == RENAMED_MONITOR);
	TEST_TRUE(hosts.host(0)->m_logon == RENAMED_LOGON);
	TEST_TRUE(hosts.isModified());
}
TEST_CASE_END

TEST_CASE("Two hosts can be swapped by index")
{
	Hosts hosts;

	hosts.add(makeHost(TXT("1st host")));
	hosts.add(makeHost(TXT("2nd host")));
	hosts.add(makeHost(TXT("3rd host")));

	XML::DocumentPtr config = createDocument();

	hosts.save(config);

	hosts.swap(0, 2);

	TEST_TRUE(hosts.host(0)->m_name == TXT("3rd host"));

	TEST_TRUE(hosts.host(2)->m_name == TXT("1st host"));

	TEST_TRUE(hosts.isModified());
}
TEST_CASE_END

}
TEST_SET_END
