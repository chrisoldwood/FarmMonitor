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
#include <WMI/Win32_Process.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to manage the processes on a host.

class ProcessesDialog : public CDialog
{
public:
	//! Constructor.
	ProcessesDialog(const tstring& host);
	
private:
	//! A collection of processes.
	typedef std::vector<WMI::Win32_Process> Processes;

	//! The view columns
	enum Column
	{
		NAME,
		ID,
		THREAD_COUNT,
		HANDLE_COUNT,
		WORKING_SET,
		PRIVATE_PAGES,
		VIRTUAL_SIZE,
		COMMAND_LINE,
	};

	//
	// Members.
	//
	tstring		m_host;			//!< The host to manage.
	Processes	m_processes;	//!< The WMI proxies to the processes.
	
	//
	// Controls.
	//
	CListView	m_view;			//!< The processes view.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! View selection change handler.
	LRESULT onProcessSelected(NMHDR& header);

	//! Handle a right-click on the view.
	LRESULT onRightClick(NMHDR& header);

	//! Refresh button handler.
	void onRefreshView();

	//! Terminate the selected process.
	void onTerminateProcess();

	//
	// Internal methods.
	//

	//! Update the state of the UI.
	void updateUi();
};

#endif // PROCESSESDIALOG_HPP
