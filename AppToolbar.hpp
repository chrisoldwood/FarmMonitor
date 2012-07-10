////////////////////////////////////////////////////////////////////////////////
//! \file   AppToolbar.hpp
//! \brief  The AppToolbar class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_APPTOOLBAR_HPP
#define APP_APPTOOLBAR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/ToolBar.hpp>
#include <WCL/CmdBtn.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The application toolbar.

class AppToolbar : public CToolBar
{
public:
	//! Constructor.
	AppToolbar();

	//! Destructor.
	virtual ~AppToolbar();
	
	//
	// Members.
	//
	CCmdButton	m_help;		//!< The Help button.
};

#endif // APP_APPTOOLBAR_HPP
