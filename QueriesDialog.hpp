////////////////////////////////////////////////////////////////////////////////
//! \file   QueriesDialog.hpp
//! \brief  The QueriesDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef QUERIESDIALOG_HPP
#define QUERIESDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "Queries.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to edit the set of WMI queries.

class QueriesDialog : public CDialog
{
public:
	//! Default constructor.
	QueriesDialog();
	
	//
	// Members.
	//
	Queries	m_queries;	//! The set of queries.

private:
	//! The view columns
	enum Column
	{
		TITLE,
		WMI_CLASS,
		WMI_PROPERTY,
		FILTER_PROPERTY,
		FILTER_VALUE,
		FORMAT,
	};

	//
	// Members.
	//
	CListView	m_view;		//! The queries view.
	
	//
	// Controls.
	//

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();

	//! Query view selection change handler.
	LRESULT onQuerySelected(NMHDR& header);

	//! Query double-clicked handler.
	LRESULT onQueryDoubleClicked(NMHDR& header);

	//! Add button handler.
	void onAddQuery();

	//! Copy button handler.
	void onCopyQuery();

	//! Edit button handler.
	void onEditQuery();

	//! Delete button handler.
	void onDeleteQuery();

	//! Up button handler.
	void onMoveQueryUp();

	//! Down button handler.
	void onMoveQueryDown();

	//
	// Internal methods.
	//

	//! Update the state of the UI.
	void updateUi();

	//! Add an item to the view.
	void addItemToView(ConstQueryPtr query, bool select = false);

	//! Update an item in the view.
	void updateViewItem(size_t row, ConstQueryPtr query);
};

#endif // QUERIESDIALOG_HPP
