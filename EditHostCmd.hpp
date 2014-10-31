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
	EditHostCmd(AppWnd& appWnd, AppDlg& appDlg, WCL::ICmdController& controller);

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
};

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

inline EditHostCmd::EditHostCmd(AppWnd& appWnd, AppDlg& appDlg, WCL::ICmdController& controller)
	: WCL::UiCommandBase(ID_HOST_EDITHOST)
	, m_appWnd(appWnd)
	, m_appDlg(appDlg)
	, m_controller(controller)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

inline void EditHostCmd::execute()
{
	ASSERT(m_appDlg.isHostSelected());

	AddEditHostDlg dlg(AddEditHostDlg::EDIT_HOST);

	dlg.m_host = *m_appDlg.getSelectedHost();

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
