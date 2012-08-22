////////////////////////////////////////////////////////////////////////////////
//! \file   ShowManualCmd.hpp
//! \brief  The ShowManualCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_SHOWMANUALCMD_HPP
#define APP_SHOWMANUALCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>
#include <WCL/BusyCursor.hpp>
#include <Core/FileSystem.hpp>
#include <WCL/Path.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The command to show the manual.

class ShowManualCmd : public WCL::UiCommandBase
{
public:
	//! Construction with the app window.
	ShowManualCmd(AppWnd& appWnd);

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
//! Default constructor.

inline ShowManualCmd::ShowManualCmd(AppWnd& appWnd)
	: WCL::UiCommandBase(ID_HELP_CONTENTS)
	, m_appWnd(appWnd)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

inline void ShowManualCmd::execute()
{
	CBusyCursor busyCursor;

	tstring manual = CPath::ApplicationDir() / TXT("FarmMonitor.html");

	if (!Core::pathExists(manual))
	{
		m_appWnd.FatalMsg(TXT("The manual is not installed:-\n\n%s"), manual.c_str());
		return;
	}

	::ShellExecute(NULL, NULL, manual.c_str(), NULL, NULL, SW_SHOW);
}

#endif // APP_SHOWMANUALCMD_HPP
