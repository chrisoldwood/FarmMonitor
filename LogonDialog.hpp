////////////////////////////////////////////////////////////////////////////////
//! \file   LogonDialog.hpp
//! \brief  The LogonDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef LOGONDIALOG_HPP
#define LOGONDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "Logon.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to enter custom credentials.

class LogonDialog : public CDialog
{
public:
	//! Default constructor.
	LogonDialog();
	
	//
	// Members.
	//
	Logon	m_logon;

private:
	//
	// Members.
	//
	
	//
	// Controls.
	//
	CEditBox	m_usernameEditor;
	CEditBox	m_passwordEditor;

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();
};

#endif // LOGONDIALOG_HPP
