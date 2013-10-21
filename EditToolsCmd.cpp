////////////////////////////////////////////////////////////////////////////////
//! \file   EditToolsCmd.cpp
//! \brief  The EditToolsCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "EditToolsCmd.hpp"
#include "HostToolsDialog.hpp"
#include "AppWnd.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

EditToolsCmd::EditToolsCmd(AppWnd& appWnd, Tools& tools)
	: WCL::UiCommandBase(ID_TOOLS_HOSTTOOLS)
	, m_appWnd(appWnd)
	, m_tools(tools)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

void EditToolsCmd::execute()
{
	HostToolsDialog dialog;

	dialog.m_tools.deepCopy(m_tools);

	if (dialog.RunModal(m_appWnd) == IDOK)
	{
		if (dialog.m_tools.isModified())
		{
			m_tools.replaceAll(dialog.m_tools);

			m_appWnd.rebuildToolsMenu();
		}
	}
}
