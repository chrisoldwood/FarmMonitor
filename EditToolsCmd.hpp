////////////////////////////////////////////////////////////////////////////////
//! \file   EditToolsCmd.hpp
//! \brief  The EditToolsCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_EDITTOOLSCMD_HPP
#define APP_EDITTOOLSCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/UiCommandBase.hpp>

// Forward declarations.
class AppWnd;
class Tools;

////////////////////////////////////////////////////////////////////////////////
//! The command to show the dialog for editing the tools.

class EditToolsCmd : public WCL::UiCommandBase
{
public:
	//! Constructor.
	EditToolsCmd(AppWnd& appWnd, Tools& tools);

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
	Tools&		m_tools;	//! The collection of tools.
};

#endif // APP_EDITTOOLSCMD_HPP
