////////////////////////////////////////////////////////////////////////////////
//! \file   ToolsTests.cpp
//! \brief  The unit tests for the Tools class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "Tools.hpp"
#include <WCL/AppConfig.hpp>
#include <XML/XPathIterator.hpp>
#include <XML/TextNode.hpp>

namespace
{

tstring SAVED_TOOL_NAME = TXT("saved tool name");
tstring SAVED_CMD_LINE  = TXT("saved cmd line");

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
			TXT("Tools")
		)
	));
}

XML::DocumentPtr createDocument(const tstring& name, const tstring& commandLine)
{
	XML::ElementNodePtr nameNode = XML::makeElement(TXT("Name"), XML::makeText(name));
	XML::ElementNodePtr cmdLineNode = XML::makeElement(TXT("CommandLine"), XML::makeText(commandLine));
	XML::ElementNodePtr tool = XML::makeElement(TXT("Tool"));

	tool->appendChild(nameNode);
	tool->appendChild(cmdLineNode);

	return XML::makeDocument(XML::makeElement
	(
		TXT("FarmMonitor"), XML::makeElement
		(
			TXT("Tools"), tool
		)
	));
}

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
	TEST_TRUE(tools.tool(0)->m_name == SAVED_TOOL_NAME);
	TEST_TRUE(tools.tool(0)->m_commandLine == SAVED_CMD_LINE);
}
TEST_CASE_END

TEST_CASE("A set of tools can be loaded from an XML document")
{
	XML::DocumentPtr config = createDocument(SAVED_TOOL_NAME, SAVED_CMD_LINE);
	Tools            tools;

	tools.load(config);

	TEST_FALSE(tools.isModified());
	TEST_TRUE(tools.size() == 1);
	TEST_TRUE(tools.tool(0)->m_name == SAVED_TOOL_NAME);
	TEST_TRUE(tools.tool(0)->m_commandLine == SAVED_CMD_LINE);
}
TEST_CASE_END

TEST_CASE("Loading a set of tools from an app config provider replaces the existing set")
{
	FakeAppConfigReader reader;
	Tools               tools;

	tools.append(ToolPtr(new Tool(TXT("tool 1"), TXT("cmd line 1"))));
	tools.append(ToolPtr(new Tool(TXT("tool 2"), TXT("cmd line 2"))));

	tools.load(reader);

	TEST_FALSE(tools.isModified());
	TEST_TRUE(tools.size() == 1);
	TEST_TRUE(tools.tool(0)->m_name == SAVED_TOOL_NAME);
	TEST_TRUE(tools.tool(0)->m_commandLine == SAVED_CMD_LINE);
}
TEST_CASE_END

TEST_CASE("Loading a set of tools from an XML document replaces the existing set")
{
	XML::DocumentPtr config = createDocument(SAVED_TOOL_NAME, SAVED_CMD_LINE);
	Tools            tools;

	tools.append(ToolPtr(new Tool(TXT("tool 1"), TXT("cmd line 1"))));
	tools.append(ToolPtr(new Tool(TXT("tool 2"), TXT("cmd line 2"))));

	tools.load(config);

	TEST_FALSE(tools.isModified());
	TEST_TRUE(tools.size() == 1);
	TEST_TRUE(tools.tool(0)->m_name == SAVED_TOOL_NAME);
	TEST_TRUE(tools.tool(0)->m_commandLine == SAVED_CMD_LINE);
}
TEST_CASE_END

TEST_CASE("A set of tools can be saved to an app config provider")
{
	FakeAppConfigWriter writer;
	Tools               tools;

	tools.append(ToolPtr(new Tool(TEST_TOOL_NAME, TEST_CMD_LINE)));

	tools.save(writer);

	TEST_TRUE(writer.m_values.size() == 0);
	TEST_TRUE(writer.m_deletions.size() == 1);
	TEST_TRUE(writer.m_deletions[0] == TXT("Tools"));
}
TEST_CASE_END

TEST_CASE("A set of tools can be saved to an XML document")
{
	XML::DocumentPtr config = createDocument();
	Tools            tools;

	tools.append(ToolPtr(new Tool(TEST_TOOL_NAME, TEST_CMD_LINE)));

	tools.save(config);

	TEST_FALSE(tools.isModified());

	XML::XPathIterator it(TXT("/FarmMonitor/Tools"), config->getRootElement());
	XML::XPathIterator end;

	TEST_TRUE(it != end);

	XML::ElementNodePtr savedTools = Core::dynamic_ptr_cast<XML::ElementNode>(*it);

	TEST_TRUE(savedTools->getChildCount() == 1);

	XML::ElementNodePtr tool = savedTools->getChild<XML::ElementNode>(0);

	TEST_TRUE(tool->name() == TXT("Tool"));

	XML::ElementNodePtr property = tool->getChild<XML::ElementNode>(0);

	TEST_TRUE(property->name() == TXT("Name"));

	XML::TextNodePtr value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(value->text() == TEST_TOOL_NAME);

	property = tool->getChild<XML::ElementNode>(1);
	value = property->getChild<XML::TextNode>(0);

	TEST_TRUE(property->name() == TXT("CommandLine"));
	TEST_TRUE(value->text() == TEST_CMD_LINE);
}
TEST_CASE_END

TEST_CASE("Writing an unmodified set of tools should still write to the app config provider")
{
	FakeAppConfigReader reader;
	Tools               tools;

	tools.load(reader);

	TEST_FALSE(tools.isModified());

	FakeAppConfigWriter writer;

	tools.save(writer);

	TEST_TRUE(writer.m_values.size() == 0);
	TEST_TRUE(writer.m_deletions.size() == 1);
}
TEST_CASE_END

TEST_CASE("Writing an unmodified set of tools should still write to the XML document")
{
	Tools tools;

	tools.load(createDocument(SAVED_TOOL_NAME, SAVED_CMD_LINE));

	TEST_FALSE(tools.isModified());

	XML::DocumentPtr config = createDocument();

	tools.save(config);

	XML::XPathIterator it(TXT("/FarmMonitor/Tools/Tool"), config->getRootElement());
	XML::XPathIterator end;

	TEST_FALSE(it == end);
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
