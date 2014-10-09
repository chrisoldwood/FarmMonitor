////////////////////////////////////////////////////////////////////////////////
//! \file   FarmMonitor.hpp
//! \brief  The FarmMonitor class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_FARMMONITOR_HPP
#define APP_FARMMONITOR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/App.hpp>
#include "AppWnd.hpp"
#include "AppCmds.hpp"
#include "Hosts.hpp"
#include "Tools.hpp"
#include <Core/UniquePtr.hpp>
#include <WCL/AutoCom.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The application singleton.

class FarmMonitor : public CApp
{
public:
	//! Constructor.
	FarmMonitor();

	//! Destructor.
	virtual ~FarmMonitor();

	//
	// Public members.
	//
	Hosts		m_hosts;		//!< The hosts being monitored.
	Tools		m_tools;		//!< The external tools to invoke.

	AppWnd		m_appWnd;		//!< The main window.
	AppCmds		m_appCmds;		//!< The command handler.

private:
	//
	// Private members.
	//
	WCL::AutoCom	m_autoCom;			//!< The COM initialiser.
	CRect			m_startPosition;	//!< The main window start position.
	AppDlg::ColumnWidths m_startWidths;	//!< The main view column widths.

	//
	// CApp overriden methods.
	//

	//! Handle application startup.
	virtual	bool OnOpen();

	//! Handle application shutdown.
	virtual	bool OnClose();

	//
	// Internal methods.
	//

	//! Load the application settings from the app config store.
	bool loadConfigFromAppConfig();

	//! Save the application settings to the app config store.
	void saveConfigToAppConfig();

	//! Load the application settings from the default XML file.
	bool loadConfigFromXmlFile();

	//! Save the application settings to the default XML file.
	void saveConfigToXmlFile();
};

//! The application singleton instance.
extern FarmMonitor g_app;

#endif // APP_FARMMONITOR_HPP
