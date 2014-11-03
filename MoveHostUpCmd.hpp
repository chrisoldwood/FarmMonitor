////////////////////////////////////////////////////////////////////////////////
//! \file   MoveHostUpCmd.hpp
//! \brief  The MoveHostUpCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_MOVEHOSTUPCMD_HPP
#define APP_MOVEHOSTUPCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>
#include "AppWnd.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The command to move a host up the list.

class MoveHostUpCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	MoveHostUpCmd(AppWnd& appWnd, AppDlg& appDlg, WCL::ICmdController& controller, Hosts& hosts);

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

inline MoveHostUpCmd::MoveHostUpCmd(AppWnd& appWnd, AppDlg& appDlg, WCL::ICmdController& controller, Hosts& hosts)
	: WCL::UiCommandBase(ID_HOST_UP)
	, m_appWnd(appWnd)
	, m_appDlg(appDlg)
	, m_controller(controller)
	, m_hosts(hosts)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

inline void MoveHostUpCmd::execute()
{
	ASSERT(m_appDlg.isHostSelected());
	ASSERT(m_appDlg.getSelectedHostIndex() > 0);

	const size_t selection = m_appDlg.getSelectedHostIndex();

	m_hosts.swap(selection, selection-1);
	m_appDlg.swapHosts(selection, selection-1);

	m_controller.UpdateUI();
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh the UI for the command.

inline void MoveHostUpCmd::updateUi()
{
	const size_t selection = m_appDlg.getSelectedHostIndex();
	const bool   enabled = (selection != Core::npos) && (selection != 0);

	m_appWnd.m_menu.EnableCmd(id(), enabled);
}

#endif // APP_MOVEHOSTUPCMD_HPP
