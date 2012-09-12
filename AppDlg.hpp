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

// Forward declarations.
namespace WCL
{
class ICmdController;
}
class AppWnd;
class Hosts;
class Tools;

////////////////////////////////////////////////////////////////////////////////
//! The application main dialog. This is the dialog that sits in the centre of
//! application window.

class AppDlg : public CMainDlg
{
public:
	//! Constructor.
	AppDlg(AppWnd& appWnd, WCL::ICmdController& appCmds, Hosts& hosts, Tools& tools);

	//! Destructor.
	virtual ~AppDlg();

	//! Is a host currently selected?
	bool isHostSelected() const;

	//! Get the currently selected host, if available.
	tstring getSelectedHost() const;

	//! Add a new host to be monitored.
	void addHost(const tstring& hostname);
	
	//! Remove the currently selected host.
	void removeSelectedHost();
	
private:
	//
	// Members.
	//
	AppWnd&					m_appWnd;		//!< The main window.
	WCL::ICmdController&	m_appCmds;		//!< The command controller.
	Hosts&					m_hosts;		//!< The hosts being monitored.
	Tools&					m_tools;		//!< The set of external tools.

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

	//! Handle a right-click on the hosts view.
	LRESULT onRightClick(NMHDR& header);

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
