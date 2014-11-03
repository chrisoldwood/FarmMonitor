////////////////////////////////////////////////////////////////////////////////
//! \file   ExecuteToolCmd.cpp
//! \brief  The ExecuteToolCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ExecuteToolCmd.hpp"
#include "Tools.hpp"
#include "AppWnd.hpp"
#include "AppDlg.hpp"
#include "FarmMonitor.hpp"
#include <Core/AnsiWide.hpp>
#include <Core/StringUtils.hpp>
#include <WCL/ContextMenu.hpp>

//! The host name pattern.
static const tstring HOSTNAME = TXT("${HostName}");

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

ExecuteToolCmd::ExecuteToolCmd(Tools& tools, uint index, AppWnd& appWnd, AppDlg& appDlg)
	: WCL::UiCommandBase(ID_HOST_INVOKE_TOOL_1+index)
	, m_tools(tools)
	, m_index(index)
	, m_appWnd(appWnd)
	, m_appDlg(appDlg)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

void ExecuteToolCmd::execute()
{
	ASSERT(m_appDlg.isHostSelected());

	const tstring hostname = m_appDlg.getSelectedHost()->m_name;
	const tstring toolName = m_tools.tool(m_index)->m_name;
	const tstring toolCommand = m_tools.tool(m_index)->m_commandLine;
	const tstring commandLine = Core::replaceNoCase(toolCommand, HOSTNAME, hostname);

	const uint result = ::WinExec(T2A(commandLine), SW_SHOW);

	if (result <= 31)
	{
		const tchar*  error = toString(result);
		const tstring message = Core::fmt(TXT("Failed to execute '%s' for '%s':-\n\n%s [Code: %d]"),
											toolName.c_str(), hostname.c_str(), error, result);

		g_app.FatalMsg(TXT("%s"), message.c_str());
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Refresh the UI for the command.

void ExecuteToolCmd::updateUi()
{
	bool hostSelected = m_appDlg.isHostSelected();

	m_appWnd.Menu()->EnableCmd(id(), hostSelected);
}

////////////////////////////////////////////////////////////////////////////////
//! Rebuild the main Tools menu.

void ExecuteToolCmd::rebuildToolsMainMenu(const Tools& tools, CPopupMenu& menu, bool hostSelected)
{
	const uint beginCommandId = ID_HOST_INVOKE_TOOL_1;
	const uint endCommandId = ID_HOST_INVOKE_TOOL_19 + 1;

	for (uint id = beginCommandId; id != endCommandId; ++id)
		menu.RemoveCmd(id);

	uint commandId = beginCommandId;

	for (Tools::const_iterator it = tools.begin(); ( (it != tools.end()) && (commandId != endCommandId) );
			++it, ++commandId)
	{
		appendCommand(*it, commandId, menu, hostSelected);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Build the tools content menu.

void ExecuteToolCmd::buildToolsContextMenu(const Tools& tools, WCL::ContextMenu& menu, bool hostSelected)
{
	const uint beginCommandId = ID_HOST_INVOKE_TOOL_1;
	const uint endCommandId = ID_HOST_INVOKE_TOOL_19 + 1;

	uint commandId = beginCommandId;

	for (Tools::const_iterator it = tools.begin(); ( (it != tools.end()) && (commandId != endCommandId) );
			++it, ++commandId)
	{
		ExecuteToolCmd::appendCommand(*it, commandId, menu, hostSelected);
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Append the command to the Tools menu.

void ExecuteToolCmd::appendCommand(const ConstToolPtr& tool, uint commandId, CPopupMenu& menu, bool enabled)
{
	const uint    ordinal = (commandId - ID_HOST_INVOKE_TOOL_1) + 1;
	const tchar*  format = (ordinal < 10) ? TXT("&%u %s") : TXT("%u %s");
	const tstring text = Core::fmt(format, ordinal, tool->m_name.c_str());

	menu.AppendCmd(commandId, text);
	menu.EnableCmd(commandId, enabled);
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a ShellExecute error code to a string.

const tchar* ExecuteToolCmd::toString(uint code)
{
	switch (code)
	{
		case 0:							return TXT("The operating system is out of memory or resources.");
		case ERROR_FILE_NOT_FOUND:		return TXT("The specified file was not found.");
		case ERROR_PATH_NOT_FOUND:		return TXT("The specified path was not found.");
		case ERROR_BAD_FORMAT:			return TXT("The .exe file is invalid (non-Microsoft Win32 .exe or error in .exe image).");
		case SE_ERR_ACCESSDENIED:		return TXT("The operating system denied access to the specified file.");
		case SE_ERR_ASSOCINCOMPLETE:	return TXT("The file name association is incomplete or invalid.");
		case SE_ERR_DDEBUSY:			return TXT("The DDE transaction could not be completed because other DDE transactions were being processed.");
		case SE_ERR_DDEFAIL:			return TXT("The DDE transaction failed.");
		case SE_ERR_DDETIMEOUT:			return TXT("The DDE transaction could not be completed because the request timed out.");
		case SE_ERR_DLLNOTFOUND:		return TXT("The specified dynamic-link library (DLL) was not found.");
//		case SE_ERR_FNF:				(same as ERROR_FILE_NOT_FOUND)
		case SE_ERR_NOASSOC:			return TXT("There is no application associated with the given file name extension.");
		case SE_ERR_OOM:				return TXT("There was not enough memory to complete the operation.");
//		case SE_ERR_PNF:				(same as ERROR_PATH_NOT_FOUND)
		case SE_ERR_SHARE:				return TXT("A sharing violation occurred.");
	}

	return TXT("Unknown error code.");
}
