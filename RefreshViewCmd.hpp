////////////////////////////////////////////////////////////////////////////////
//! \file   RefreshViewCmd.hpp
//! \brief  The RefreshViewCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_REFRESHVIEWCMD_HPP
#define APP_REFRESHVIEWCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>
#include "AppWnd.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The command to refresh the main application view.

class RefreshViewCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	RefreshViewCmd(AppWnd& appWnd);

	//
	// IUiCommand methods.
	//

	//! Execute the command.
	virtual void execute();

private:
	//
	// Members.
	//
	AppWnd&	m_appWnd;	//! The app window.
};

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

inline RefreshViewCmd::RefreshViewCmd(AppWnd& appWnd)
	: WCL::UiCommandBase(ID_VIEW_REFRESH)
	, m_appWnd(appWnd)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

inline void RefreshViewCmd::execute()
{
	m_appWnd.m_mainDlg.refreshView();
}

#endif // APP_REFRESHVIEWCMD_HPP
