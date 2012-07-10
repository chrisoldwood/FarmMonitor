////////////////////////////////////////////////////////////////////////////////
//! \file   AppCmds.hpp
//! \brief  The AppCmds class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_APPCMDS_HPP
#define APP_APPCMDS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CmdCtrl.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The application command controller.

class AppCmds : public CCmdControl
{
public:
	//! Constructor.
	AppCmds();

	//! Destructor.
	virtual ~AppCmds();

	//
	// Commands.
	//

	//! Close the application.
	void onFileExit();

	//! Show the about dialog.
	void onHelpAbout();

	//
	// UI handlers.
	//
};

#endif // APP_APPCMDS_HPP
