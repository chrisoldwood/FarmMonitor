////////////////////////////////////////////////////////////////////////////////
//! \file   EditQueriesCmd.hpp
//! \brief  The EditQueriesCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_EDITQUERIESCMD_HPP
#define APP_EDITQUERIESCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>

// Forward declarations.
class AppWnd;
class AppDlg;
class Queries;

////////////////////////////////////////////////////////////////////////////////
//! The command to show the dialog for editing the WMI queries.

class EditQueriesCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	EditQueriesCmd(AppWnd& appWnd, AppDlg& appDlg, Queries& queries);

	//
	// IUiCommand methods.
	//

	//! Execute the command.
	virtual void execute();

private:
	//
	// Members.
	//
	AppWnd&		m_appWnd;	//! The app frame window.
	AppDlg&		m_appDlg;	//! The app main view.
	Queries&	m_queries;	//! The collection of WMI queries.
};

#endif // APP_EDITQUERIESCMD_HPP
