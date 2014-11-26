////////////////////////////////////////////////////////////////////////////////
//! \file   QueryRunnerTests.cpp
//! \brief  The unit tests for the QueryRunner class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <WCL/AutoCom.hpp>
#include "QueryRunner.hpp"
#include <WMI/Connection.hpp>
#include <WMI/Win32_OperatingSystem.hpp>
#include <WMI/Win32_LogicalDisk.hpp>

TEST_SET(QueryRunner)
{
	WCL::AutoCom autoCom(COINIT_APARTMENTTHREADED);

	const tstring localhost = TXT("localhost");

TEST_CASE("a query returns the value from the only item of a single item result set")
{
	tstring expected;

{
	WMI::Connection connection(localhost);

	const WMI::Win32_OperatingSystem::Iterator it = WMI::Win32_OperatingSystem::select(connection);

	expected = it->Name();
}

	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_OperatingSystem"), TXT("Name")),
	};

	QueryRunner::Results results = QueryRunner::run(localhost, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
	TEST_TRUE(results[0] == expected);
}
TEST_CASE_END

TEST_CASE("multiple queries returns one value per query in the order defined")
{
	tstring expected;

{
	WMI::Connection connection(localhost);

	const WMI::Win32_OperatingSystem::Iterator it = WMI::Win32_OperatingSystem::select(connection);

	expected = it->Name();
}

	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_OperatingSystem"), TXT("Name")),
		makeQuery(TXT("title"), TXT("Win32_OperatingSystem"), TXT("LastBootUpTime")),
		makeQuery(TXT("title"), TXT("Win32_OperatingSystem"), TXT("FreeVirtualMemory")),
		makeQuery(TXT("title"), TXT("Win32_OperatingSystem"), TXT("Name")),
	};

	QueryRunner::Results results = QueryRunner::run(localhost, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
	TEST_TRUE(results[0] == expected);
	TEST_TRUE(results[3] == expected);
}
TEST_CASE_END

TEST_CASE("a query returns the value from the first item of the result set")
{
	tstring expected;

{
	WMI::Connection connection(localhost);

	const WMI::Win32_LogicalDisk::Iterator it = WMI::Win32_LogicalDisk::select(connection);

	expected = it->DeviceID();
}

	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_LogicalDisk"), TXT("DeviceID")),
	};

	QueryRunner::Results results = QueryRunner::run(localhost, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
	TEST_TRUE(results[0] == expected);
}
TEST_CASE_END

TEST_CASE("a query returns the value from the matching item when the result set is filtered on a property")
{
	std::vector<tstring> devices;

{
	WMI::Connection connection(localhost);

	WMI::Win32_LogicalDisk::Iterator it = WMI::Win32_LogicalDisk::select(connection);
	WMI::Win32_LogicalDisk::Iterator end;

	for (;it != end; ++it)
		devices.push_back(it->DeviceID());
}

	TEST_TRUE(devices.size() > 0);

	tstring nonFirstDevice = devices.back();

	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_LogicalDisk"), TXT("DeviceID"), TXT("DeviceID"), nonFirstDevice, Query::DEFAULT_FORMAT)
	};

	QueryRunner::Results results = QueryRunner::run(localhost, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
	TEST_TRUE(results[0] == nonFirstDevice);
}
TEST_CASE_END

TEST_CASE("a query returns an empty value when the result set is empty")
{
	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_LogicalDisk"), TXT("DeviceID"), TXT("DeviceID"), TXT("invalid device id"), Query::DEFAULT_FORMAT)
	};

	QueryRunner::Results results = QueryRunner::run(localhost, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
}
TEST_CASE_END

TEST_CASE("the default format for a value is to convert it to a string")
{
	const tstring NO_FORMAT = TXT("");

	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_OperatingSystem"), TXT("OSType"), TXT(""), TXT(""), NO_FORMAT),
	};

	QueryRunner::Results results = QueryRunner::run(localhost, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == 1);
	TEST_TRUE(results[0] != TXT(""));
}
TEST_CASE_END

TEST_CASE("an unsupported format returns the value as an error message")
{
	const tstring INVALID_FORMAT = TXT("%z");

	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_OperatingSystem"), TXT("OSType"), TXT(""), TXT(""), INVALID_FORMAT),
	};

	QueryRunner::Results results = QueryRunner::run(localhost, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == 1);
	TEST_TRUE(results[0] == TXT("<unknown format>"));
}
TEST_CASE_END

TEST_CASE("'%s' explicitly formats the value as a simple string")
{
	tstring expected;

{
	WMI::Connection connection(localhost);

	const WMI::Win32_OperatingSystem::Iterator it = WMI::Win32_OperatingSystem::select(connection);

	expected = it->Name();
}

	const tstring STRING_FORMAT = TXT("%s");

	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_OperatingSystem"), TXT("Name"), TXT(""), TXT(""), STRING_FORMAT),
	};

	QueryRunner::Results results = QueryRunner::run(localhost, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == 1);
	TEST_TRUE(results[0] == expected);
}
TEST_CASE_END

TEST_CASE("'%t' formats the value as a date/time")
{
	const tstring DATETIME_FORMAT = TXT("%t");

	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_OperatingSystem"), TXT("LastBootUpTime"), TXT(""), TXT(""), DATETIME_FORMAT),
	};

	QueryRunner::Results results = QueryRunner::run(localhost, queries, queries+ARRAY_SIZE(queries));

	TEST_TRUE(results.size() == 1);
	TEST_TRUE(results[0][2]  == TXT('/'));
	TEST_TRUE(results[0][5]  == TXT('/'));
	TEST_TRUE(results[0][10] == TXT(' '));
	TEST_TRUE(results[0][13] == TXT(':'));
	TEST_TRUE(results[0][16] == TXT(':'));
}
TEST_CASE_END

}
TEST_SET_END
