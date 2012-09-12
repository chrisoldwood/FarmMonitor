////////////////////////////////////////////////////////////////////////////////
//! \file   ToolsTests.cpp
//! \brief  The unit tests for the Tools class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "Tools.hpp"
#include <WCL/AppConfig.hpp>

namespace
{

class FakeAppConfigReader : public WCL::IAppConfigReader
{
public:
	virtual tstring readString(const tstring& sectionName, const tstring& keyName, const tstring& defaultValue) const
	{
		if ( (sectionName == TXT("Tools")) && (keyName == TXT("Count")) )
			return TXT("1");

		if ( (sectionName == TXT("Tools")) && (keyName == TXT("ToolName[0]")) )
			return SAVED_TOOL_NAME;

		if ( (sectionName == TXT("Tools")) && (keyName == TXT("CmdLine[0]")) )
			return SAVED_CMD_LINE;

		return defaultValue;
	}

	virtual void readList(const tstring& /*sectionName*/, const tstring& /*keyName*/, const tstring& /*defaultValue*/, StringArray& /*list*/) const
	{
	}

	static tstring SAVED_TOOL_NAME;
	static tstring SAVED_CMD_LINE;
};

tstring FakeAppConfigReader::SAVED_TOOL_NAME = TXT("saved tool name");
tstring FakeAppConfigReader::SAVED_CMD_LINE  = TXT("saved cmd line");

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

TEST_SET(Tools)
{
	const tstring TEST_TOOL_NAME = TXT("test tool name");
	const tstring TEST_CMD_LINE = TXT("test cmd line");

TEST_CASE("By default the container has no items and is not modified")
{
	Tools tools;

	TEST_TRUE(tools.size() == 0);
	TEST_FALSE(tools.isModified());
}
TEST_CASE_END

TEST_CASE("A set of tools can be loaded from an app config provider")
{
	FakeAppConfigReader reader;
	Tools               tools;

	tools.load(reader);

	TEST_FALSE(tools.isModified());
	TEST_TRUE(tools.size() == 1);
	TEST_TRUE(tools.tool(0)->m_name == FakeAppConfigReader::SAVED_TOOL_NAME);
	TEST_TRUE(tools.tool(0)->m_commandLine == FakeAppConfigReader::SAVED_CMD_LINE);
}
TEST_CASE_END

TEST_CASE("Loading a set of tools replaces the existing set")
{
	FakeAppConfigReader reader;
	Tools               tools;

	tools.append(ToolPtr(new Tool(TXT("tool 1"), TXT("cmd line 1"))));
	tools.append(ToolPtr(new Tool(TXT("tool 2"), TXT("cmd line 2"))));

	tools.load(reader);

	TEST_FALSE(tools.isModified());
	TEST_TRUE(tools.size() == 1);
	TEST_TRUE(tools.tool(0)->m_name == FakeAppConfigReader::SAVED_TOOL_NAME);
	TEST_TRUE(tools.tool(0)->m_commandLine == FakeAppConfigReader::SAVED_CMD_LINE);
}
TEST_CASE_END

TEST_CASE("A set of tools can be saved to an app config provider")
{
	FakeAppConfigWriter writer;
	Tools               tools;

	tools.append(ToolPtr(new Tool(TEST_TOOL_NAME, TEST_CMD_LINE)));

	tools.save(writer);

	TEST_FALSE(tools.isModified());
	TEST_TRUE(writer.m_values.size() == 3);
	TEST_TRUE(writer.m_values[0] == TXT("1"));
	TEST_TRUE(writer.m_values[1] == TEST_TOOL_NAME);
	TEST_TRUE(writer.m_values[2] == TEST_CMD_LINE);
}
TEST_CASE_END

TEST_CASE("An unmodified set of tools shouln't write to the app config provider")
{
	FakeAppConfigWriter writer;
	Tools               tools;

	tools.save(writer);

	TEST_TRUE(writer.m_values.size() == 0);
	TEST_FALSE(tools.isModified());
}
TEST_CASE_END

TEST_CASE("Appending a tool increases the size and marks the container as modified")
{
	Tools tools;

	tools.append(ToolPtr(new Tool(TEST_TOOL_NAME, TEST_CMD_LINE)));

	TEST_TRUE(tools.size() == 1);
	TEST_TRUE(tools.isModified());
}
TEST_CASE_END

TEST_CASE("Replacing a tool upates the existing item and marks the container as modified")
{
	FakeAppConfigReader reader;
	Tools               tools;

	tools.load(reader);

	tools.replace(0, ToolPtr(new Tool(TEST_TOOL_NAME, TEST_CMD_LINE)));

	TEST_TRUE(tools.isModified());
	TEST_TRUE(tools.size() == 1);
	TEST_TRUE(tools.tool(0)->m_name == TEST_TOOL_NAME);
	TEST_TRUE(tools.tool(0)->m_commandLine == TEST_CMD_LINE);
}
TEST_CASE_END

TEST_CASE("Removing a tool decreses the size and marks the container as modified")
{
	FakeAppConfigReader reader;
	Tools               tools;

	tools.load(reader);
	tools.remove(0);

	TEST_TRUE(tools.isModified());
	TEST_TRUE(tools.size() == 0);
}
TEST_CASE_END

TEST_CASE("A deep copy of the container replaces the existing contents and becomes unmodified")
{
	Tools lhs;

	lhs.append(ToolPtr(new Tool(TEST_TOOL_NAME, TEST_CMD_LINE)));

	Tools rhs;

	rhs.append(ToolPtr(new Tool(TXT("to be replaced 1"), TXT("to be replaced 1"))));
	rhs.append(ToolPtr(new Tool(TXT("to be replaced 2"), TXT("to be replaced 2"))));
	rhs.deepCopy(lhs);

	lhs.remove(0);

	TEST_FALSE(rhs.isModified());
	TEST_TRUE(rhs.size() == 1);
}
TEST_CASE_END

TEST_CASE("Replacing the container contents also becomes modified")
{
	Tools lhs;

	lhs.append(ToolPtr(new Tool(TEST_TOOL_NAME, TEST_CMD_LINE)));

	Tools rhs;

	rhs.append(ToolPtr(new Tool(TXT("to be replaced 1"), TXT("to be replaced 1"))));
	rhs.append(ToolPtr(new Tool(TXT("to be replaced 2"), TXT("to be replaced 2"))));
	rhs.replaceAll(lhs);

	TEST_TRUE(rhs.isModified());
	TEST_TRUE(rhs.size() == 1);
}
TEST_CASE_END

}
TEST_SET_END
