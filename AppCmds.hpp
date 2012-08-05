////////////////////////////////////////////////////////////////////////////////
//! \file   AppCmds.hpp
//! \brief  The AppCmds class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_APPCMDS_HPP
#define APP_APPCMDS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CmdCtrl.hpp>

class AppWnd;
class AppDlg;

////////////////////////////////////////////////////////////////////////////////
//! The application command controller.

class AppCmds : public CCmdControl
{
public:
	//! Constructor.
	AppCmds(AppWnd& appWnd, AppDlg& appDlg);

	//! Destructor.
	virtual ~AppCmds();

	//
	// Commands.
	//

	//! Close the application.
	void onFileExit();

	//! Add a new host to monitor.
	void onEditAddHost();

	//! Remove the selected host.
	void onEditRemoveHost();

	//! Show the manual.
	void onHelpContents();

	//! Show the about dialog.
	void onHelpAbout();

	//
	// UI handlers.
	//

	//! Update the UI for the Remove Host command.
	void onUpdateEditRemoveHost();

private:
	//
	// Members.
	//

	//! The app window.
	AppWnd&	m_appWnd;

	//! The main view.
	AppDlg&	m_appDlg;
};

#endif // APP_APPCMDS_HPP
