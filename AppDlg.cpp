////////////////////////////////////////////////////////////////////////////////
//! \file   AppDlg.cpp
//! \brief  The AppDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppDlg.hpp"
#include "Hosts.hpp"
#include "Tools.hpp"
#include "AppWnd.hpp"
#include <WMI/Connection.hpp>
#include <WMI/Exception.hpp>
#include <WMI/Win32_OperatingSystem.hpp>
#include <WMI/Win32_LogicalDisk.hpp>
#include <WCL/BusyCursor.hpp>
#include <WCL/ContextMenu.hpp>
#include <WCL/ICmdController.hpp>
#include <WCL/BusyCursor.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppDlg::AppDlg(AppWnd& appWnd, WCL::ICmdController& appCmds, Hosts& hosts, Tools& tools)
	: CMainDlg(IDD_MAIN)
	, m_appWnd(appWnd)
	, m_appCmds(appCmds)
	, m_hosts(hosts)
	, m_tools(tools)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_HOSTS,	&m_hostView)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_HOSTS,   LEFT_EDGE, TOP_EDGE,    RIGHT_EDGE, BOTTOM_EDGE)
		CTRLGRAV(IDC_REFRESH, LEFT_EDGE, BOTTOM_EDGE, LEFT_EDGE,  BOTTOM_EDGE)
	END_GRAVITY_TABLE

	DEFINE_CTRLMSG_TABLE
		NFY_CTRLMSG(IDC_HOSTS,   LVN_ITEMCHANGED, &AppDlg::onHostSelectionChanged)
		NFY_CTRLMSG(IDC_HOSTS,   NM_RCLICK,       &AppDlg::onRightClick)
		CMD_CTRLMSG(IDC_REFRESH, BN_CLICKED,      &AppDlg::onRefreshClicked)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AppDlg::~AppDlg()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Is a host currently selected?

bool AppDlg::isHostSelected() const
{
	return m_hostView.IsSelection();
}

////////////////////////////////////////////////////////////////////////////////
//! Get the currently selected host, if available. If not host is selected this
//! returns and empty string.

tstring AppDlg::getSelectedHost() const
{
	tstring hostname;

	size_t selection = m_hostView.Selection();

	if (selection != Core::npos)
		hostname = m_hostView.ItemText(selection, 0);

	return hostname;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the final widths of the main view columns.

const AppDlg::ColumnWidths& AppDlg::getFinalColumnWidths() const
{
	ASSERT(m_hWnd == NULL);

	return m_finalWidths;
}

////////////////////////////////////////////////////////////////////////////////
//! Set the widths of the main view columns.

void AppDlg::setColumnWidths(const ColumnWidths& widths)
{
	ASSERT(widths.size() == m_hostView.NumColumns());

	for (size_t i = 0; i != m_hostView.NumColumns(); ++i)
		m_hostView.ColumnWidth(i, widths[i]);
}

////////////////////////////////////////////////////////////////////////////////
//! Add a new host to be monitored.

void AppDlg::addHost(const tstring& hostname)
{
	const size_t index = m_hosts.add(hostname);

	addHostToView(index);

	if (!m_hostView.IsSelection())
		m_hostView.Select(0);
}
	
////////////////////////////////////////////////////////////////////////////////
//! Rename the currently selected host.

void AppDlg::renameHost(const tstring& hostname)
{
	ASSERT(m_hostView.IsSelection());

	const size_t selection = m_hostView.Selection();

	m_hosts.rename(selection, hostname);
	m_hostView.ItemText(selection, HOST_NAME, hostname);
	clearHost(selection);
}

////////////////////////////////////////////////////////////////////////////////
//! Remove the currently selected host.

void AppDlg::removeSelectedHost()
{
	ASSERT(m_hostView.IsSelection());

	const size_t selection = m_hostView.Selection();

	m_hosts.remove(selection);
	m_hostView.DeleteItem(selection);

	if (m_hostView.ItemCount() != 0)
	{
		const size_t newSelection = (selection == 0) ? 0 : selection-1;

		m_hostView.Select(newSelection);
	}
}
	
////////////////////////////////////////////////////////////////////////////////
//! Handle dialog creation.

void AppDlg::OnInitDialog()
{
	initialiseHostView();
}

////////////////////////////////////////////////////////////////////////////////
//! Handle the dialog being destroyed.

void AppDlg::OnDestroy()
{
	ASSERT(m_finalWidths.size() == 0);

	for (size_t i = 0; i != m_hostView.NumColumns(); ++i)
		m_finalWidths.push_back(m_hostView.ColumnWidth(i));

	CMainDlg::OnDestroy();
}

////////////////////////////////////////////////////////////////////////////////
//! Handle change in host selection.

LRESULT AppDlg::onHostSelectionChanged(NMHDR& header)
{
	const NMLISTVIEW& message = reinterpret_cast<const NMLISTVIEW&>(header);

	if (message.uChanged & LVIF_STATE)
		m_appCmds.UpdateUI();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle a right-click on the hosts view.

LRESULT AppDlg::onRightClick(NMHDR& /*header*/)
{
	const bool isSelection = m_hostView.IsSelection();	
	const uint beginCommandId = ID_HOST_INVOKE_TOOL_1;
	const uint endCommandId = ID_HOST_INVOKE_TOOL_9 + 1;

	WCL::ContextMenu menu(IDR_CONTEXT);

	menu.EnableCmd(ID_HOST_EDITHOST, isSelection);
	menu.EnableCmd(ID_HOST_REMOVEHOST, isSelection);
	menu.EnableCmd(ID_HOST_COPYHOST, isSelection);

	uint commandId = beginCommandId;

	for (Tools::const_iterator it = m_tools.begin(); ( (it != m_tools.end()) && (commandId != endCommandId) );
			++it, ++commandId)
	{
		const uint    ordinal = (commandId - ID_HOST_INVOKE_TOOL_1) + 1;
		const tstring text = Core::fmt(TXT("&%u %s"), ordinal, (*it)->m_name.c_str());

		menu.AppendCmd(commandId, text);
		menu.EnableCmd(commandId, isSelection);
	}

	menu.display(m_appWnd);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle the refresh button being clicked.

void AppDlg::onRefreshClicked()
{
	m_appCmds.Execute(ID_VIEW_REFRESH);
}

////////////////////////////////////////////////////////////////////////////////
//! Initialise the hosts view.

void AppDlg::initialiseHostView()
{
	m_hostView.ImageList(LVSIL_SMALL, IDB_HOST_ICONS, 16, RGB(255, 0, 255));
	m_hostView.FullRowSelect(true);

	m_hostView.InsertColumn(HOST_NAME,        TXT("Host"),       m_hostView.StringWidth(20), LVCFMT_LEFT);
	m_hostView.InsertColumn(TOTAL_MEMORY,     TXT("Total Mem"),  m_hostView.StringWidth(10), LVCFMT_LEFT);
	m_hostView.InsertColumn(FREE_MEMORY,      TXT("Free Mem"),   m_hostView.StringWidth(10), LVCFMT_LEFT);
	m_hostView.InsertColumn(DISK_USAGE,       TXT("C: Usage"),   m_hostView.StringWidth(10), LVCFMT_LEFT);
	m_hostView.InsertColumn(LAST_BOOTUP_TIME, TXT("Rebooted"),   m_hostView.StringWidth(20), LVCFMT_LEFT);
	m_hostView.InsertColumn(LAST_ERROR,       TXT("Last Error"), m_hostView.StringWidth(25), LVCFMT_LEFT);

	for (size_t i = 0; i != m_hosts.size(); ++i)
		addHostToView(i);
}

////////////////////////////////////////////////////////////////////////////////
//! Add a host to the view.

void AppDlg::addHostToView(size_t index)
{
	m_hostView.InsertItem(index, m_hosts.name(index), STATUS_UNKNOWN);
}

////////////////////////////////////////////////////////////////////////////////
//! Formats a memory size value. Memory sizes are KB based.

tstring FormatMemoryValue(uint64 valueInKb)
{
	const double valueInGb = valueInKb / (1024.0 * 1024.0);

	return Core::fmt(TXT("%.1f GB"), valueInGb);
}

////////////////////////////////////////////////////////////////////////////////
//! Formats the amount of disk space used.

tstring FormatDiskUsage(const WMI::Win32_LogicalDisk& disk)
{
	const uint64 diskSize = disk.Size();
	const uint64 freeSpace = disk.FreeSpace();
	const uint64 spaceUsed = diskSize - freeSpace;
	const double percentUsed = (spaceUsed * 100.0) / diskSize;

	return Core::fmt(TXT("%.f %%"), percentUsed);
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh the hosts view.

void AppDlg::refreshView()
{
	CBusyCursor waitCursor;

	for (size_t i = 0; i != m_hosts.size(); ++i)
		refreshHost(i);
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh the view for a single host.

void AppDlg::refreshHost(size_t index)
{
	clearHost(index);

	const tstring& host = m_hosts.name(index);

	try
	{
		WMI::Connection connection(host);

		const WMI::Win32_OperatingSystem::Iterator osIter = WMI::Win32_OperatingSystem::select(connection);

		m_hostView.ItemText(index, TOTAL_MEMORY,     FormatMemoryValue(osIter->TotalVirtualMemorySize()));
		m_hostView.ItemText(index, FREE_MEMORY,      FormatMemoryValue(osIter->FreeVirtualMemory()));
		m_hostView.ItemText(index, LAST_BOOTUP_TIME, osIter->LastBootUpTime().ToString());

		WMI::Win32_LogicalDisk::Iterator diskIter = WMI::Win32_LogicalDisk::select(connection);
		WMI::Win32_LogicalDisk::Iterator diskEnd;

		for (; diskIter != diskEnd; ++diskIter)
		{
			if (diskIter->DeviceID() == TXT("C:"))
			{
				m_hostView.ItemText(index, DISK_USAGE, FormatDiskUsage(*diskIter));
			}
		}

		m_hostView.ItemImage(index, STATUS_GOOD);
	}
	catch (WMI::Exception& e)
	{
		m_hostView.ItemText(index, LAST_ERROR, e.twhat());
		m_hostView.ItemImage(index, STATUS_BAD);
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Clear the results for the host.

void AppDlg::clearHost(size_t index)
{
	m_hostView.ItemText(index, TOTAL_MEMORY,     TXT(""));
	m_hostView.ItemText(index, FREE_MEMORY,      TXT(""));
	m_hostView.ItemText(index, DISK_USAGE,       TXT(""));
	m_hostView.ItemText(index, LAST_BOOTUP_TIME, TXT(""));
	m_hostView.ItemText(index, LAST_ERROR,       TXT(""));
	m_hostView.ItemImage(index, STATUS_UNKNOWN);
}
