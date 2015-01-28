////////////////////////////////////////////////////////////////////////////////
//! \file   EditQueryDialog.hpp
//! \brief  The EditQueryDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef EDITQUERYDIALOG_HPP
#define EDITQUERYDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "Query.hpp"
#include <set>

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to edit the defintion for a WMI query.

class EditQueryDialog : public CDialog
{
public:
	//! The set of tool names.
	typedef std::set<tstring> Titles;

public:
	//! Default constructor.
	EditQueryDialog();
	
	//
	// Members.
	//
	Query	m_query;			//!< The query being edited.
	Titles	m_usedTitles;		//!< The set of query titles already in use.

private:
	//! A query template.
	typedef std::pair<tstring, ConstQueryPtr> QueryTemplate;
	//! A collection of query templates.
	typedef std::vector<QueryTemplate> QueryTemplates;

	//
	// Controls.
	//
	CComboBox	m_templateChoice;			//!< The template queries combo box.
	CEditBox	m_titleEditor;				//!< The query title edit control.
	CComboBox	m_wmiClassEditor;			//!< The WMI class name editable combo control.
	CEditBox	m_wmiPropertyEditor;		//!< The WMI class property name edit control.
	CComboBox	m_formatChoice;				//!< The output format combo box.
	CEditBox	m_filterPropertyEditor;		//!< The optional property to filter on edit box.
	CEditBox	m_filterValueEditor;		//!< The optional filter value edit box.

	//
	// Members.
	//
	QueryTemplates	m_queryTemplates;		//!< The list of query templates.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();

	//! Apply button handler.
	void onApplyTemplate();

	//
	// Internal methods.
	//

	//! Build the collection of template queries.
	void buildTemplates();

	//! Populate the set of WMI query templates.
	void populateTemplates();

	//! Populate the set of value formats.
	void populateFormats();

	//! Display the query details.
	void displayQuery(const Query& query);
};

#endif // EDITQUERYDIALOG_HPP
