////////////////////////////////////////////////////////////////////////////////
//! \file   EditQueryDialog.hpp
//! \brief  The EditQueryDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef EDITQUERYDIALOG_HPP
#define EDITQUERYDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

////////////////////////////////////////////////////////////////////////////////
//!

class EditQueryDialog : public CDialog
{
public:
	//! Default constructor.
	EditQueryDialog();
	
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

#endif // EDITQUERYDIALOG_HPP
