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
#include "Hosts.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The application main dialog. This is the dialog that sits in the centre of
//! application window.

class AppDlg : public CMainDlg
{
public:
	//! Constructor.
	AppDlg(HostsPtr hosts);

	//! Destructor.
	virtual ~AppDlg();

	//! Is a host currently selected?
	bool isHostSelected() const;

	//! Add a new host to be monitored.
	void addHost(const tstring& hostname);
	
	//! Remove the currently selected host.
	void removeSelectedHost();
	
private:
	//
	// Members.
	//
	HostsPtr	m_hosts;		//!< The hosts being monitored.

	//
	// Controls.
	//
	CListView	m_hostView;		//!< The main display.

	//
	// Message processors.
	//

	//! Handle dialog creation.
	virtual void OnInitDialog();

	//! Handle change in host selection.
	LRESULT onHostSelectionChanged(NMHDR& header);

	//
	// Internal methods.
	//

	//! Initialise the hosts view.
	void initialiseHostView();

	//! Add a host to the view.
	void addHostToView(size_t index);

	//! Refresh the hosts view.
	void refreshView();

	//! Refresh the view for a single host.
	void refreshHost(size_t index);

	//! Clear the results for the host.
	void clearHost(size_t index);

	//! The hosts view columns.
	enum Column
	{
		HOST_NAME,
		TOTAL_MEMORY,
		FREE_MEMORY,
		DISK_USAGE,
		LAST_BOOTUP_TIME,
		LAST_ERROR,

		NUM_COLUMNS,
	};
};

#endif // APP_APPDLG_HPP
