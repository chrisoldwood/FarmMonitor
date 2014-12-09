////////////////////////////////////////////////////////////////////////////////
//! \file   ManageProcessesCmd.hpp
//! \brief  The ManageProcessesCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_MANAGEPROCESSESCMD_HPP
#define APP_MANAGEPROCESSESCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>

// Forward declarations.
class AppWnd;
class AppDlg;

////////////////////////////////////////////////////////////////////////////////
//! Show the dialog to manage the processes on the host.

class ManageProcessesCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	ManageProcessesCmd(AppWnd& appWnd, AppDlg& appDlg);

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
	AppWnd&	m_appWnd;		//! The app frame window.
	AppDlg&	m_appDlg;		//! The app main view.
};

#endif // APP_MANAGEPROCESSESCMD_HPP
