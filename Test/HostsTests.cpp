////////////////////////////////////////////////////////////////////////////////
//! \file   HostsTests.cpp
//! \brief  The unit tests for the Hosts class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "Hosts.hpp"
#include <WCL/AppConfig.hpp>
#include <XML/Document.hpp>
#include <XML/XPathIterator.hpp>

namespace
{

static tstring SAVED_HOST = TXT("saved host");;

class FakeAppConfigReader : public WCL::IAppConfigReader
{
public:
	virtual tstring readString(const tstring& sectionName, const tstring& keyName, const tstring& defaultValue) const
	{
		if ( (sectionName == TXT("Hosts")) && (keyName == TXT("Count")) )
			return TXT("1");

		if ( (sectionName == TXT("Hosts")) && (keyName == TXT("Host[0]")) )
			return SAVED_HOST;

		return defaultValue;
	}

	virtual void readStringList(const tstring& /*sectionName*/, const tstring& /*keyName*/, const tstring& /*defaultValue*/, StringArray& /*list*/) const
	{
	}

};

class FakeAppConfigWriter : public WCL::IAppConfigWriter
{
public:
	virtual void writeString(const tstring& /*sectionName*/, const tstring& /*keyName*/, const tstring& value)
	{
		m_values.push_back(value);
	}

	virtual void writeStringList(const tstring& /*sectionName*/, const tstring& /*keyName*/, const StringArray& /*list*/)
	{
	}

	virtual void deleteSection(const tstring& sectionName)
	{
		m_deletions.push_back(sectionName);
	}

	std::vector<tstring>	m_values;
	std::vector<tstring>	m_deletions;
};

XML::DocumentPtr createDocument()
{
	return XML::makeDocument(XML::makeElement
	(
		TXT("FarmMonitor"), XML::makeElement
		(
			TXT("Hosts")
		)
	));
}

XML::DocumentPtr createDocument(const tstring& host)
{
	return XML::makeDocument(XML::makeElement
	(
		TXT("FarmMonitor"), XML::makeElement
		(
			TXT("Hosts"), XML::makeElement
			(
				TXT("Host"), XML::makeAttribute(TXT("Name"), host)
			)
		)
	));
}

}

TEST_SET(Hosts)
{
	const tstring TEST_HOST = TXT("test host");
	const tstring TEST_HOST_2 = TXT("another host");

TEST_CASE("By default the container has no items and is not modified")
{
	Hosts hosts;

	TEST_TRUE(hosts.size() == 0);
	TEST_FALSE(hosts.isModified());
}
TEST_CASE_END

TEST_CASE("A set of hosts can be loaded from an app config provider")
{
	FakeAppConfigReader reader;
	Hosts               hosts;

	hosts.load(reader);

	TEST_FALSE(hosts.isModified());
	TEST_TRUE(hosts.size() == 1);
	TEST_TRUE(hosts.name(0) == SAVED_HOST);
}
TEST_CASE_END

TEST_CASE("A set of hosts can be loaded from an XML document")
{
	XML::DocumentPtr config = createDocument(SAVED_HOST);
	Hosts            hosts;

	hosts.load(config);

	TEST_FALSE(hosts.isModified());
	TEST_TRUE(hosts.size() == 1);
	TEST_TRUE(hosts.name(0) == SAVED_HOST);
}
TEST_CASE_END

TEST_CASE("Loading a set of hosts replaces the existing set")
{
	FakeAppConfigReader reader;
	Hosts               hosts;

	hosts.add(TXT("host 1"));
	hosts.add(TXT("host 2"));

	hosts.load(reader);

	TEST_FALSE(hosts.isModified());
	TEST_TRUE(hosts.size() == 1);
	TEST_TRUE(hosts.name(0) == SAVED_HOST);
}
TEST_CASE_END

TEST_CASE("Loading a set of hosts from an XML document replaces the existing set")
{
	XML::DocumentPtr config = XML::makeDocument(XML::makeElement
	(
		TXT("FarmMonitor"), XML::makeElement
		(
			TXT("Hosts"), XML::makeElement
			(
				TXT("Host"), XML::makeAttribute(TXT("Name"), SAVED_HOST)
			)
		)
	));

	Hosts hosts;

	hosts.add(TXT("host 1"));
	hosts.add(TXT("host 2"));

	hosts.load(config);

	TEST_FALSE(hosts.isModified());
	TEST_TRUE(hosts.size() == 1);
	TEST_TRUE(hosts.name(0) == SAVED_HOST);
}
TEST_CASE_END

TEST_CASE("A set of hosts can be saved to an app config provider")
{
	FakeAppConfigWriter writer;
	Hosts               hosts;

	hosts.add(TEST_HOST);

	hosts.save(writer);

	TEST_TRUE(writer.m_values.size() == 0);
	TEST_TRUE(writer.m_deletions.size() == 1);
	TEST_TRUE(writer.m_deletions[0] == TXT("Hosts"));
}
TEST_CASE_END

TEST_CASE("A set of hosts can be saved to an XML document")
{
	XML::DocumentPtr config = createDocument();
	Hosts            hosts;

	hosts.add(TEST_HOST);
	hosts.add(TEST_HOST_2);

	hosts.save(config);

	TEST_FALSE(hosts.isModified());

	XML::XPathIterator it(TXT("/FarmMonitor/Hosts"), config->getRootElement());
	XML::XPathIterator end;

	TEST_TRUE(it != end);

	XML::ElementNodePtr savedHosts = Core::dynamic_ptr_cast<XML::ElementNode>(*it);

	TEST_TRUE(savedHosts->getChildCount() == 2);

	XML::ElementNodePtr host = savedHosts->getChild<XML::ElementNode>(0);

	TEST_TRUE(host->name() == TXT("Host"));
	TEST_TRUE(host->getAttributeValue(TXT("Name")) == TEST_HOST);

	host = savedHosts->getChild<XML::ElementNode>(1);

	TEST_TRUE(host->name() == TXT("Host"));
	TEST_TRUE(host->getAttributeValue(TXT("Name")) == TEST_HOST_2);
}
TEST_CASE_END

TEST_CASE("Writing an unmodified set of hosts should still write to the app config provider")
{
	FakeAppConfigReader reader;
	Hosts               hosts;

	hosts.load(reader);

	TEST_FALSE(hosts.isModified());

	FakeAppConfigWriter writer;

	hosts.save(writer);

	TEST_TRUE(writer.m_values.size() == 0);
	TEST_TRUE(writer.m_deletions.size() == 1);
}
TEST_CASE_END

TEST_CASE("Writing an unmodified set of hosts should still write to the XML document")
{
	Hosts hosts;

	hosts.load(createDocument(SAVED_HOST));

	TEST_FALSE(hosts.isModified());

	XML::DocumentPtr config = createDocument();

	hosts.save(config);

	XML::XPathIterator it(TXT("/FarmMonitor/Hosts/Host"), config->getRootElement());
	XML::XPathIterator end;

	TEST_FALSE(it == end);
	TEST_FALSE(hosts.isModified());
}
TEST_CASE_END

TEST_CASE("Adding a host increases the size and marks the container as modified")
{
	Hosts hosts;

	hosts.add(TEST_HOST);

	TEST_TRUE(hosts.size() == 1);
	TEST_TRUE(hosts.isModified());
}
TEST_CASE_END

TEST_CASE("Removing a host decreses the size and marks the container as modified")
{
	XML::DocumentPtr config = createDocument(SAVED_HOST);
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

	XML::DocumentPtr config = createDocument(RENAMED_HOST);
	Hosts            hosts;

	hosts.load(config);
	hosts.rename(0, RENAMED_HOST);

	TEST_TRUE(hosts.size() == 1);
	TEST_TRUE(hosts.name(0) == RENAMED_HOST);
	TEST_TRUE(hosts.isModified());
}
TEST_CASE_END

}
TEST_SET_END
