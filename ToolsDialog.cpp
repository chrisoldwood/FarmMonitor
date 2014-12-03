////////////////////////////////////////////////////////////////////////////////
//! \file   ToolsDialog.cpp
//! \brief  The ToolsDialog class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ToolsDialog.hpp"
#include "EditToolDialog.hpp"

//! The maximum number of tool definitions supported.
const size_t MAX_TOOLS = ID_HOST_INVOKE_TOOL_19 - ID_HOST_INVOKE_TOOL_1 + 1;

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

ToolsDialog::ToolsDialog()
	: CDialog(IDD_HOST_TOOLS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_TOOLS, &m_view)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_ADD,    BN_CLICKED,      &ToolsDialog::onAddTool)
		CMD_CTRLMSG(IDC_COPY,   BN_CLICKED,      &ToolsDialog::onCopyTool)
		CMD_CTRLMSG(IDC_EDIT,   BN_CLICKED,      &ToolsDialog::onEditTool)
		CMD_CTRLMSG(IDC_DELETE, BN_CLICKED,      &ToolsDialog::onDeleteTool)
		CMD_CTRLMSG(IDC_UP,     BN_CLICKED,      &ToolsDialog::onMoveToolUp)
		CMD_CTRLMSG(IDC_DOWN,   BN_CLICKED,      &ToolsDialog::onMoveToolDown)
		NFY_CTRLMSG(IDC_TOOLS,  LVN_ITEMCHANGED, &ToolsDialog::onToolSelected)
		NFY_CTRLMSG(IDC_TOOLS,  NM_DBLCLK,       &ToolsDialog::onToolDoubleClicked)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void ToolsDialog::OnInitDialog()
{
	m_view.InsertColumn(TOOL_NAME,    TXT("Tool"),         m_view.StringWidth(25));
	m_view.InsertColumn(COMMAND_LINE, TXT("Command Line"), m_view.StringWidth(45));

	for (Tools::const_iterator it = m_tools.begin(); it != m_tools.end(); ++it)
		addItemToView(*it, (it == m_tools.begin()));

	updateUi();
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool ToolsDialog::OnOk()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Tools view selection change handler.

LRESULT ToolsDialog::onToolSelected(NMHDR& header)
{
	const NMLISTVIEW& message = reinterpret_cast<const NMLISTVIEW&>(header);

	if (message.uChanged & LVIF_STATE)
		updateUi();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Double-clicked tool.

LRESULT ToolsDialog::onToolDoubleClicked(NMHDR& /*header*/)
{
	if (m_view.IsSelection())
		onEditTool();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Add button handler.

void ToolsDialog::onAddTool()
{
	EditToolDialog dialog;

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

void ToolsDialog::onCopyTool()
{
	ASSERT(m_view.IsSelection());

	const size_t       selection = m_view.Selection();
	const ConstToolPtr source = m_tools.tool(selection);

	EditToolDialog dialog;

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

void ToolsDialog::onEditTool()
{
	ASSERT(m_view.IsSelection());

	const size_t       selection = m_view.Selection();
	const ConstToolPtr original = m_tools.tool(selection);

	EditToolDialog dialog;

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

void ToolsDialog::onDeleteTool()
{
	ASSERT(m_view.IsSelection());

	const size_t selection = m_view.Selection();

	m_view.DeleteItem(selection, true);
	m_tools.remove(selection);

	updateUi();
}

////////////////////////////////////////////////////////////////////////////////
//! Up button handler.

void ToolsDialog::onMoveToolUp()
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

void ToolsDialog::onMoveToolDown()
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

void ToolsDialog::updateUi()
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

void ToolsDialog::addItemToView(ConstToolPtr tool, bool select)
{
	const size_t row = m_view.AppendItem(tool->m_name);

	updateViewItem(row, tool);

	if (select)
		m_view.Select(row);
}

////////////////////////////////////////////////////////////////////////////////
//! Update an item in the view.

void ToolsDialog::updateViewItem(size_t row, ConstToolPtr tool)
{
	m_view.ItemText(row, TOOL_NAME,    tool->m_name);
	m_view.ItemText(row, COMMAND_LINE, tool->m_commandLine);
}
