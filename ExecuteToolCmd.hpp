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

// Forward declarations.
class Tools;
class AppWnd;
class AppDlg;

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
};

#endif // APP_EXECUTETOOLCMD_HPP
