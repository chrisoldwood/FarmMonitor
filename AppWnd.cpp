////////////////////////////////////////////////////////////////////////////////
//! \file   AppWnd.cpp
//! \brief  The AppWnd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppWnd.hpp"
#include "Tools.hpp"
#include <Core/StringUtils.hpp>
#include "ExecuteToolCmd.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppWnd::AppWnd(WCL::IMsgThread& thread, WCL::ICmdController& controller, Hosts& hosts, Tools& tools)
	: CDlgFrame(IDR_APPICON, m_mainDlg, false)
	, m_mainDlg(*this, controller, hosts, tools)
	, m_toolbar(thread, controller)
	, m_tools(tools)
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

	rebuildToolsMenu();

	// Call base class.
	CDlgFrame::OnCreate(clientRect);
}

////////////////////////////////////////////////////////////////////////////////
//! Rebuild the custom tools menu.

void AppWnd::rebuildToolsMenu()
{
	const bool hostSelected = m_mainDlg.isHostSelected();
	CPopupMenu hostMenu = m_menu.GetItemPopup(1);

	ExecuteToolCmd::rebuildToolsMainMenu(m_tools, hostMenu, hostSelected);
}
