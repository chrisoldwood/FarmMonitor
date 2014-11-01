////////////////////////////////////////////////////////////////////////////////
//! \file   ExecuteToolCmd.hpp
//! \brief  The ExecuteToolCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_EXECUTETOOLCMD_HPP
#define APP_EXECUTETOOLCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>
#include "Tool.hpp"

// Forward declarations.
class Tools;
class AppWnd;
class AppDlg;
class CPopupMenu;

namespace WCL
{
class ContextMenu;
}

////////////////////////////////////////////////////////////////////////////////
//! The command used to execute an external tool for the selected host.

class ExecuteToolCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	ExecuteToolCmd(Tools& tools, uint index, AppWnd& appWnd, AppDlg& appDlg);

	//
	// IUiCommand methods.
	//

	//! Execute the command.
	virtual void execute();

	//! Refresh the UI for the command.
	virtual void updateUi();

	//
	// Other methods.
	//

	//! Rebuild the main Tools menu.
	static void rebuildToolsMainMenu(const Tools& tools, CPopupMenu& menu, bool hostSelected);

	// Build the tools content menu.
	static void buildToolsContextMenu(const Tools& tools, WCL::ContextMenu& menu, bool hostSelected);

private:
	//
	// Members.
	//
	Tools&	m_tools;	//!< The set of external tools.
	size_t	m_index;	//!< The index of the tool to invoke.
	AppWnd&	m_appWnd;	//!< The app frame window.
	AppDlg& m_appDlg;	//!< The main view.

	//! Convert a ShellExecute error code to a string.
	const tchar* toString(uint code);

	//! Append the command to the Tools menu.
	static void appendCommand(const ConstToolPtr& tool, uint commandId, CPopupMenu& menu, bool enabled);
};

#endif // APP_EXECUTETOOLCMD_HPP
