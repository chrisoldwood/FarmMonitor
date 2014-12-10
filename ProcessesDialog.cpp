////////////////////////////////////////////////////////////////////////////////
//! \file   ProcessesDialog.cpp
//! \brief  The ProcessesDialog class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ProcessesDialog.hpp"
#include "Resource.h"
#include <WCL/BusyCursor.hpp>
#include <WMI/Connection.hpp>
#include <WMI/Exception.hpp>
#include <WMI/Win32_Process.hpp>
#include <Core/StringUtils.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

ProcessesDialog::ProcessesDialog(const tstring& host)
	: CDialog(IDD_PROCESSES)
	, m_host(host)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_PROCESSES, &m_view)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_REFRESH, BN_CLICKED, &ProcessesDialog::onRefreshView)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void ProcessesDialog::OnInitDialog()
{
	m_view.FullRowSelect(true);

	m_view.InsertColumn(NAME,    TXT("Name"),    m_view.StringWidth(25));
	m_view.InsertColumn(THREADS, TXT("Threads"), m_view.StringWidth(10), LVCFMT_RIGHT);
	m_view.InsertColumn(HANDLES, TXT("Handles"), m_view.StringWidth(10), LVCFMT_RIGHT);

	PostCtrlMsg(BN_CLICKED, IDC_REFRESH, CtrlHandle(IDC_REFRESH));
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh button handler.

void ProcessesDialog::onRefreshView()
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

		WMI::Win32_Process::Iterator end;
		WMI::Win32_Process::Iterator it = WMI::Win32_Process::select(connection);

		for (; it != end; ++it)
		{
			size_t index = m_view.AppendItem(it->Name());
			m_view.ItemText(index, THREADS, Core::format(it->ThreadCount()));
			m_view.ItemText(index, HANDLES, Core::format(it->HandleCount()));
		}
	}
	catch (WMI::Exception& e)
	{
		FatalMsg(TXT("Failed to query for the processes on '%s':\n\n%s"),
				 m_host.c_str(), e.twhat());
	}

	m_view.Redraw(true);
	m_view.RepaintNow();
}
