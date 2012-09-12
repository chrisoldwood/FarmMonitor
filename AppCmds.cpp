////////////////////////////////////////////////////////////////////////////////
//! \file   AppCmds.cpp
//! \brief  The AppCmds class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppCmds.hpp"
#include "ExitAppCmd.hpp"
#include "AddHostCmd.hpp"
#include "RemoveHostCmd.hpp"
#include "EditToolsCmd.hpp"
#include "ShowManualCmd.hpp"
#include "AboutAppCmd.hpp"
#include "ExecuteToolCmd.hpp"

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
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 0, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 1, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 2, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 3, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 4, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 5, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 6, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 7, appDlg))
		WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, 8, appDlg))
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
