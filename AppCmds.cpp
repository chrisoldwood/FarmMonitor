////////////////////////////////////////////////////////////////////////////////
//! \file   AppCmds.cpp
//! \brief  The AppCmds class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppCmds.hpp"
#include "FarmMonitor.hpp"
#include "AboutDlg.hpp"
#include "AddHostDlg.hpp"
#include <WCL/BusyCursor.hpp>
#include <Core/FileSystem.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppCmds::AppCmds(AppWnd& appWnd, AppDlg& appDlg)
	: m_appWnd(appWnd)
	, m_appDlg(appDlg)
{
	// Define the command table.
	DEFINE_CMD_TABLE
		// File menu.
		CMD_ENTRY(ID_FILE_EXIT,			&AppCmds::onFileExit,		NULL,								-1)
		//Edit menu
		CMD_ENTRY(ID_EDIT_ADDHOST,		&AppCmds::onEditAddHost,	NULL,								-1)
		CMD_ENTRY(ID_EDIT_REMOVEHOST,	&AppCmds::onEditRemoveHost,	&AppCmds::onUpdateEditRemoveHost,	-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_CONTENTS,		&AppCmds::onHelpContents,	NULL,								-1)
		CMD_ENTRY(ID_HELP_ABOUT,		&AppCmds::onHelpAbout,		NULL,								 2)
	END_CMD_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AppCmds::~AppCmds()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Close the application.

void AppCmds::onFileExit()
{
	m_appWnd.Close();
}

////////////////////////////////////////////////////////////////////////////////
//! Add a new host to monitor.

void AppCmds::onEditAddHost()
{
	AddHostDlg dlg;

	if (dlg.RunModal(m_appWnd) == IDOK)
	{
		m_appDlg.addHost(dlg.m_hostname);

		UpdateUI();
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Remove the selected host.

void AppCmds::onEditRemoveHost()
{
	ASSERT(m_appDlg.isHostSelected());

	m_appDlg.removeSelectedHost();

	UpdateUI();
}

////////////////////////////////////////////////////////////////////////////////
//! Update the UI for the Remove Host command.

void AppCmds::onUpdateEditRemoveHost()
{
	bool hostSelected = m_appDlg.isHostSelected();

	m_appWnd.m_menu.EnableCmd(ID_EDIT_REMOVEHOST, hostSelected);
}

////////////////////////////////////////////////////////////////////////////////
//! Show the manual.

void AppCmds::onHelpContents()
{
	CBusyCursor busyCursor;

	tstring manual = CPath::ApplicationDir() / TXT("FarmMonitor.html");

	if (!Core::pathExists(manual))
	{
		g_app.FatalMsg(TXT("The manual is not installed:-\n\n%s"), manual.c_str());
		return;
	}

	::ShellExecute(NULL, NULL, manual.c_str(), NULL, NULL, SW_SHOW);
}

////////////////////////////////////////////////////////////////////////////////
//! Show the about dialog.

void AppCmds::onHelpAbout()
{
	AboutDlg aboutDlg;

	aboutDlg.RunModal(m_appWnd);
}
