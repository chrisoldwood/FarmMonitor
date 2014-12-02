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

#include <WCL/ExternalCmdController.hpp>

// Forward declarations.
class AppWnd;
class AppDlg;
class Model;

////////////////////////////////////////////////////////////////////////////////
//! The application command controller.

class AppCmds : public WCL::ExternalCmdController
{
public:
	//! Constructor.
	AppCmds(AppWnd& appWnd, AppDlg& appDlg, Model& model);

	//! Destructor.
	virtual ~AppCmds();
};

#endif // APP_APPCMDS_HPP
