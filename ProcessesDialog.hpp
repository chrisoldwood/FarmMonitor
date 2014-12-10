////////////////////////////////////////////////////////////////////////////////
//! \file   ProcessesDialog.hpp
//! \brief  The ProcessesDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef PROCESSESDIALOG_HPP
#define PROCESSESDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to manage the processes on a host.

class ProcessesDialog : public CDialog
{
public:
	//! Constructor.
	ProcessesDialog(const tstring& host);
	
private:
	//! The view columns
	enum Column
	{
		NAME,
		THREADS,
		HANDLES,
	};

	//
	// Members.
	//
	tstring		m_host;		//!< The host to manage.
	
	//
	// Controls.
	//
	CListView	m_view;		//!< The processes view.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! Refresh button handler.
	void onRefreshView();
};

#endif // PROCESSESDIALOG_HPP
