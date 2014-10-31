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
#include "AddEditHostDlg.hpp"
#include "AppWnd.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The command to add a new host to monitor.

class AddHostCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	AddHostCmd(AppWnd& appWnd, AppDlg& appDlg, WCL::ICmdController& controller);

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
};

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

inline AddHostCmd::AddHostCmd(AppWnd& appWnd, AppDlg& appDlg, WCL::ICmdController& controller)
	: WCL::UiCommandBase(ID_HOST_ADDHOST)
	, m_appWnd(appWnd)
	, m_appDlg(appDlg)
	, m_controller(controller)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

inline void AddHostCmd::execute()
{
	AddEditHostDlg dlg(AddEditHostDlg::ADD_HOST);

	if (dlg.RunModal(m_appWnd) == IDOK)
	{
		m_appDlg.addHost(makeHost(dlg.m_host));

		m_controller.UpdateUI();
	}
}

#endif // APP_ADDHOSTCMD_HPP
