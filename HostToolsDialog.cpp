////////////////////////////////////////////////////////////////////////////////
//! \file   HostToolsDialog.cpp
//! \brief  The HostToolsDialog class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "HostToolsDialog.hpp"
#include "HostToolDialog.hpp"

//! The maximum number of tool definitions supported.
const size_t MAX_TOOLS = ID_HOST_INVOKE_TOOL_19 - ID_HOST_INVOKE_TOOL_1 + 1;

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

HostToolsDialog::HostToolsDialog()
	: CDialog(IDD_HOST_TOOLS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_TOOLS, &m_view)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_ADD,    BN_CLICKED,      &HostToolsDialog::onAddTool)
		CMD_CTRLMSG(IDC_COPY,   BN_CLICKED,      &HostToolsDialog::onCopyTool)
		CMD_CTRLMSG(IDC_EDIT,   BN_CLICKED,      &HostToolsDialog::onEditTool)
		CMD_CTRLMSG(IDC_DELETE, BN_CLICKED,      &HostToolsDialog::onDeleteTool)
		CMD_CTRLMSG(IDC_UP,     BN_CLICKED,      &HostToolsDialog::onMoveToolUp)
		CMD_CTRLMSG(IDC_DOWN,   BN_CLICKED,      &HostToolsDialog::onMoveToolDown)
		NFY_CTRLMSG(IDC_TOOLS,  LVN_ITEMCHANGED, &HostToolsDialog::onToolSelected)
		NFY_CTRLMSG(IDC_TOOLS,  NM_DBLCLK,       &HostToolsDialog::onToolDoubleClicked)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void HostToolsDialog::OnInitDialog()
{
	m_view.InsertColumn(TOOL_NAME,    TXT("Tool"),         m_view.StringWidth(25));
	m_view.InsertColumn(COMMAND_LINE, TXT("Command Line"), m_view.StringWidth(45));

	for (Tools::const_iterator it = m_tools.begin(); it != m_tools.end(); ++it)
		addItemToView(*it, (it == m_tools.begin()));

	updateUi();
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool HostToolsDialog::OnOk()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Database view selection change handler.

LRESULT HostToolsDialog::onToolSelected(NMHDR& header)
{
	const NMLISTVIEW& message = reinterpret_cast<const NMLISTVIEW&>(header);

	if (message.uChanged & LVIF_STATE)
		updateUi();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Double-clicked database.

LRESULT HostToolsDialog::onToolDoubleClicked(NMHDR& /*header*/)
{
	if (m_view.IsSelection())
		onEditTool();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Add button handler.

void HostToolsDialog::onAddTool()
{
	HostToolDialog dialog;

	for (Tools::const_iterator it = m_tools.begin(); it != m_tools.end(); ++it)
		dialog.m_usedNames.insert((*it)->m_name);

	if (dialog.RunModal(*this) == IDOK)
	{
		ConstToolPtr tool = makeTool(dialog.m_tool);

		m_tools.append(tool);

		addItemToView(tool, true);
		updateUi();
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Copy button handler.

void HostToolsDialog::onCopyTool()
{
	ASSERT(m_view.IsSelection());

	const size_t       selection = m_view.Selection();
	const ConstToolPtr source = m_tools.tool(selection);

	HostToolDialog dialog;

	dialog.m_tool = *source;

	for (Tools::const_iterator it = m_tools.begin(); it != m_tools.end(); ++it)
		dialog.m_usedNames.insert((*it)->m_name);

	if (dialog.RunModal(*this) == IDOK)
	{
		ConstToolPtr tool = makeTool(dialog.m_tool);

		m_tools.append(tool);

		addItemToView(tool, true);
		updateUi();
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Edit button handler.

void HostToolsDialog::onEditTool()
{
	ASSERT(m_view.IsSelection());

	const size_t       selection = m_view.Selection();
	const ConstToolPtr original = m_tools.tool(selection);

	HostToolDialog dialog;

	dialog.m_tool = *original;

	for (Tools::const_iterator it = m_tools.begin(); it != m_tools.end(); ++it)
	{
		if ((*it)->m_name != original->m_name)
			dialog.m_usedNames.insert((*it)->m_name);
	}

	if (dialog.RunModal(*this) == IDOK)
	{
		const ConstToolPtr edited = makeTool(dialog.m_tool);

		m_tools.replace(selection, edited);

		updateViewItem(selection, edited);
		updateUi();
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Delete button handler.

void HostToolsDialog::onDeleteTool()
{
	ASSERT(m_view.IsSelection());

	const size_t selection = m_view.Selection();

	m_view.DeleteItem(selection, true);
	m_tools.remove(selection);

	updateUi();
}

////////////////////////////////////////////////////////////////////////////////
//! Up button handler.

void HostToolsDialog::onMoveToolUp()
{
	ASSERT(m_view.IsSelection());
	ASSERT(m_view.Selection() != 0);

	const size_t selection = m_view.Selection();

	m_tools.swap(selection, selection-1);

	updateViewItem(selection,   m_tools.tool(selection));
	updateViewItem(selection-1, m_tools.tool(selection-1));

	m_view.Select(selection-1);
}

////////////////////////////////////////////////////////////////////////////////
//! Down button handler.

void HostToolsDialog::onMoveToolDown()
{
	ASSERT(m_view.IsSelection());
	ASSERT(m_view.Selection() != (m_view.ItemCount()-1));

	const size_t selection = m_view.Selection();

	m_tools.swap(selection, selection+1);

	updateViewItem(selection,   m_tools.tool(selection));
	updateViewItem(selection+1, m_tools.tool(selection+1));

	m_view.Select(selection+1);
}

////////////////////////////////////////////////////////////////////////////////
//! Update the state of the UI.

void HostToolsDialog::updateUi()
{
	const bool isSelection = m_view.IsSelection();
	const bool maxDefined = (m_view.ItemCount() == MAX_TOOLS);
	const bool isMoveable = (m_view.ItemCount() > 1);
	const bool isFirstSelected = isSelection && (m_view.Selection() == 0);
	const bool isLastSelected = isSelection && (m_view.Selection() == (m_view.ItemCount()-1));

	Control(IDC_ADD).Enable(!maxDefined);
	Control(IDC_COPY).Enable(!maxDefined && isSelection);
	Control(IDC_EDIT).Enable(isSelection);
	Control(IDC_DELETE).Enable(isSelection);
	Control(IDC_UP).Enable(isMoveable && isSelection && !isFirstSelected);
	Control(IDC_DOWN).Enable(isMoveable && isSelection && !isLastSelected);
}

////////////////////////////////////////////////////////////////////////////////
//! Add an item to the view.

void HostToolsDialog::addItemToView(ConstToolPtr tool, bool select)
{
	const size_t row = m_view.AppendItem(tool->m_name);

	m_view.ItemText(row, COMMAND_LINE, tool->m_commandLine);

	if (select)
		m_view.Select(row);
}

////////////////////////////////////////////////////////////////////////////////
//! Update an item in the view.

void HostToolsDialog::updateViewItem(size_t row, ConstToolPtr tool)
{
	m_view.ItemText(row, TOOL_NAME,    tool->m_name);
	m_view.ItemText(row, COMMAND_LINE, tool->m_commandLine);
}
