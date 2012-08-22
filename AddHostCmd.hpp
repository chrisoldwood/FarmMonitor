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
#include "AddHostDlg.hpp"
#include "AppWnd.hpp"
#include "Resource.h"

////////////////////////////////////////////////////////////////////////////////
//! The command to add a new host to monitor.

class AddHostCmd : public WCL::UiCommandBase
{
public:
	//! Default constructor.
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
//! Default constructor.

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
	AddHostDlg dlg;

	if (dlg.RunModal(m_appWnd) == IDOK)
	{
		m_appDlg.addHost(dlg.m_hostname);

		m_controller.UpdateUI();
	}
}

#endif // APP_ADDHOSTCMD_HPP
