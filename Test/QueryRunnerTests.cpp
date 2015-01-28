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
#include "Formats.hpp"

TEST_SET(QueryRunner)
{
	WCL::AutoCom autoCom(COINIT_APARTMENTTHREADED);

	const tstring   localhost = TXT("localhost");
	WMI::Connection connection(localhost);

TEST_CASE("a query returns the value from the only item of a single item result set")
{
	tstring expected;

{
	const WMI::Win32_OperatingSystem::Iterator it = WMI::Win32_OperatingSystem::select(connection);

	expected = it->Name();
}

	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_OperatingSystem"), TXT("Name")),
	};

	QueryRunner::Results results = QueryRunner::run(connection, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
	TEST_TRUE(results[0] == expected);
}
TEST_CASE_END

TEST_CASE("multiple queries returns one value per query in the order defined")
{
	tstring expected;

{
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

	QueryRunner::Results results = QueryRunner::run(connection, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
	TEST_TRUE(results[0] == expected);
	TEST_TRUE(results[3] == expected);
}
TEST_CASE_END

TEST_CASE("a query returns the value from the first item of the result set")
{
	tstring expected;

{
	const WMI::Win32_LogicalDisk::Iterator it = WMI::Win32_LogicalDisk::select(connection);

	expected = it->DeviceID();
}

	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_LogicalDisk"), TXT("DeviceID")),
	};

	QueryRunner::Results results = QueryRunner::run(connection, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
	TEST_TRUE(results[0] == expected);
}
TEST_CASE_END

TEST_CASE("a query returns the value from the matching item when the result set is filtered on a property")
{
	std::vector<tstring> devices;

{
	WMI::Win32_LogicalDisk::Iterator it = WMI::Win32_LogicalDisk::select(connection);
	WMI::Win32_LogicalDisk::Iterator end;

	for (;it != end; ++it)
		devices.push_back(it->DeviceID());
}

	TEST_TRUE(devices.size() > 0);

	tstring nonFirstDevice = devices.back();

	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_LogicalDisk"), TXT("DeviceID"), TXT("DeviceID"), nonFirstDevice, Formats::DEFAULT)
	};

	QueryRunner::Results results = QueryRunner::run(connection, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
	TEST_TRUE(results[0] == nonFirstDevice);
}
TEST_CASE_END

TEST_CASE("a query returns an empty value when the result set is empty")
{
	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_LogicalDisk"), TXT("DeviceID"), TXT("DeviceID"), TXT("invalid device id"), Formats::DEFAULT)
	};

	QueryRunner::Results results = QueryRunner::run(connection, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
}
TEST_CASE_END

TEST_CASE("a query returns an error when the property does not exist")
{
	ConstQueryPtr queries[] =
	{
		makeQuery(TXT("title"), TXT("Win32_OperatingSystem"), TXT("Unknown Property Name")),
	};

	QueryRunner::Results results = QueryRunner::run(connection, queries, queries+ARRAY_SIZE(queries));
	
	TEST_TRUE(results.size() == ARRAY_SIZE(queries));
	TEST_TRUE(results[0] == TXT("#N/A"));
}
TEST_CASE_END

TEST_CASE("an unsupported format returns the value as an error message")
{
	const tstring      INVALID_FORMAT = TXT("invalid format");
	const WCL::Variant value(L"test value");

	const tstring actual = QueryRunner::formatValue(value, INVALID_FORMAT);

	TEST_TRUE(actual == TXT("<unknown format>"));
}
TEST_CASE_END

TEST_CASE("'%s' explicitly formats the value as a simple string")
{
	const tstring      expected = TXT("test value");
	const WCL::Variant value(T2W(expected));

	const tstring actual = QueryRunner::formatValue(value, Formats::STRING);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("'%t' formats the value as a date/time")
{
	const WCL::Variant value(L"20010203040506.375000+123");

	const tstring actual = QueryRunner::formatValue(value, Formats::DATETIME);

	TEST_TRUE(actual == TXT("03/02/2001 04:05:06 +123"));
}
TEST_CASE_END

TEST_CASE("'%t' formats an error when the value is not a WMI datetime")
{
	const WCL::Variant value(L"invalid datetime");

	const tstring actual = QueryRunner::formatValue(value, Formats::DATETIME);

	TEST_TRUE(actual == TXT("<non-datetime value>"));
}
TEST_CASE_END

TEST_CASE("'B' formats the value as a whole number of bytes")
{
	const WCL::Variant value((uint32)12345);

	const tstring actual = QueryRunner::formatValue(value, Formats::BYTES);

	TEST_TRUE(actual == TXT("12,345 B"));
}
TEST_CASE_END

TEST_CASE("'KB' formats the value as a whole number of kilobytes")
{
	const WCL::Variant value((uint32)4u*1024u);

	const tstring actual = QueryRunner::formatValue(value, Formats::K_BYTES);

	TEST_TRUE(actual == TXT("4 KB"));
}
TEST_CASE_END

TEST_CASE("'MB' formats the value as a whole number of megabytes")
{
	const WCL::Variant value((uint32)8u*1024u*1024u);

	const tstring actual = QueryRunner::formatValue(value, Formats::M_BYTES);

	TEST_TRUE(actual == TXT("8 MB"));
}
TEST_CASE_END

TEST_CASE("'GB' formats the value as a whole number of gigabytes")
{
	const WCL::Variant value((uint64)16u*1024u*1024u*1024u);

	const tstring actual = QueryRunner::formatValue(value, Formats::G_BYTES);

	TEST_TRUE(actual == TXT("16 GB"));
}
TEST_CASE_END

TEST_CASE("'KB2MB' formats the value in KB as a whole number of megabytes")
{
	const WCL::Variant value((uint32)8u*1024u);

	const tstring actual = QueryRunner::formatValue(value, Formats::KB_AS_MB);

	TEST_TRUE(actual == TXT("8 MB"));
}
TEST_CASE_END

TEST_CASE("'KB2GB' formats the value in KB as a whole number of gigabytes")
{
	const WCL::Variant value((uint32)16u*1024u*1024u);

	const tstring actual = QueryRunner::formatValue(value, Formats::KB_AS_GB);

	TEST_TRUE(actual == TXT("16 GB"));
}
TEST_CASE_END

TEST_CASE("An error is returned if the value cannot be coerced")
{
	const tstring      BYTES_FORMAT = TXT("B");
	const WCL::Variant value(L"text value");

	const tstring actual = QueryRunner::formatValue(value, Formats::BYTES);

	TEST_TRUE(actual == TXT("#ERR"));
}
TEST_CASE_END

}
TEST_SET_END
