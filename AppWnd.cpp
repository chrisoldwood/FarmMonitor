////////////////////////////////////////////////////////////////////////////////
//! \file   AppWnd.cpp
//! \brief  The AppWnd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppWnd.hpp"
#include "FarmMonitor.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppWnd::AppWnd()
	: CDlgFrame(IDR_APPICON, m_mainDlg, false)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AppWnd::~AppWnd()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Handle window creation.

void AppWnd::OnCreate(const CRect& clientRect)
{
	//
	// Create and attach the components.
	//
	m_accelerators.LoadRsc(IDR_APPACCEL);
	Accel(&m_accelerators);

	m_menu.LoadRsc(IDR_APPMENU);
	Menu(&m_menu);

//	m_toolbar.Create(*this, IDC_TOOL_BAR, clientRect);
//	ToolBar(&m_toolbar);

	m_statusbar.Create(*this, IDC_STATUS_BAR, clientRect);
	StatusBar(&m_statusbar);

	m_mainDlg.RunModeless(*this);
	ActiveDlg(&m_mainDlg);

	// Call base class.
	CDlgFrame::OnCreate(clientRect);
}

////////////////////////////////////////////////////////////////////////////////
//! Handle the window closing.

void AppWnd::OnClose()
{
	// Fetch windows final placement.
	g_app.m_lastWndPos = Placement();
}
