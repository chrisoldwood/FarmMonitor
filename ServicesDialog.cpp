////////////////////////////////////////////////////////////////////////////////
//! \file   ServicesDialog.cpp
//! \brief  The ServicesDialog class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ServicesDialog.hpp"
#include "Resource.h"
#include <WCL/BusyCursor.hpp>
#include <WMI/Connection.hpp>
#include <WMI/Exception.hpp>
#include <WMI/Win32_Service.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

ServicesDialog::ServicesDialog(const tstring& host)
	: CDialog(IDD_SERVICES)
	, m_host(host)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_SERVICES, &m_view)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_REFRESH, BN_CLICKED, &ServicesDialog::onRefreshView)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void ServicesDialog::OnInitDialog()
{
	m_view.ImageList(LVSIL_SMALL, IDB_SERVICE_ICONS, 16, RGB(255, 0, 255));
	m_view.FullRowSelect(true);

	m_view.InsertColumn(NAME,       TXT("Name"),       m_view.StringWidth(40));
	m_view.InsertColumn(STATE,      TXT("State"),      m_view.StringWidth(15));
	m_view.InsertColumn(START_MODE, TXT("Start Mode"), m_view.StringWidth(15));

	PostCtrlMsg(BN_CLICKED, IDC_REFRESH, CtrlHandle(IDC_REFRESH));
}

////////////////////////////////////////////////////////////////////////////////
//! Determine the icon from the current state and startup type.

static size_t DetermineImage(const WMI::Win32_Service& service)
{
	if (service.StartMode() == TXT("Disabled"))
		return 0;

	if (service.State() == TXT("Running"))
		return 1;

	if ( (service.StartMode() == TXT("Auto")) && (service.State() != TXT("Running")) )
		return 3;

	return 2;
} 

////////////////////////////////////////////////////////////////////////////////
//! Refresh button handler.

void ServicesDialog::onRefreshView()
{
	CBusyCursor waitCursor;

	m_view.Redraw(false);
	m_view.DeleteAllItems();

	try
	{
		WMI::Connection connection;

//		if (m_logon.empty())
			connection.open(m_host);
//		else
//			connection.open(m_host->m_name, m_logon->m_user, m_logon->m_password);

		WMI::Win32_Service::Iterator end;
		WMI::Win32_Service::Iterator it = WMI::Win32_Service::select(connection);

		for (; it != end; ++it)
		{
			size_t index = m_view.AppendItem(it->DisplayName(), DetermineImage(*it));
			m_view.ItemText(index, STATE,      it->State());
			m_view.ItemText(index, START_MODE, it->StartMode());
		}
	}
	catch (WMI::Exception& e)
	{
		FatalMsg(TXT("Failed to query for the services on '%s':\n\n%s"),
				 m_host.c_str(), e.twhat());
	}

	m_view.Redraw(true);
	m_view.RepaintNow();
}
