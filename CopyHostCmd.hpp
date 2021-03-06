////////////////////////////////////////////////////////////////////////////////
//! \file   CopyHostCmd.hpp
//! \brief  The CopyHostCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_COPYHOSTCMD_HPP
#define APP_COPYHOSTCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>
#include "AppWnd.hpp"
#include <WCL/Clipboard.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The command to copy the currently selected host name to the clipboard.

class CopyHostCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	CopyHostCmd(AppWnd& appWnd, AppDlg& appDlg);

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
};

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

inline CopyHostCmd::CopyHostCmd(AppWnd& appWnd, AppDlg& appDlg)
	: WCL::UiCommandBase(ID_HOST_COPYHOST)
	, m_appWnd(appWnd)
	, m_appDlg(appDlg)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

inline void CopyHostCmd::execute()
{
	ASSERT(m_appDlg.isHostSelected());

	const tstring hostname = m_appDlg.getSelectedHost()->m_name;

	CClipboard::CopyText(m_appWnd.Handle(), hostname);
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh the UI for the command.

inline void CopyHostCmd::updateUi()
{
	const bool hostSelected = m_appDlg.isHostSelected();

	m_appWnd.m_menu.EnableCmd(id(), hostSelected);
}

#endif // APP_COPYHOSTCMD_HPP
