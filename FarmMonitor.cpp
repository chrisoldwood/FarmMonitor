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
	, m_hosts()
	, m_tools()
	, m_appWnd(m_MainThread, m_appCmds, m_hosts, m_tools)
	, m_appCmds(m_appWnd, m_appWnd.m_mainDlg, m_tools)
{
	m_strTitle = TXT("Farm Monitor");
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
	m_autoCom.Initialise(COINIT_APARTMENTTHREADED);

	if (!loadConfig())
		return false;

	if (!m_appWnd.Open(m_iCmdShow, m_startPosition))
		return false;

	if (!m_startWidths.empty())
		m_appWnd.m_mainDlg.setColumnWidths(m_startWidths);

//	m_appCmds.InitialiseUI();
	m_appCmds.UpdateUI();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle application shutdown.

bool FarmMonitor::OnClose()
{
	saveConfig();

	m_autoCom.Uninitialise();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Load the application settings.

bool FarmMonitor::loadConfig()
{
	try
	{
		WCL::AppConfig appConfig(PUBLISHER, APPLICATION);

		const tstring version = appConfig.readString(appConfig.DEFAULT_SECTION, TXT("Version"), CONFIG_VERSION);

		if (version != CONFIG_VERSION)
			throw Core::ConfigurationException(Core::fmt(TXT("The configuration data is incompatible - '%s'"), version.c_str()));

		m_startPosition = appConfig.readValue<CRect>(TXT("UI"), TXT("MainWindow"), m_startPosition);
		appConfig.readList<size_t>(TXT("UI"), TXT("ColumnWidths"), m_startWidths, m_startWidths);

		m_hosts.load(appConfig);
		m_tools.load(appConfig);
	}
	catch (const Core::Exception& e)
	{
		FatalMsg(TXT("Failed to load the application configuration:-\n\n%s"), e.twhat());
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Save the application settings.

void FarmMonitor::saveConfig()
{
	try
	{
		WCL::AppConfig appConfig(PUBLISHER, APPLICATION);

		appConfig.writeString(appConfig.DEFAULT_SECTION, TXT("Version"), CONFIG_VERSION);

		appConfig.writeValue<CRect>(TXT("UI"), TXT("MainWindow"), m_appWnd.FinalPlacement());
		appConfig.writeList<size_t>(TXT("UI"), TXT("ColumnWidths"), m_appWnd.m_mainDlg.getFinalColumnWidths());

		m_hosts.save(appConfig);
		m_tools.save(appConfig);
	}
	catch (const Core::Exception& e)
	{
		FatalMsg(TXT("Failed to save the application configuration:-\n\n%s"), e.twhat());
	}
}
