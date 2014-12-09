////////////////////////////////////////////////////////////////////////////////
//! \file   ServicesDialog.hpp
//! \brief  The ServicesDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef SERVICESDIALOG_HPP
#define SERVICESDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

////////////////////////////////////////////////////////////////////////////////
//!

class ServicesDialog : public CDialog
{
public:
	//! Default constructor.
	ServicesDialog();
	
	//
	// Members.
	//

private:
	//
	// Members.
	//
	
	//
	// Controls.
	//

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();
};

#endif // SERVICESDIALOG_HPP
