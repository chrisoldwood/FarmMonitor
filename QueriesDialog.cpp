////////////////////////////////////////////////////////////////////////////////
//! \file   QueriesDialog.cpp
//! \brief  The QueriesDialog class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "QueriesDialog.hpp"
#include "Resource.h"
#include "QueryDialog.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

QueriesDialog::QueriesDialog()
	: CDialog(IDD_QUERIES)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_QUERIES, &m_view)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_ADD,      BN_CLICKED,      &QueriesDialog::onAddQuery)
		CMD_CTRLMSG(IDC_COPY,     BN_CLICKED,      &QueriesDialog::onCopyQuery)
		CMD_CTRLMSG(IDC_EDIT,     BN_CLICKED,      &QueriesDialog::onEditQuery)
		CMD_CTRLMSG(IDC_DELETE,   BN_CLICKED,      &QueriesDialog::onDeleteQuery)
		CMD_CTRLMSG(IDC_UP,       BN_CLICKED,      &QueriesDialog::onMoveQueryUp)
		CMD_CTRLMSG(IDC_DOWN,     BN_CLICKED,      &QueriesDialog::onMoveQueryDown)
		NFY_CTRLMSG(IDC_QUERIES,  LVN_ITEMCHANGED, &QueriesDialog::onQuerySelected)
		NFY_CTRLMSG(IDC_QUERIES,  NM_DBLCLK,       &QueriesDialog::onQueryDoubleClicked)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void QueriesDialog::OnInitDialog()
{
	m_view.InsertColumn(TITLE,           TXT("Title"),           m_view.StringWidth(15));
	m_view.InsertColumn(WMI_CLASS,       TXT("WMI Class"),       m_view.StringWidth(35));
	m_view.InsertColumn(WMI_PROPERTY,    TXT("WMI Property"),    m_view.StringWidth(20));
	m_view.InsertColumn(FILTER_PROPERTY, TXT("Filter Property"), m_view.StringWidth(20));
	m_view.InsertColumn(FILTER_VALUE,    TXT("Filter Value"),    m_view.StringWidth(15));
	m_view.InsertColumn(FORMAT,          TXT("Format"),          m_view.StringWidth(10));

	for (Queries::const_iterator it = m_queries.begin(); it != m_queries.end(); ++it)
		addItemToView(*it, (it == m_queries.begin()));

	updateUi();
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool QueriesDialog::OnOk()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Queries view selection change handler.

LRESULT QueriesDialog::onQuerySelected(NMHDR& header)
{
	const NMLISTVIEW& message = reinterpret_cast<const NMLISTVIEW&>(header);

	if (message.uChanged & LVIF_STATE)
		updateUi();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Double-clicked query.

LRESULT QueriesDialog::onQueryDoubleClicked(NMHDR& /*header*/)
{
	if (m_view.IsSelection())
		onEditQuery();

	return 0;
}


////////////////////////////////////////////////////////////////////////////////
//! Add button handler.

void QueriesDialog::onAddQuery()
{
/*	QueryDialog dialog;

	for (Tools::const_iterator it = m_tools.begin(); it != m_tools.end(); ++it)
		dialog.m_usedNames.insert((*it)->m_name);

	if (dialog.RunModal(*this) == IDOK)
	{
		ConstToolPtr tool = makeTool(dialog.m_tool);

		m_tools.append(tool);

		addItemToView(tool, true);
		updateUi();
	}*/
}

////////////////////////////////////////////////////////////////////////////////
//! Copy button handler.

void QueriesDialog::onCopyQuery()
{
	ASSERT(m_view.IsSelection());

	const size_t        selection = m_view.Selection();
	const ConstQueryPtr source = m_queries.query(selection);
/*
	QueryDialog dialog;

	dialog.m_tool = *source;

	for (Tools::const_iterator it = m_tools.begin(); it != m_tools.end(); ++it)
		dialog.m_usedNames.insert((*it)->m_name);

	if (dialog.RunModal(*this) == IDOK)
	{
		ConstToolPtr tool = makeTool(dialog.m_tool);

		m_tools.append(tool);

		addItemToView(tool, true);
		updateUi();
	}*/
}

////////////////////////////////////////////////////////////////////////////////
//! Edit button handler.

void QueriesDialog::onEditQuery()
{
	ASSERT(m_view.IsSelection());

	const size_t        selection = m_view.Selection();
	const ConstQueryPtr original = m_queries.query(selection);
/*
	QueryDialog dialog;

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
	}*/
}

////////////////////////////////////////////////////////////////////////////////
//! Delete button handler.

void QueriesDialog::onDeleteQuery()
{
	ASSERT(m_view.IsSelection());

	const size_t selection = m_view.Selection();

	m_view.DeleteItem(selection, true);
	m_queries.remove(selection);

	updateUi();
}

////////////////////////////////////////////////////////////////////////////////
//! Up button handler.

void QueriesDialog::onMoveQueryUp()
{
	ASSERT(m_view.IsSelection());
	ASSERT(m_view.Selection() != 0);

	const size_t selection = m_view.Selection();

	m_queries.swap(selection, selection-1);

	updateViewItem(selection,   m_queries.query(selection));
	updateViewItem(selection-1, m_queries.query(selection-1));

	m_view.Select(selection-1);
}

////////////////////////////////////////////////////////////////////////////////
//! Down button handler.

void QueriesDialog::onMoveQueryDown()
{
	ASSERT(m_view.IsSelection());
	ASSERT(m_view.Selection() != (m_view.ItemCount()-1));

	const size_t selection = m_view.Selection();

	m_queries.swap(selection, selection+1);

	updateViewItem(selection,   m_queries.query(selection));
	updateViewItem(selection+1, m_queries.query(selection+1));

	m_view.Select(selection+1);
}

////////////////////////////////////////////////////////////////////////////////
//! Update the state of the UI.

void QueriesDialog::updateUi()
{
	const bool isSelection = m_view.IsSelection();
	const bool isMoveable = (m_view.ItemCount() > 1);
	const bool isFirstSelected = isSelection && (m_view.Selection() == 0);
	const bool isLastSelected = isSelection && (m_view.Selection() == (m_view.ItemCount()-1));

	Control(IDC_ADD).Enable(true);
	Control(IDC_COPY).Enable(isSelection);
	Control(IDC_EDIT).Enable(isSelection);
	Control(IDC_DELETE).Enable(isSelection);
	Control(IDC_UP).Enable(isMoveable && isSelection && !isFirstSelected);
	Control(IDC_DOWN).Enable(isMoveable && isSelection && !isLastSelected);
}

////////////////////////////////////////////////////////////////////////////////
//! Add an item to the view.

void QueriesDialog::addItemToView(ConstQueryPtr query, bool select)
{
	const size_t row = m_view.AppendItem(query->m_title);

	updateViewItem(row, query);

	if (select)
		m_view.Select(row);
}

////////////////////////////////////////////////////////////////////////////////
//! Update an item in the view.

void QueriesDialog::updateViewItem(size_t row, ConstQueryPtr query)
{
	m_view.ItemText(row, TITLE,	          query->m_title);
	m_view.ItemText(row, WMI_CLASS,       query->m_wmiClass);
	m_view.ItemText(row, WMI_PROPERTY,    query->m_wmiProperty);
	m_view.ItemText(row, FILTER_PROPERTY, query->m_filterProperty);
	m_view.ItemText(row, FILTER_VALUE,    query->m_filterValue);
	m_view.ItemText(row, FORMAT,          query->m_format);
}
