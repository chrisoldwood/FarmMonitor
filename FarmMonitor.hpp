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
#include <Core/UniquePtr.hpp>
#include <WCL/AutoCom.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The application singleton.

class FarmMonitor : public CApp
{
public:
	typedef std::vector<tstring> HostList;

public:
	//! Constructor.
	FarmMonitor();

	//! Destructor.
	virtual ~FarmMonitor();

	//
	// Public members.
	//
	AppWnd		m_appWnd;		//!< The main window.
	AppCmds		m_appCmds;		//!< The command handler.

	CRect		m_lastWndPos;	//!< Main window previous position.

	HostList	m_hostList;		//!< The list of hosts to monitor.

private:
	//
	// Private members.
	//
	WCL::AutoCom	m_autoCom;	//!< The COM initialiser.

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

	//! Load the application settings.
	void loadConfig();

	//! Save the application settings.
	void saveConfig();
};

//! The application singleton instance.
extern FarmMonitor g_app;

#endif // APP_FARMMONITOR_HPP
