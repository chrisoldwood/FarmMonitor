////////////////////////////////////////////////////////////////////////////////
//! \file   HostToolDialog.hpp
//! \brief  The HostToolDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef HOSTTOOLDIALOG_HPP
#define HOSTTOOLDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "Tool.hpp"
#include <set>

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to edit the defintion for a tool.

class HostToolDialog : public CDialog
{
public:
	//! The set of tool names.
	typedef std::set<tstring> ToolNames;

public:
	//! Default constructor.
	HostToolDialog();
	
	//
	// Members.
	//
	Tool		m_tool;				//! The tool being edited.
	ToolNames	m_usedNames;		//! The set of tool names already in use.

private:
	//
	// Members.
	//
	
	//
	// Controls.
	//
	CEditBox	m_nameEditor;		//! The tool name edit control.
	CComboBox	m_cmdLineEditor;	//! The command line edit control.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();
};

#endif // HOSTTOOLDIALOG_HPP
