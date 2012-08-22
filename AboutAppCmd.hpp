////////////////////////////////////////////////////////////////////////////////
//! \file   AboutAppCmd.hpp
//! \brief  The AboutAppCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_ABOUTAPPCMD_HPP
#define APP_ABOUTAPPCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>
#include "AppWnd.hpp"
#include "Resource.h"
#include "AboutDlg.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The command to show the application "About" dialog.

class AboutAppCmd : public WCL::UiCommandBase
{
public:
	//! Construction with the app window.
	AboutAppCmd(AppWnd& appWnd);

	//
	// IUiCommand methods.
	//

	//! Execute the command.
	virtual void execute();

private:
	//
	// Members.
	//
	AppWnd&	m_appWnd;	//! The app window.
};

////////////////////////////////////////////////////////////////////////////////
//! Construction with the app window.

inline AboutAppCmd::AboutAppCmd(AppWnd& appWnd)
	: WCL::UiCommandBase(ID_HELP_ABOUT)
	, m_appWnd(appWnd)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

inline void AboutAppCmd::execute()
{
	AboutDlg aboutDlg;

	aboutDlg.RunModal(m_appWnd);
}

#endif // APP_ABOUTAPPCMD_HPP
