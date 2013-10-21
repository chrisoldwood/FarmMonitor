////////////////////////////////////////////////////////////////////////////////
//! \file   Tools.cpp
//! \brief  The Tools class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Tools.hpp"
#include <WCL/AppConfig.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Tools::Tools()
	: m_modified(false)
	, m_tools()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

Tools::~Tools()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the number of tools.

size_t Tools::size() const
{
	return m_tools.size();
}
	
////////////////////////////////////////////////////////////////////////////////
//! Get the tool at a given position.

const ConstToolPtr& Tools::tool(size_t index) const
{
	ASSERT(index < m_tools.size());

	return m_tools[index];
}

////////////////////////////////////////////////////////////////////////////////
//! Has the collection of tools been modified?

bool Tools::isModified() const
{
	return m_modified;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the collection start iterator.

Tools::const_iterator Tools::begin() const
{
	return m_tools.begin();
}

////////////////////////////////////////////////////////////////////////////////
//! Get the collection end iterator.

Tools::const_iterator Tools::end() const
{
	return m_tools.end();
}

////////////////////////////////////////////////////////////////////////////////
//! Add a tool to the collection.

void Tools::append(ConstToolPtr tool)
{
	m_tools.push_back(tool);
	m_modified = true;
}

////////////////////////////////////////////////////////////////////////////////
//! Replace a tool in the collection.

void Tools::replace(size_t index, ConstToolPtr tool)
{
	ASSERT(index < m_tools.size());

	m_tools[index] = tool;
	m_modified = true;
}

////////////////////////////////////////////////////////////////////////////////
//! Remove a tool from the collection.

void Tools::remove(size_t index)
{
	ASSERT(index < m_tools.size());

	m_tools.erase(m_tools.begin() + index);
	m_modified = true;
}

////////////////////////////////////////////////////////////////////////////////
//! Load the set of tools.

void Tools::load(WCL::IAppConfigReader& config)
{
	Collection tools;

	const size_t count = config.readValue<size_t>(TXT("Tools"), TXT("Count"), 0);

	for (size_t i = 0; i != count; ++i)
	{
		const tstring toolName = config.readString(TXT("Tools"), Core::fmt(TXT("ToolName[%u]"), i), TXT(""));
		const tstring cmdLine  = config.readString(TXT("Tools"), Core::fmt(TXT("CmdLine[%u]"),  i), TXT(""));

		if (!toolName.empty() && !cmdLine.empty())
			tools.push_back(ToolPtr(new Tool(toolName, cmdLine)));
	}

	std::swap(m_tools, tools);
	m_modified = false;
}

////////////////////////////////////////////////////////////////////////////////
//! Save the set of tools.

void Tools::save(WCL::IAppConfigWriter& config)
{
	if (!m_modified)
		return;

	config.deleteSection(TXT("Tools"));
	config.writeValue<size_t>(TXT("Tools"), TXT("Count"), m_tools.size());

	for (size_t i = 0; i != m_tools.size(); ++i)
	{
		const ConstToolPtr tool = m_tools[i];

		config.writeString(TXT("Tools"), Core::fmt(TXT("ToolName[%u]"), i), tool->m_name);
		config.writeString(TXT("Tools"), Core::fmt(TXT("CmdLine[%u]"),  i), tool->m_commandLine);
	}

	m_modified = false;
}

////////////////////////////////////////////////////////////////////////////////
//! Create a deep copy of another collection.

void Tools::deepCopy(const Tools& rhs)
{
	deepCopy(rhs, false);
}

////////////////////////////////////////////////////////////////////////////////
//! Replace the entire collection with another.

void Tools::replaceAll(const Tools& rhs)
{
	deepCopy(rhs, true);
}

////////////////////////////////////////////////////////////////////////////////
//! Create a deep copy of another collection.

void Tools::deepCopy(const Tools& rhs, bool modified)
{
	Collection tools;

	for (const_iterator it = rhs.begin(); it != rhs.end(); ++it)
		tools.push_back(ToolPtr(new Tool(*(*it))));

	std::swap(m_tools, tools);
	m_modified = modified;
}