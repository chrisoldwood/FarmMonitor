////////////////////////////////////////////////////////////////////////////////
//! \file   AppCmds.cpp
//! \brief  The AppCmds class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppCmds.hpp"
#include "ExitAppCmd.hpp"
#include "AddHostCmd.hpp"
#include "EditHostCmd.hpp"
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
		WCL_EXTCMD_ENTRY(new EditHostCmd(appWnd, appDlg, *this))
		WCL_EXTCMD_ENTRY(new RemoveHostCmd(appWnd, appDlg, *this))
		WCL_EXTCMD_ENTRY(new CopyHostCmd(appWnd, appDlg, *this))

		for (size_t i = 0, end = ID_HOST_INVOKE_TOOL_19-ID_HOST_INVOKE_TOOL_1+1; i != end; ++i)
		{
			WCL_EXTCMD_ENTRY(new ExecuteToolCmd(tools, i, appWnd, appDlg))
		}

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
