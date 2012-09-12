////////////////////////////////////////////////////////////////////////////////
//! \file   HostsTests.cpp
//! \brief  The unit tests for the Hosts class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "Hosts.hpp"
#include <WCL/AppConfig.hpp>

namespace
{

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

	virtual void readList(const tstring& /*sectionName*/, const tstring& /*keyName*/, const tstring& /*defaultValue*/, StringArray& /*list*/) const
	{
	}

	static tstring SAVED_HOST;
};

tstring FakeAppConfigReader::SAVED_HOST = TXT("saved host");

class FakeAppConfigWriter : public WCL::IAppConfigWriter
{
public:
	virtual void writeString(const tstring& /*sectionName*/, const tstring& /*keyName*/, const tstring& value)
	{
		m_values.push_back(value);
	}

	virtual void writeList(const tstring& /*sectionName*/, const tstring& /*keyName*/, const StringArray& /*list*/)
	{
	}

	virtual void deleteSection(const tstring& /*sectionName*/)
	{
	}

	std::vector<tstring>	m_values;
};

}

TEST_SET(Hosts)
{
	const tstring TEST_HOST = TXT("unit test");

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
	TEST_TRUE(hosts.name(0) == FakeAppConfigReader::SAVED_HOST);
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
	TEST_TRUE(hosts.name(0) == FakeAppConfigReader::SAVED_HOST);
}
TEST_CASE_END

TEST_CASE("A set of hosts can be saved to an app config provider")
{
	FakeAppConfigWriter writer;
	Hosts               hosts;

	hosts.add(TEST_HOST);

	hosts.save(writer);

	TEST_FALSE(hosts.isModified());
	TEST_TRUE(writer.m_values.size() == 2);
	TEST_TRUE(writer.m_values[0] == TXT("1"));
	TEST_TRUE(writer.m_values[1] == TEST_HOST);
}
TEST_CASE_END

TEST_CASE("An unmodified set of hosts shouln't write to the app config provider")
{
	FakeAppConfigWriter writer;
	Hosts               hosts;

	hosts.save(writer);

	TEST_TRUE(writer.m_values.size() == 0);
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
	FakeAppConfigReader reader;
	Hosts               hosts;

	hosts.load(reader);
	hosts.remove(0);

	TEST_TRUE(hosts.isModified());
	TEST_TRUE(hosts.size() == 0);
}
TEST_CASE_END

}
TEST_SET_END
