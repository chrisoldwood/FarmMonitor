////////////////////////////////////////////////////////////////////////////////
//! \file   AppDlg.hpp
//! \brief  The AppDlg class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_APPDLG_HPP
#define APP_APPDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/MainDlg.hpp>
#include <WCL/ListView.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The application main dialog. This is the dialog that sits in the centre of
//! application window.

class AppDlg : public CMainDlg
{
public:
	//! Constructor.
	AppDlg();

	//! Destructor.
	virtual ~AppDlg();
	
private:
	//
	// Controls.
	//
	CListView	m_hosts;	//!< The main display.

	//
	// Message processors.
	//

	//! Handle dialog creation.
	virtual void OnInitDialog();

	//
	// Internal methods.
	//

	//! Initialise the hosts view.
	void initialiseHostsView();

	//! Refresh the hosts view.
	void refreshView();

	//! The hosts view columns.
	enum Column
	{
		HOSTNAME,
		LAST_BOOTUP_TIME,

		NUM_COLUMNS,
	};
};

#endif // APP_APPDLG_HPP
