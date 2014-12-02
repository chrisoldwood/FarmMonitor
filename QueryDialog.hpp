////////////////////////////////////////////////////////////////////////////////
//! \file   QueryDialog.hpp
//! \brief  The QueryDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef QUERYDIALOG_HPP
#define QUERYDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

////////////////////////////////////////////////////////////////////////////////
//!

class QueryDialog : public CDialog
{
public:
	//! Default constructor.
	QueryDialog();
	
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

#endif // QUERYDIALOG_HPP
