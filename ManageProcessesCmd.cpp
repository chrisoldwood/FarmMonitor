////////////////////////////////////////////////////////////////////////////////
//! \file   ManageProcessesCmd.cpp
//! \brief  The ManageProcessesCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ManageProcessesCmd.hpp"
#include "AppWnd.hpp"
#include "ProcessesDialog.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

ManageProcessesCmd::ManageProcessesCmd(AppWnd& appWnd, AppDlg& appDlg)
	: WCL::UiCommandBase(ID_HOST_PROCESSES)
	, m_appWnd(appWnd)
	, m_appDlg(appDlg)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

void ManageProcessesCmd::execute()
{
	ASSERT(m_appDlg.isHostSelected());

	const tstring hostname = m_appDlg.getSelectedHost()->m_name;

	ProcessesDialog dialog(hostname);

	dialog.RunModal(m_appWnd);
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh the UI for the command.

void ManageProcessesCmd::updateUi()
{
	bool hostSelected = m_appDlg.isHostSelected();

	m_appWnd.m_menu.EnableCmd(id(), hostSelected);
}
