////////////////////////////////////////////////////////////////////////////////
//! \file   ServicesDialog.hpp
//! \brief  The ServicesDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef SERVICESDIALOG_HPP
#define SERVICESDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to manage the services on a host.

class ServicesDialog : public CDialog
{
public:
	//! Constructor.
	ServicesDialog(const tstring& host);
	
private:
	//! The view columns
	enum Column
	{
		NAME,
		STATE,
		START_MODE,
	};

	//
	// Members.
	//
	tstring		m_host;		//!< The host to manage.
	
	//
	// Controls.
	//
	CListView	m_view;		//!< The services view.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! Refresh button handler.
	void onRefreshView();
};

#endif // SERVICESDIALOG_HPP
