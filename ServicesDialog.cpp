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
#include <WCL/ContextMenu.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constants.

const DWORD DEFAULT_TIMEOUT = 30000;

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

ServicesDialog::ServicesDialog(const tstring& host)
	: CDialog(IDD_SERVICES)
	, m_host(host)
	, m_services()
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_SERVICES, &m_view)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		NFY_CTRLMSG(IDC_SERVICES, LVN_ITEMCHANGED, &ServicesDialog::onServiceSelected)
		NFY_CTRLMSG(IDC_SERVICES, NM_RCLICK,       &ServicesDialog::onRightClick)
		CMD_CTRLMSG(IDC_REFRESH,  BN_CLICKED,      &ServicesDialog::onRefreshView)
		CMD_CTRLMSG(IDC_START,    BN_CLICKED,      &ServicesDialog::onStartService)
		CMD_CTRLMSG(IDC_STOP,     BN_CLICKED,      &ServicesDialog::onStopService)
		CMD_CTRLMSG(IDC_RESTART,  BN_CLICKED,      &ServicesDialog::onRestartService)
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

	updateUi();

	PostCtrlMsg(BN_CLICKED, IDC_REFRESH, CtrlHandle(IDC_REFRESH));
}

////////////////////////////////////////////////////////////////////////////////
//! Determine the icon from the current state and startup type.

static size_t DetermineImage(const WMI::Win32_Service& service)
{
	if (service.IsDisabled())
		return 0;

	if (service.IsRunning())
		return 1;

	if (service.IsAutomatic() && !service.IsRunning())
		return 3;

	return 2;
} 

////////////////////////////////////////////////////////////////////////////////
//! Services view selection change handler.

LRESULT ServicesDialog::onServiceSelected(NMHDR& header)
{
	const NMLISTVIEW& message = reinterpret_cast<const NMLISTVIEW&>(header);

	if (message.uChanged & LVIF_STATE)
		updateUi();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle a right-click on the view.

LRESULT ServicesDialog::onRightClick(NMHDR& header)
{
	if (m_view.IsSelection())
	{
		WCL::ContextMenu menu(IDR_SERVICES);

		const size_t selection = m_view.ItemData(m_view.Selection());
		const WMI::Win32_Service service = m_services[selection];
		const bool stopped = service.IsStopped();
		const bool running = service.IsRunning();
		
		menu.EnableCmd(IDC_START, stopped);
		menu.EnableCmd(IDC_STOP, running);
		menu.EnableCmd(IDC_RESTART, running);

		const NMITEMACTIVATE& message = reinterpret_cast<NMITEMACTIVATE&>(header);
		const CPoint position = m_view.calcMsgMousePos(message);
		const uint   command = menu.TrackMenu(m_view, position);

		if (command != 0)
			PostCtrlMsg(BN_CLICKED, command, CtrlHandle(command));
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh button handler.

void ServicesDialog::onRefreshView()
{
	CBusyCursor waitCursor;

	const size_t selection = m_view.Selection();

	m_view.Redraw(false);
	m_view.DeleteAllItems();
	m_services.clear();

	try
	{
		WMI::Connection connection;

//		if (m_logon.empty())
			connection.open(m_host);
//		else
//			connection.open(m_host->m_name, m_logon->m_user, m_logon->m_password);

		WMI::Win32_Service::Iterator end;
		WMI::Win32_Service::Iterator it = WMI::Win32_Service::selectAll(connection);

		for (size_t i = 0; it != end; ++it, ++i)
		{
			m_services.push_back(*it);

			size_t index = m_view.AppendItem(it->DisplayName(), DetermineImage(*it));
			m_view.ItemText(index, STATE,      it->State());
			m_view.ItemText(index, START_MODE, it->StartMode());
			m_view.ItemData(index, i);
		}
	}
	catch (WMI::Exception& e)
	{
		FatalMsg(TXT("Failed to query for the services on '%s':\n\n%s"),
				 m_host.c_str(), e.twhat());
	}

	ASSERT(m_services.size() == m_view.ItemCount());

	if (selection != Core::npos)
	{
		m_view.Select(selection);
		m_view.MakeItemVisible(selection);
	}

	m_view.Redraw(true);
	m_view.RepaintNow();
	updateUi();
}

////////////////////////////////////////////////////////////////////////////////
//! Start the service.

bool StartService(WMI::Win32_Service& service, CWnd& dialog)
{
	try
	{
		CBusyCursor waitCursor;

		const uint32 result = service.StartService();

		if (result != 0)
			dialog.AlertMsg(TXT("Failed to start the service:\n\nStartService returned: %u"), result);

		DWORD       timeNow = ::GetTickCount();
		const DWORD maxWaitTime = timeNow + DEFAULT_TIMEOUT;

		while ( (!service.IsRunning()) && (timeNow < maxWaitTime) )
		{
			service.refresh();
			timeNow = ::GetTickCount();
		}

		if (service.IsRunning())
			return true;

		dialog.AlertMsg(TXT("The service failed to start within the time allowed"));
	}
	catch (const WMI::Exception& e)
	{
		dialog.AlertMsg(TXT("Failed to start the service:\n\n%s"), e.twhat());
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
//! Start the selected service.

void ServicesDialog::onStartService()
{
	ASSERT(m_view.IsSelection());

	const size_t       selection = m_view.ItemData(m_view.Selection());
	WMI::Win32_Service service = m_services[selection];

	StartService(service, *this);

	onRefreshView();
}

////////////////////////////////////////////////////////////////////////////////
//! Stop the service.

bool StopService(WMI::Win32_Service& service, CWnd& dialog)
{
	try
	{
		CBusyCursor waitCursor;

		const uint32 result = service.StopService();

		if (result != 0)
			dialog.AlertMsg(TXT("Failed to stop the service:\n\nStopService returned: %u"), result);

		DWORD       timeNow = ::GetTickCount();
		const DWORD maxWaitTime = timeNow + DEFAULT_TIMEOUT;

		while ( (!service.IsStopped()) && (timeNow < maxWaitTime) )
		{
			service.refresh();
			timeNow = ::GetTickCount();
		}

		if (service.IsStopped())
			return true;

		dialog.AlertMsg(TXT("The service failed to stop within the time allowed"));
	}
	catch (const WMI::Exception& e)
	{
		dialog.AlertMsg(TXT("Failed to stop the service:\n\n%s"), e.twhat());
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
//! Stop the selected service.

void ServicesDialog::onStopService()
{
	ASSERT(m_view.IsSelection());

	const size_t       selection = m_view.ItemData(m_view.Selection());
	WMI::Win32_Service service = m_services[selection];

	StopService(service, *this);

	onRefreshView();
}

////////////////////////////////////////////////////////////////////////////////
//! Restart the selected service.

void ServicesDialog::onRestartService()
{
	ASSERT(m_view.IsSelection());

	const size_t       selection = m_view.ItemData(m_view.Selection());
	WMI::Win32_Service service = m_services[selection];

	if (StopService(service, *this))
		StartService(service, *this);

	onRefreshView();
}

////////////////////////////////////////////////////////////////////////////////
//! Update the state of the UI.

void ServicesDialog::updateUi()
{
	if (!m_view.IsSelection())
	{
		Control(IDC_START).Enable(false);
		Control(IDC_STOP).Enable(false);
		Control(IDC_RESTART).Enable(false);
	}
	else
	{
		const size_t selection = m_view.ItemData(m_view.Selection());
		const WMI::Win32_Service service = m_services[selection];
		const bool stopped = service.IsStopped();
		const bool running = service.IsRunning();
		
		Control(IDC_START).Enable(stopped);
		Control(IDC_STOP).Enable(running);
		Control(IDC_RESTART).Enable(running);
	}
}
