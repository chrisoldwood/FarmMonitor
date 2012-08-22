////////////////////////////////////////////////////////////////////////////////
//! \file   AppCmds.cpp
//! \brief  The AppCmds class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppCmds.hpp"
#include "ExitAppCmd.hpp"
#include "AddHostCmd.hpp"
#include "RemoveHostCmd.hpp"
#include "ShowManualCmd.hpp"
#include "AboutAppCmd.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppCmds::AppCmds(AppWnd& appWnd, AppDlg& appDlg)
	: WCL::ExternalCmdController()
{
	WCL_DEFINE_EXTCMD_TABLE
		// File menu.
		WCL_EXTCMD_ENTRY(new ExitAppCmd(appWnd))
		// Edit menu
		WCL_EXTCMD_ENTRY(new AddHostCmd(appWnd, appDlg, *this))
		WCL_EXTCMD_ENTRY(new RemoveHostCmd(appWnd, appDlg, *this))
		// Help menu.
		WCL_EXTCMD_ENTRY(new ShowManualCmd(appWnd))
		WCL_EXTCMD_ENTRY(new AboutAppCmd(appWnd))
	WCL_END_EXTCMD_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AppCmds::~AppCmds()
{
}
