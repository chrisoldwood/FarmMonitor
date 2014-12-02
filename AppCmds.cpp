////////////////////////////////////////////////////////////////////////////////
//! \file   AppCmds.cpp
//! \brief  The AppCmds class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppCmds.hpp"
#include "ExitAppCmd.hpp"
#include "AddHostCmd.hpp"
#include "EditHostCmd.hpp"
#include "MoveHostUpCmd.hpp"
#include "MoveHostDownCmd.hpp"
#include "RemoveHostCmd.hpp"
#include "CopyHostCmd.hpp"
#include "EditToolsCmd.hpp"
#include "ShowManualCmd.hpp"
#include "AboutAppCmd.hpp"
#include "ExecuteToolCmd.hpp"
#include "RefreshViewCmd.hpp"
#include "EditQueriesCmd.hpp"
#include "Model.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppCmds::AppCmds(AppWnd& appWnd, AppDlg& appDlg, Model& model)
	: WCL::ExternalCmdController()
{
	WCL_DEFINE_EXTCMD_TABLE
		// File menu.
		WCL_EXTCMD_ENTRY(new ExitAppCmd(appWnd))
		// Host menu
		WCL_EXTCMD_ENTRY(new AddHostCmd(appWnd, appDlg, *this, model.m_hosts))
		WCL_EXTCMD_ENTRY(new EditHostCmd(appWnd, appDlg, *this, model.m_hosts))
		WCL_EXTCMD_ENTRY(new RemoveHostCmd(appWnd, appDlg, *this))
		WCL_EXTCMD_ENTRY(new MoveHostUpCmd(appWnd, appDlg, *this, model.m_hosts))
		WCL_EXTCMD_ENTRY(new MoveHostDownCmd(appWnd, appDlg, *this, model.m_hosts))
		WCL_EXTCMD_ENTRY(new CopyHostCmd(appWnd, appDlg))

		for (size_t i = 0, end = ID_HOST_INVOKE_TOOL_19-ID_HOST_INVOKE_TOOL_1+1; i != end; ++i)
		{
			WCL_EXTCMD_ENTRY(new ExecuteToolCmd(model.m_tools, i, appWnd, appDlg))
		}

		// View menu.
		WCL_EXTCMD_ENTRY(new EditQueriesCmd(appWnd, appDlg, model.m_queries))
		WCL_EXTCMD_ENTRY(new RefreshViewCmd(appWnd))
		// Tools menu.
		WCL_EXTCMD_ENTRY(new EditToolsCmd(appWnd, model.m_tools))
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
