////////////////////////////////////////////////////////////////////////////////
//! \file   ManageServicesCmd.cpp
//! \brief  The ManageServicesCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ManageServicesCmd.hpp"
#include "AppWnd.hpp"
#include "ServicesDialog.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

ManageServicesCmd::ManageServicesCmd(AppWnd& appWnd, AppDlg& appDlg)
	: WCL::UiCommandBase(ID_HOST_SERVICES)
	, m_appWnd(appWnd)
	, m_appDlg(appDlg)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

void ManageServicesCmd::execute()
{
	ASSERT(m_appDlg.isHostSelected());

	const tstring hostname = m_appDlg.getSelectedHost()->m_name;

	ServicesDialog dialog(hostname);

	dialog.RunModal(m_appWnd);
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh the UI for the command.

void ManageServicesCmd::updateUi()
{
	bool hostSelected = m_appDlg.isHostSelected();

	m_appWnd.m_menu.EnableCmd(id(), hostSelected);
}
