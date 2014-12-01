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
#include "Host.hpp"
#include "Credentials.hpp"

// Forward declarations.
namespace WCL
{
class ICmdController;
}
class AppWnd;
class Model;
class Hosts;
class Tools;

////////////////////////////////////////////////////////////////////////////////
//! The application main dialog. This is the dialog that sits in the centre of
//! application window.

class AppDlg : public CMainDlg
{
public:
	//! The column width collection type.
	typedef std::vector<size_t> ColumnWidths;

public:
	//! Constructor.
	AppDlg(AppWnd& appWnd, WCL::ICmdController& appCmds, Model& model);

	//! Destructor.
	virtual ~AppDlg();

	//
	// Properties.
	//

	//! Is a host currently selected?
	bool isHostSelected() const;

	//! Get the index of the currently selected host, if available.
	size_t getSelectedHostIndex() const;

	//! Get the currently selected host, if available.
	ConstHostPtr getSelectedHost() const;

	//! Get the final widths of the main view columns.
	const ColumnWidths& getFinalColumnWidths() const;

	//! Set the widths of the main view columns.
	void setColumnWidths(const ColumnWidths& widths);

	//
	// Methods.
	//

	//! Add a new host to be monitored.
	void addHost(ConstHostPtr host);
	
	//! Replace the currently selected host.
	void replaceHost(ConstHostPtr host);
	
	//! Swap two hosts in the view by index.
	void swapHosts(size_t first, size_t second);

	//! Remove the currently selected host.
	void removeSelectedHost();
	
	//! Refresh the hosts view.
	void refreshView();

private:
	//
	// Members.
	//
	AppWnd&					m_appWnd;		//!< The main window.
	WCL::ICmdController&	m_appCmds;		//!< The command controller.
	Model&					m_model;		//!< The data model.
	ColumnWidths			m_finalWidths;	//!< The final width for each column.
	Credentials				m_logons;		//!< The credential cache.

	//
	// Controls.
	//
	CListView	m_hostView;		//!< The main display.

	//
	// Message processors.
	//

	//! Handle dialog creation.
	virtual void OnInitDialog();

	//! Handle the dialog being destroyed.
	virtual void OnDestroy();

	//! Handle change in host selection.
	LRESULT onHostSelectionChanged(NMHDR& header);

	//! Handle a right-click on the hosts view.
	LRESULT onRightClick(NMHDR& header);

	//
	// Internal methods.
	//

	//! Initialise the hosts view.
	void initialiseHostView();

	//! Add a host to the view.
	void addHostToView(size_t index);

	//! Refresh the view for a single host.
	bool refreshHost(size_t index);

	//! Clear the results for the host.
	void clearHost(size_t index);

	//! The hosts view columns.
	enum Column
	{
		HOST_NAME,
		ENVIRONMENT,
		DESCRIPTION,
		TOTAL_MEMORY,
		FREE_MEMORY,
		DISK_SIZE,
		DISK_FREE,
		LAST_BOOTUP_TIME,
		LAST_ERROR,

		NUM_COLUMNS,
	};

	//! The host status.
	enum HostStatus
	{
		STATUS_UNKNOWN	= 0,
		STATUS_BAD		= 1,
		STATUS_GOOD		= 2,
	};
};

#endif // APP_APPDLG_HPP
