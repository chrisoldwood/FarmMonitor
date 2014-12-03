////////////////////////////////////////////////////////////////////////////////
//! \file   AddHostCmd.hpp
//! \brief  The AddHostCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_ADDHOSTCMD_HPP
#define APP_ADDHOSTCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>
#include "EditHostDialog.hpp"
#include "AppWnd.hpp"
#include "Hosts.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The command to add a new host to monitor.

class AddHostCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	AddHostCmd(AppWnd& appWnd, AppDlg& appDlg, WCL::ICmdController& controller, Hosts& hosts);

	//
	// IUiCommand methods.
	//

	//! Execute the command.
	virtual void execute();

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

inline AddHostCmd::AddHostCmd(AppWnd& appWnd, AppDlg& appDlg, WCL::ICmdController& controller, Hosts& hosts)
	: WCL::UiCommandBase(ID_HOST_ADDHOST)
	, m_appWnd(appWnd)
	, m_appDlg(appDlg)
	, m_controller(controller)
	, m_hosts(hosts)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

inline void AddHostCmd::execute()
{
	EditHostDialog dialog(EditHostDialog::ADD_HOST);

	for (Hosts::const_iterator it = m_hosts.begin(); it != m_hosts.end(); ++it)
		dialog.m_hosts.insert((*it)->m_name);

	if (dialog.RunModal(m_appWnd) == IDOK)
	{
		m_appDlg.addHost(makeHost(dialog.m_host));

		m_controller.UpdateUI();
	}
}

#endif // APP_ADDHOSTCMD_HPP
