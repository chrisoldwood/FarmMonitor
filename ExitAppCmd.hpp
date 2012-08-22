////////////////////////////////////////////////////////////////////////////////
//! \file   ExitAppCmd.hpp
//! \brief  The ExitAppCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_EXITAPPCMD_HPP
#define APP_EXITAPPCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>
#include "AppWnd.hpp"
#include "Resource.h"

////////////////////////////////////////////////////////////////////////////////
//! The command to exit the application.

class ExitAppCmd : public WCL::UiCommandBase
{
public:
	//! Construction with the app window.
	ExitAppCmd(AppWnd& appWnd);

	//
	// IUiCommand methods.
	//

	//! Execute the command.
	virtual void execute();

private:
	//
	// Members.
	//
	AppWnd&	m_appWnd;	//! The app window.
};

////////////////////////////////////////////////////////////////////////////////
//! Construction with the app window.

inline ExitAppCmd::ExitAppCmd(AppWnd& appWnd)
	: WCL::UiCommandBase(ID_FILE_EXIT)
	, m_appWnd(appWnd)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

inline void ExitAppCmd::execute()
{
	m_appWnd.Close();
}

#endif // APP_EXITAPPCMD_HPP
