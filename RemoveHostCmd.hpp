////////////////////////////////////////////////////////////////////////////////
//! \file   RemoveHostCmd.hpp
//! \brief  The RemoveHostCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_REMOVEHOSTCMD_HPP
#define APP_REMOVEHOSTCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>
#include "AppWnd.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The command to remove the currently selected host from monitoring.

class RemoveHostCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	RemoveHostCmd(AppWnd& appWnd, AppDlg& appDlg, WCL::ICmdController& controller);

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
	AppWnd&					m_appWnd;	//! The app frame window.
	AppDlg&					m_appDlg;	//! The app main view.
	WCL::ICmdController&	m_controller;	//! The command dispatcher.
};

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

inline RemoveHostCmd::RemoveHostCmd(AppWnd& appWnd, AppDlg& appDlg, WCL::ICmdController& controller)
	: WCL::UiCommandBase(ID_HOST_REMOVEHOST)
	, m_appWnd(appWnd)
	, m_appDlg(appDlg)
	, m_controller(controller)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

inline void RemoveHostCmd::execute()
{
	ASSERT(m_appDlg.isHostSelected());

	m_appDlg.removeSelectedHost();

	m_controller.UpdateUI();
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh the UI for the command.

inline void RemoveHostCmd::updateUi()
{
	bool hostSelected = m_appDlg.isHostSelected();

	m_appWnd.m_menu.EnableCmd(id(), hostSelected);
}

#endif // APP_REMOVEHOSTCMD_HPP
