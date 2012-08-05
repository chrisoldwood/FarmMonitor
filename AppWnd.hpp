////////////////////////////////////////////////////////////////////////////////
//! \file   AppWnd.hpp
//! \brief  The AppWnd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_APPWND_HPP
#define APP_APPWND_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/DlgFrame.hpp>
#include "AppDlg.hpp"
#include <WCL/FrameMenu.hpp>
#include "AppToolbar.hpp"
#include <WCL/StatusBar.hpp>
#include <WCL/Accel.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The application main window.

class AppWnd : public CDlgFrame
{
public:
	//! Constructor.
	AppWnd(WCL::IMsgThread& thread, WCL::ICmdController& controller, HostsPtr hosts);

	//! Destructor.
	virtual ~AppWnd();

	//
	// Public Members.
	//
	AppDlg		m_mainDlg;		//!< The main dialog.
	CFrameMenu	m_menu;			//!< The main menu.
	AppToolbar	m_toolbar;		//!< The toolbar.
	CStatusBar	m_statusbar;	//!< The status bar.

private:
	//
	// Internal members.
	//
	CAccel		m_accelerators;	//!< The accelerator table.

	// Child window IDs.
	enum
	{
		IDC_TOOL_BAR   = 100,
		IDC_STATUS_BAR = 101
	};

	//
	// Message processors.
	//

	//! Handle window creation.
	virtual void OnCreate(const CRect& clientRect);
};

#endif // APP_APPWND_HPP
