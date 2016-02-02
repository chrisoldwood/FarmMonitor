////////////////////////////////////////////////////////////////////////////////
//! \file   AppDlg.cpp
//! \brief  The AppDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppDlg.hpp"
#include "Model.hpp"
#include "AppWnd.hpp"
#include <WMI/Connection.hpp>
#include <WMI/Exception.hpp>
#include <WMI/Win32_OperatingSystem.hpp>
#include <WMI/Win32_LogicalDisk.hpp>
#include <WCL/BusyCursor.hpp>
#include <WCL/ContextMenu.hpp>
#include <WCL/ICmdController.hpp>
#include <WCL/BusyCursor.hpp>
#include "ExecuteToolCmd.hpp"
#include "QueryRunner.hpp"
#include "LogonDialog.hpp"
#include <Core/Algorithm.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constants.

const tstring LastErrorName = TXT("Last Error");

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppDlg::AppDlg(AppWnd& appWnd, WCL::ICmdController& appCmds, Model& model)
	: CMainDlg(IDD_MAIN)
	, m_appWnd(appWnd)
	, m_appCmds(appCmds)
	, m_model(model)
	, m_finalWidths()
	, m_logons()
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_HOSTS,	&m_hostView)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_HOSTS,   LEFT_EDGE, TOP_EDGE,    RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE

	DEFINE_CTRLMSG_TABLE
		NFY_CTRLMSG(IDC_HOSTS,   LVN_ITEMCHANGED, &AppDlg::onHostSelectionChanged)
		NFY_CTRLMSG(IDC_HOSTS,   NM_RCLICK,       &AppDlg::onRightClick)
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
//! Get the index of the currently selected host, if available. If no host is
//! selected it returns npos;

size_t AppDlg::getSelectedHostIndex() const
{
	return m_hostView.Selection();
}

////////////////////////////////////////////////////////////////////////////////
//! Get the currently selected host, if available. If no host is selected this
//! returns a null host ptr.

ConstHostPtr AppDlg::getSelectedHost() const
{
	ConstHostPtr host;

	size_t selection = m_hostView.Selection();

	if (selection != Core::npos)
		host = m_model.m_hosts.host(selection);

	return host;
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
	const size_t columns = std::min(widths.size(), m_hostView.NumColumns());

	for (size_t i = 0; i != columns; ++i)
		m_hostView.ColumnWidth(i, widths[i]);
}

////////////////////////////////////////////////////////////////////////////////
//! Add a new host to be monitored.

void AppDlg::addHost(ConstHostPtr host)
{
	const size_t index = m_model.m_hosts.add(host);

	addHostToView(index);

	if (!m_hostView.IsSelection())
		m_hostView.Select(0);
}
	
////////////////////////////////////////////////////////////////////////////////
//! Replace the currently selected host.

void AppDlg::replaceHost(ConstHostPtr host)
{
	ASSERT(m_hostView.IsSelection());

	const size_t selection = m_hostView.Selection();
	const bool   renamed = (m_model.m_hosts.host(selection)->m_name != host->m_name);

	m_model.m_hosts.replace(selection, host);

	m_hostView.ItemText(selection, HOST_NAME,   host->m_name);
	m_hostView.ItemText(selection, ENVIRONMENT, host->m_environment);
	m_hostView.ItemText(selection, DESCRIPTION, host->m_description);

	if ( (renamed) || (!host->m_monitor) )
		clearHost(selection);
}

////////////////////////////////////////////////////////////////////////////////
//! Swap two hosts in the view by index.

void AppDlg::swapHosts(size_t first, size_t second)
{
	for (size_t column = 0; column != m_hostView.NumColumns(); ++column)
	{
		tstring temp = m_hostView.ItemText(first, column);
		m_hostView.ItemText(first, column, m_hostView.ItemText(second, column));
		m_hostView.ItemText(second, column, temp);
	}

	if (m_hostView.Selection() == first)
		m_hostView.Select(second);
	else if (m_hostView.Selection() == second)
		m_hostView.Select(first);
}

////////////////////////////////////////////////////////////////////////////////
//! Remove the currently selected host.

void AppDlg::removeSelectedHost()
{
	ASSERT(m_hostView.IsSelection());

	const size_t selection = m_hostView.Selection();

	m_model.m_hosts.remove(selection);
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
	const bool       isSelection = m_hostView.IsSelection();	
	WCL::ContextMenu menu(IDR_CONTEXT);

	const bool isMoveable = (m_hostView.ItemCount() > 1);
	const bool isFirstSelected = isSelection && (m_hostView.Selection() == 0);
	const bool isLastSelected = isSelection && (m_hostView.Selection() == (m_hostView.ItemCount()-1));

	menu.EnableCmd(ID_HOST_EDITHOST, isSelection);
	menu.EnableCmd(ID_HOST_REMOVEHOST, isSelection);
	menu.EnableCmd(ID_HOST_UP, isMoveable && isSelection && !isFirstSelected);
	menu.EnableCmd(ID_HOST_DOWN, isMoveable && isSelection && !isLastSelected);
	menu.EnableCmd(ID_HOST_COPYHOST, isSelection);
	menu.EnableCmd(ID_HOST_SERVICES, isSelection);
	menu.EnableCmd(ID_HOST_PROCESSES, isSelection);

	ExecuteToolCmd::buildToolsContextMenu(m_model.m_tools, menu, isSelection);

	menu.display(m_appWnd);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Initialise the hosts view.

void AppDlg::initialiseHostView()
{
	m_hostView.ImageList(LVSIL_SMALL, IDB_HOST_ICONS, 16, RGB(255, 0, 255));
	m_hostView.FullRowSelect(true);

	m_hostView.InsertColumn(HOST_NAME,   TXT("Host"),        m_hostView.StringWidth(20), LVCFMT_LEFT);
	m_hostView.InsertColumn(ENVIRONMENT, TXT("Environment"), m_hostView.StringWidth(15), LVCFMT_LEFT);
	m_hostView.InsertColumn(DESCRIPTION, TXT("Description"), m_hostView.StringWidth(15), LVCFMT_LEFT);

	for (size_t column = 0; column != m_model.m_queries.size(); ++column)
		m_hostView.InsertColumn(column+CUSTOM_BASE, m_model.m_queries.query(column)->m_title, m_hostView.StringWidth(10), LVCFMT_RIGHT);

	const size_t LAST_ERROR = CUSTOM_BASE+m_model.m_queries.size();

	m_hostView.InsertColumn(LAST_ERROR, LastErrorName, m_hostView.StringWidth(25), LVCFMT_LEFT);

	for (size_t i = 0; i != m_model.m_hosts.size(); ++i)
		addHostToView(i);
}

////////////////////////////////////////////////////////////////////////////////
//! Add a host to the view.

void AppDlg::addHostToView(size_t index)
{
	m_hostView.InsertItem(index,              m_model.m_hosts.host(index)->m_name, STATUS_UNKNOWN);
	m_hostView.ItemText  (index, ENVIRONMENT, m_model.m_hosts.host(index)->m_environment);
	m_hostView.ItemText  (index, DESCRIPTION, m_model.m_hosts.host(index)->m_description);
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
//! Rebuild the view.

void AppDlg::rebuildView()
{
	typedef std::map<tstring, size_t> NameWidthMap;

	NameWidthMap columnWidths;

	const size_t numColumns = m_hostView.NumColumns();

	for (size_t column = CUSTOM_BASE; column != numColumns; ++column)
		columnWidths[m_hostView.ColumnName(column)] = m_hostView.ColumnWidth(column);

	NameWidthMap::const_iterator end = columnWidths.end();

	for (size_t column = CUSTOM_BASE; column != numColumns; ++column)
		m_hostView.DeleteColumn(CUSTOM_BASE);

	const size_t defaultWidth = m_hostView.StringWidth(10);

	for (size_t column = 0; column != m_model.m_queries.size(); ++column)
	{
		ConstQueryPtr query = m_model.m_queries.query(column);
		const size_t  width = Core::findOrDefault(columnWidths, query->m_title, defaultWidth);

		m_hostView.InsertColumn(column+CUSTOM_BASE, query->m_title, width, LVCFMT_RIGHT);
	}

	const size_t LAST_ERROR = CUSTOM_BASE+m_model.m_queries.size();

	m_hostView.InsertColumn(LAST_ERROR, LastErrorName, columnWidths[LastErrorName], LVCFMT_LEFT);
}
	
////////////////////////////////////////////////////////////////////////////////
//! Refresh the hosts view.

void AppDlg::refreshView()
{
	CBusyCursor waitCursor;
	bool        abort = false;

	for (size_t i = 0; (i != m_model.m_hosts.size()) && (!abort); ++i)
	{
		if (!refreshHost(i))
			abort = true;
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh the view for a single host. Returns true to continue or false if
//! the refreshing is to be aborted.

bool AppDlg::refreshHost(size_t index)
{
	clearHost(index);

	ConstHostPtr host = m_model.m_hosts.host(index);

	if (!host->m_monitor)
		return true;

	ConstLogonPtr logon;
//	bool          newLogon = false;

	if (!host->m_logon.empty())
	{
		logon = m_logons.find(host->m_logon);

		if (logon.empty())
		{
			LogonDialog dialog;

			dialog.m_logon.m_user = host->m_logon;

			if (dialog.RunModal(m_appWnd) != IDOK)
				return false;

//			logon = makeLogon(dialog.m_logon);
//			newLogon = true;
		}
	}

	const size_t LAST_ERROR = CUSTOM_BASE+m_model.m_queries.size();

	try
	{
		QueryRunner::const_iterator begin = m_model.m_queries.begin();
		QueryRunner::const_iterator end = m_model.m_queries.end();

		WMI::Connection connection;

		if (logon.empty())
			connection.open(host->m_name);
		else
			connection.open(host->m_name, logon->m_user, logon->m_password);

//		if (newLogon)
//			m_logons.set(logon);

		QueryRunner::Results results = QueryRunner::run(connection, begin, end);

		for (size_t column = 0; column != results.size(); ++column)
			m_hostView.ItemText(index, column+CUSTOM_BASE, results[column]);

		m_hostView.ItemImage(index, STATUS_GOOD);
	}
	catch (const WMI::Exception& e)
	{
		m_hostView.ItemText(index, LAST_ERROR, e.twhat());
		m_hostView.ItemImage(index, STATUS_BAD);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Clear the results for the host.

void AppDlg::clearHost(size_t index)
{
	const size_t numColumns = m_hostView.NumColumns();

	for (size_t column = CUSTOM_BASE; column != numColumns; ++column)
		m_hostView.ItemText(index, column+CUSTOM_BASE, TXT(""));

	m_hostView.ItemImage(index, STATUS_UNKNOWN);
}
