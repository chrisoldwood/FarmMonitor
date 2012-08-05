////////////////////////////////////////////////////////////////////////////////
//! \file   AppToolbar.cpp
//! \brief  The AppToolbar class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppToolbar.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppToolbar::AppToolbar(WCL::IMsgThread& thread, WCL::ICmdController& controller)
	: CToolBar(thread, controller)
{
	// Define the toolbar.
	DEFINE_TOOLBAR
		TBCTL(&m_help,	ID_HELP_ABOUT)
	END_TOOLBAR
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AppToolbar::~AppToolbar()
{
}
