////////////////////////////////////////////////////////////////////////////////
//! \file   EditHostCmd.hpp
//! \brief  The EditHostCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_EDITHOSTCMD_HPP
#define APP_EDITHOSTCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Edit the name of the selected host.

class EditHostCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	EditHostCmd(AppWnd& appWnd, AppDlg& appDlg, WCL::ICmdController& controller, Hosts& hosts);

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
	AppWnd&					m_appWnd;		//! The app frame window.
	AppDlg&					m_appDlg;		//! The app main view.
	WCL::ICmdController&	m_controller;	//! The command dispatcher.
	Hosts&					m_hosts;		//! The collection of hosts.
};

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

inline EditHostCmd::EditHostCmd(AppWnd& appWnd, AppDlg& appDlg, WCL::ICmdController& controller, Hosts& hosts)
	: WCL::UiCommandBase(ID_HOST_EDITHOST)
	, m_appWnd(appWnd)
	, m_appDlg(appDlg)
	, m_controller(controller)
	, m_hosts(hosts)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

inline void EditHostCmd::execute()
{
	ASSERT(m_appDlg.isHostSelected());

	AddEditHostDlg dlg(AddEditHostDlg::EDIT_HOST);

	dlg.m_host = *m_appDlg.getSelectedHost();

	for (Hosts::const_iterator it = m_hosts.begin(); it != m_hosts.end(); ++it)
	{
		if ((*it)->m_name != dlg.m_host.m_name)
			dlg.m_hosts.insert((*it)->m_name);
	}

	if (dlg.RunModal(m_appWnd) == IDOK)
	{
		m_appDlg.replaceHost(makeHost(dlg.m_host));

		m_controller.UpdateUI();
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh the UI for the command.

inline void EditHostCmd::updateUi()
{
	bool hostSelected = m_appDlg.isHostSelected();

	m_appWnd.m_menu.EnableCmd(id(), hostSelected);
}

#endif // APP_EDITHOSTCMD_HPP
