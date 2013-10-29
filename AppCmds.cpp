////////////////////////////////////////////////////////////////////////////////
//! \file   AppCmds.cpp
//! \brief  The AppCmds class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppCmds.hpp"
#include "ExitAppCmd.hpp"
#include "AddHostCmd.hpp"
#include "RemoveHostCmd.hpp"
#include "CopyHostCmd.hpp"
#include "EditToolsCmd.hpp"
#include "ShowManualCmd.hpp"
#include "AboutAppCmd.hpp"
#include "ExecuteToolCmd.hpp"
#include "RefreshViewCmd.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppCmds::AppCmds(AppWnd& appWnd, AppDlg& appDlg, Tools& tools)
	: WCL::ExternalCmdController()
{
	WCL_DEFINE_EXTCMD_TABLE
		// File menu.
		WCL_EXTCMD_ENTRY(new ExitAppCmd(appWnd))
		// Host menu
		WCL_EXTCMD_ENTRY(new AddHostCmd(appWnd, appDlg, *this))
		WCL_EXTCMD_ENTRY(new RemoveHostCmd(appWnd, appDlg, *this))
		WCL_EXTCMD_ENTRY(new CopyHostCmd(appWnd, appDlg, *this))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 0, appWnd, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 1, appWnd, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 2, appWnd, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 3, appWnd, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 4, appWnd, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 5, appWnd, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 6, appWnd, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 7, appWnd, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 8, appWnd, appDlg))
		// View menu.
		WCL_EXTCMD_ENTRY(new RefreshViewCmd(appWnd))
		// Tools menu.
		WCL_EXTCMD_ENTRY(new EditToolsCmd(appWnd, tools))
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
