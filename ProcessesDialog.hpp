////////////////////////////////////////////////////////////////////////////////
//! \file   ProcessesDialog.hpp
//! \brief  The ProcessesDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef PROCESSESDIALOG_HPP
#define PROCESSESDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

////////////////////////////////////////////////////////////////////////////////
//!

class ProcessesDialog : public CDialog
{
public:
	//! Default constructor.
	ProcessesDialog();
	
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

#endif // PROCESSESDIALOG_HPP
