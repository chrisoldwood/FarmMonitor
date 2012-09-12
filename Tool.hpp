////////////////////////////////////////////////////////////////////////////////
//! \file   Tool.hpp
//! \brief  The Tool class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_TOOL_HPP
#define APP_TOOL_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
//! The defintion of an external tool that can be launched for a host.

class Tool
{
public:
	//! Default constructor.
	Tool();

	//! Constructor.
	Tool(const tstring& name, const tstring& commandLine);

	//
	// Members.
	//
	tstring		m_name;			//! The unique name for the tool.
	tstring		m_commandLine;	//! The command line to invoke.
};

//! The default Tool smart pointer type.
typedef Core::SharedPtr<Tool> ToolPtr;

//! The default Tool const smart pointer type.
typedef Core::SharedPtr<const Tool> ConstToolPtr;

#endif // APP_TOOL_HPP
