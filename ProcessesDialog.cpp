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
#include <Core/StringUtils.hpp>
#include <WCL/ContextMenu.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

ProcessesDialog::ProcessesDialog(const tstring& host)
	: CDialog(IDD_PROCESSES)
	, m_host(host)
	, m_processes()
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_PROCESSES, &m_view)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		NFY_CTRLMSG(IDC_PROCESSES, LVN_ITEMCHANGED, &ProcessesDialog::onProcessSelected)
		NFY_CTRLMSG(IDC_PROCESSES, NM_RCLICK,       &ProcessesDialog::onRightClick)
		CMD_CTRLMSG(IDC_REFRESH,   BN_CLICKED,      &ProcessesDialog::onRefreshView)
		CMD_CTRLMSG(IDC_TERMINATE, BN_CLICKED,      &ProcessesDialog::onTerminateProcess)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void ProcessesDialog::OnInitDialog()
{
	m_view.FullRowSelect(true);

	m_view.InsertColumn(NAME,          TXT("Name"),         m_view.StringWidth(25), LVCFMT_LEFT);
	m_view.InsertColumn(ID,	           TXT("ID"),           m_view.StringWidth(10), LVCFMT_RIGHT);
	m_view.InsertColumn(THREAD_COUNT,  TXT("Threads"),      m_view.StringWidth(10), LVCFMT_RIGHT);
	m_view.InsertColumn(HANDLE_COUNT,  TXT("Handles"),      m_view.StringWidth(10), LVCFMT_RIGHT);
	m_view.InsertColumn(WORKING_SET,   TXT("Work. Set"),    m_view.StringWidth(10), LVCFMT_RIGHT);
	m_view.InsertColumn(PRIVATE_PAGES, TXT("Priv. Size"),   m_view.StringWidth(10), LVCFMT_RIGHT);
	m_view.InsertColumn(VIRTUAL_SIZE,  TXT("Virt. Size"),   m_view.StringWidth(10), LVCFMT_RIGHT);
	m_view.InsertColumn(COMMAND_LINE,  TXT("Command Line"), m_view.StringWidth(20), LVCFMT_LEFT);

	updateUi();

	PostCtrlMsg(BN_CLICKED, IDC_REFRESH, CtrlHandle(IDC_REFRESH));
}

////////////////////////////////////////////////////////////////////////////////
//! View selection change handler.

LRESULT ProcessesDialog::onProcessSelected(NMHDR& header)
{
	const NMLISTVIEW& message = reinterpret_cast<const NMLISTVIEW&>(header);

	if (message.uChanged & LVIF_STATE)
		updateUi();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle a right-click on the view.

LRESULT ProcessesDialog::onRightClick(NMHDR& header)
{
	if (m_view.IsSelection())
	{
		WCL::ContextMenu menu(IDR_PROCESSES);

		menu.EnableCmd(IDC_TERMINATE, true);

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

void ProcessesDialog::onRefreshView()
{
	CBusyCursor waitCursor;

	const size_t selection = m_view.Selection();

	m_view.Redraw(false);
	m_view.DeleteAllItems();
	m_processes.clear();

	try
	{
		WMI::Connection connection;

//		if (m_logon.empty())
			connection.open(m_host);
//		else
//			connection.open(m_host->m_name, m_logon->m_user, m_logon->m_password);

		WMI::Win32_Process::Iterator end;
		WMI::Win32_Process::Iterator it = WMI::Win32_Process::select(connection);

		for (size_t i = 0; it != end; ++it, ++i)
		{
			m_processes.push_back(*it);

			size_t index = m_view.AppendItem(it->Name());
			m_view.ItemText(index, ID,            Core::format(it->ProcessId()));
			m_view.ItemText(index, THREAD_COUNT,  Core::format(it->ThreadCount()));
			m_view.ItemText(index, HANDLE_COUNT,  Core::format(it->HandleCount()));
			m_view.ItemText(index, WORKING_SET,   Core::format(it->WorkingSetSize() / (1024*1024)) + TXT(" MB"));
			m_view.ItemText(index, PRIVATE_PAGES, Core::format(it->PrivatePageCount() / (1024*1024)) + TXT(" MB"));
			m_view.ItemText(index, VIRTUAL_SIZE,  Core::format(it->VirtualSize() / (1024*1024)) + TXT(" MB"));
			m_view.ItemText(index, COMMAND_LINE,  it->CommandLine());
			m_view.ItemData(index, i);

		}
	}
	catch (WMI::Exception& e)
	{
		FatalMsg(TXT("Failed to query for the processes on '%s':\n\n%s"),
				 m_host.c_str(), e.twhat());
	}

	ASSERT(m_processes.size() == m_view.ItemCount());

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
//! Terminate the selected process.

void ProcessesDialog::onTerminateProcess()
{
	ASSERT(m_view.IsSelection());

	try
	{
		CBusyCursor waitCursor;

		const size_t       selection = m_view.ItemData(m_view.Selection());
		WMI::Win32_Process process = m_processes[selection];

		const uint32 result = process.Terminate(0);

		if (result != 0)
			AlertMsg(TXT("Failed to terminate the process:\n\nTerminate returned: %u"), result);
	}
	catch (const WMI::Exception& e)
	{
		AlertMsg(TXT("Failed to terminate the process:\n\n%s"), e.twhat());
	}

	onRefreshView();
}

////////////////////////////////////////////////////////////////////////////////
//! Update the state of the UI.

void ProcessesDialog::updateUi()
{
	Control(IDC_TERMINATE).Enable(m_view.IsSelection());
}
