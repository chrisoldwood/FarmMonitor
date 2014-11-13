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
		makeQuery(TXT("Win32_OperatingSystem"), TXT("Name")),
	};

	QueryRunner::Results results = QueryRunner::run(localhost, QueryRunner::Queries(queries, queries+ARRAY_SIZE(queries)));
	
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
		makeQuery(TXT("Win32_OperatingSystem"), TXT("Name")),
		makeQuery(TXT("Win32_OperatingSystem"), TXT("LastBootUpTime")),
		makeQuery(TXT("Win32_OperatingSystem"), TXT("FreeVirtualMemory")),
		makeQuery(TXT("Win32_OperatingSystem"), TXT("Name")),
	};

	QueryRunner::Results results = QueryRunner::run(localhost, QueryRunner::Queries(queries, queries+ARRAY_SIZE(queries)));
	
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
		makeQuery(TXT("Win32_LogicalDisk"), TXT("DeviceID")),
	};

	QueryRunner::Results results = QueryRunner::run(localhost, QueryRunner::Queries(queries, queries+ARRAY_SIZE(queries)));
	
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
		makeQuery(TXT("Win32_LogicalDisk"), TXT("DeviceID"), TXT("DeviceID"), nonFirstDevice)
	};

	QueryRunner::Results results = QueryRunner::run(localhost, QueryRunner::Queries(queries, queries+ARRAY_SIZE(queries)));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
	TEST_TRUE(results[0] == nonFirstDevice);
}
TEST_CASE_END

TEST_CASE("a query returns an empty value when the result set is empty")
{
	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("Win32_LogicalDisk"), TXT("DeviceID"), TXT("DeviceID"), TXT("invalid device id"))
	};

	QueryRunner::Results results = QueryRunner::run(localhost, QueryRunner::Queries(queries, queries+ARRAY_SIZE(queries)));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
}
TEST_CASE_END

}
TEST_SET_END
