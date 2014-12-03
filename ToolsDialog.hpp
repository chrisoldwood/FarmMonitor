////////////////////////////////////////////////////////////////////////////////
//! \file   ToolsDialog.hpp
//! \brief  The ToolsDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef TOOLSDIALOG_HPP
#define TOOLSDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "Tools.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to edit the set of tools.

class ToolsDialog : public CDialog
{
public:
	//! Default constructor.
	ToolsDialog();
	
	//
	// Members.
	//
	Tools		m_tools;	//! The set of tools.

private:
	//! The view columns
	enum Column
	{
		TOOL_NAME,
		COMMAND_LINE,
	};

	//
	// Controls.
	//
	CListView	m_view;		//! The tools view.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();

	//! Tool view selection change handler.
	LRESULT onToolSelected(NMHDR& header);

	//! Tool double-clicked handler.
	LRESULT onToolDoubleClicked(NMHDR& header);

	//! Add button handler.
	void onAddTool();

	//! Copy button handler.
	void onCopyTool();

	//! Edit button handler.
	void onEditTool();

	//! Delete button handler.
	void onDeleteTool();

	//! Up button handler.
	void onMoveToolUp();

	//! Down button handler.
	void onMoveToolDown();

	//
	// Internal methods.
	//

	//! Update the state of the UI.
	void updateUi();

	//! Add an item to the view.
	void addItemToView(ConstToolPtr tool, bool select = false);

	//! Update an item in the view.
	void updateViewItem(size_t row, ConstToolPtr tool);
};

#endif // TOOLSDIALOG_HPP
