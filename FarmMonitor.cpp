////////////////////////////////////////////////////////////////////////////////
//! \file   FarmMonitor.cpp
//! \brief  The FarmMonitor class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "FarmMonitor.hpp"
#include <WCL/AppConfig.hpp>
#include <Core/ConfigurationException.hpp>

////////////////////////////////////////////////////////////////////////////////
// Global variables.

//! The application singleton instance.
FarmMonitor g_app;

////////////////////////////////////////////////////////////////////////////////
// Constants.

//! The configuration data publisher name.
const tchar* PUBLISHER = TXT("Chris Oldwood");
//! The configuration data application name.
const tchar* APPLICATION = TXT("Farm Monitor");
//! The configuration data format version.
const tchar* CONFIG_VERSION = TXT("0.1");

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

FarmMonitor::FarmMonitor()
	: CApp(m_appWnd, m_appCmds)
	, m_hostList()
	, m_autoCom()
{

}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

FarmMonitor::~FarmMonitor()
{

}

////////////////////////////////////////////////////////////////////////////////
//! Handle application startup.

bool FarmMonitor::OnOpen()
{
	// Initialise COM.
	m_autoCom.Initialise(COINIT_APARTMENTTHREADED);

	// Set the app title.
	m_strTitle = TXT("Farm Monitor");

	try
	{
		// Load settings.
		loadConfig();
	}
	catch (const Core::Exception& e)
	{
		FatalMsg(TXT("Failed to configure the application:-\n\n%s"), e.twhat());
		return false;
	}
	
	// Load the toolbar bitmap.
	m_rCmdControl.CmdBitmap().LoadRsc(IDR_APPTOOLBAR);

	// Create the main window.
	if (!m_appWnd.Create())
		return false;

	// Show it.
	if (ShowNormal() && !m_lastWndPos.Empty())
		m_appWnd.Move(m_lastWndPos);

	m_appWnd.Show(m_iCmdShow);

	// Update UI.
	m_appCmds.UpdateUI();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle application shutdown.

bool FarmMonitor::OnClose()
{
	try
	{
		// Save settings.
		saveConfig();
	}
	catch (const Core::Exception& e)
	{
		FatalMsg(TXT("Failed to save the application configuration:-\n\n%s"), e.twhat());
		return false;
	}

	// Uninitialise COM.
	m_autoCom.Uninitialise();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Load the application settings.

void FarmMonitor::loadConfig()
{
	WCL::AppConfig appConfig(PUBLISHER, APPLICATION);

	// Read the config data version.
	tstring version = appConfig.readString(appConfig.DEFAULT_SECTION, TXT("Version"), CONFIG_VERSION);

	if (version != CONFIG_VERSION)
		throw Core::ConfigurationException(Core::fmt(TXT("The configuration data is incompatible - '%s'"), version.c_str()));

	// Read the UI settings.
	m_lastWndPos = appConfig.readValue<CRect>(TXT("UI"), TXT("MainWindow"), m_lastWndPos);

	m_hostList.push_back(TXT("localhost"));
}

////////////////////////////////////////////////////////////////////////////////
//! Save the application settings.

void FarmMonitor::saveConfig()
{
	WCL::AppConfig appConfig(PUBLISHER, APPLICATION);

	// Write the config data version.
	appConfig.writeString(appConfig.DEFAULT_SECTION, TXT("Version"), CONFIG_VERSION);

	// Write the UI settings.
	appConfig.writeValue<CRect>(TXT("UI"), TXT("MainWindow"), m_lastWndPos);
}
