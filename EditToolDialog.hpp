////////////////////////////////////////////////////////////////////////////////
//! \file   EditToolDialog.hpp
//! \brief  The EditToolDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef EDITTOOLDIALOG_HPP
#define EDITTOOLDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "Tool.hpp"
#include <set>

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to edit the defintion for a tool.

class EditToolDialog : public CDialog
{
public:
	//! The set of tool names.
	typedef std::set<tstring> ToolNames;

public:
	//! Default constructor.
	EditToolDialog();
	
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

#endif // EDITTOOLDIALOG_HPP
