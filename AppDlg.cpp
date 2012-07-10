////////////////////////////////////////////////////////////////////////////////
//! \file   AppDlg.cpp
//! \brief  The AppDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppDlg.hpp"
#include "Resource.h"
#include "FarmMonitor.hpp"
#include <WMI/Connection.hpp>
#include <WMI/Win32_OperatingSystem.hpp>
#include <WMI/Exception.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppDlg::AppDlg()
	: CMainDlg(IDD_MAIN)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_HOSTS,	&m_hosts)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_HOSTS, LEFT_EDGE, TOP_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AppDlg::~AppDlg()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Handle dialog creation.

void AppDlg::OnInitDialog()
{
	initialiseHostsView();

	refreshView();
}

////////////////////////////////////////////////////////////////////////////////
//! Initialise the hosts view.

void AppDlg::initialiseHostsView()
{
	const FarmMonitor::HostList& hosts = g_app.m_hostList;

	m_hosts.InsertColumn(HOSTNAME,         TXT("Host"),     m_hosts.StringWidth(20), LVCFMT_LEFT);
	m_hosts.InsertColumn(LAST_BOOTUP_TIME, TXT("Rebooted"), m_hosts.StringWidth(20), LVCFMT_LEFT);

	for (size_t i = 0; i < hosts.size(); ++i)
	{
		const tstring& host = hosts[i];

		m_hosts.InsertItem(i,                   host);
		m_hosts.ItemText  (i, LAST_BOOTUP_TIME, TXT(""));
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh the hosts view.

void AppDlg::refreshView()
{
	const FarmMonitor::HostList& hosts = g_app.m_hostList;

	WMI::Connection connection;

	for (size_t i = 0; i < hosts.size(); ++i)
	{
		const tstring& host = hosts[i];

		try
		{
			connection.open(host);

			WMI::Win32_OperatingSystem::Iterator it = WMI::Win32_OperatingSystem::select(connection);

			m_hosts.ItemText(i, LAST_BOOTUP_TIME, it->LastBootUpTime().ToString());
		}
		catch (WMI::Exception& e)
		{
			TRACE2(TXT("%s: %s"), host.c_str(), e.twhat());
		}
	}
}
