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
#include <WMI/Win32_Service.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to manage the services on a host.

class ServicesDialog : public CDialog
{
public:
	//! Constructor.
	ServicesDialog(const tstring& host);
	
private:
	//! A collection of services.
	typedef std::vector<WMI::Win32_Service> Services;

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
	tstring		m_host;			//!< The host to manage.
	Services	m_services;		//!< The WMI proxies to the services.
	
	//
	// Controls.
	//
	CListView	m_view;			//!< The services view.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! Services view selection change handler.
	LRESULT onServiceSelected(NMHDR& header);

	//! Handle a right-click on the view.
	LRESULT onRightClick(NMHDR& header);

	//! Refresh button handler.
	void onRefreshView();

	//! Start the selected service.
	void onStartService();

	//! Stop the selected service.
	void onStopService();

	//! Restart the selected service.
	void onRestartService();

	//
	// Internal methods.
	//

	//! Update the state of the UI.
	void updateUi();
};

#endif // SERVICESDIALOG_HPP
