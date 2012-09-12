////////////////////////////////////////////////////////////////////////////////
//! \file   ExecuteToolCmd.hpp
//! \brief  The ExecuteToolCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_EXECUTETOOLCMD_HPP
#define APP_EXECUTETOOLCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>

// Forward declarations.
class Tools;
class AppDlg;

////////////////////////////////////////////////////////////////////////////////
//! The command used to execute an external tool for the selected host.

class ExecuteToolCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	ExecuteToolCmd(Tools& tools, uint index, AppDlg& appDlg);

	//
	// IUiCommand methods.
	//

	//! Execute the command.
	virtual void execute();

private:
	//
	// Members.
	//
	Tools&	m_tools;	//!< The set of external tools.
	size_t	m_index;	//!< The index of the tool to invoke.
	AppDlg& m_appDlg;	//!< The main view.

	//! Convert a ShellExecute error code to a string.
	const tchar* toString(uint code);
};

#endif // APP_EXECUTETOOLCMD_HPP
