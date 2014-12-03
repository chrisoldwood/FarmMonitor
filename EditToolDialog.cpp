////////////////////////////////////////////////////////////////////////////////
//! \file   EditToolDialog.cpp
//! \brief  The EditToolDialog class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "EditToolDialog.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

EditToolDialog::EditToolDialog()
	: CDialog(IDD_HOST_TOOL)
	, m_tool()
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_NAME,     &m_nameEditor)
		CTRL(IDC_CMD_LINE, &m_cmdLineEditor)
	END_CTRL_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void EditToolDialog::OnInitDialog()
{
	m_cmdLineEditor.Add(TXT("mstsc /f /v ${HOSTNAME}"));
	m_cmdLineEditor.Add(TXT("explorer \\\\${HOSTNAME}\\Share"));

	m_nameEditor.Text(m_tool.m_name);

	if (!m_tool.m_commandLine.empty())
	{
		if (!m_cmdLineEditor.Exists(m_tool.m_commandLine))
			m_cmdLineEditor.Add(m_tool.m_commandLine);

		m_cmdLineEditor.Select(m_tool.m_commandLine);
	}
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool EditToolDialog::OnOk()
{
	m_tool.m_name = m_nameEditor.Text();
	m_tool.m_commandLine = m_cmdLineEditor.Text();

	if (m_tool.m_name.empty())
	{
		AlertMsg(TXT("You must provide a title for the tool."));
		return false;
	}
	
	if (m_usedNames.find(m_tool.m_name) != m_usedNames.end())
	{
		AlertMsg(TXT("There is already a tool with that name defined."));
		return false;
	}
	
	if (m_tool.m_commandLine.empty())
	{
		AlertMsg(TXT("You must supply the command line used to execute the tool."));
		return false;
	}

	return true;
}
