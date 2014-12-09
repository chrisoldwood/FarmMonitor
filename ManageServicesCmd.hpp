////////////////////////////////////////////////////////////////////////////////
//! \file   ManageServicesCmd.hpp
//! \brief  The ManageServicesCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_MANAGESERVICESCMD_HPP
#define APP_MANAGESERVICESCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>

// Forward declarations.
class AppWnd;
class AppDlg;

////////////////////////////////////////////////////////////////////////////////
//! Show the dialog to manage the services on the host.

class ManageServicesCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	ManageServicesCmd(AppWnd& appWnd, AppDlg& appDlg);

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

#endif // APP_MANAGESERVICESCMD_HPP
