////////////////////////////////////////////////////////////////////////////////
//! \file   AddHostDlg.hpp
//! \brief  The AddHostDlg class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef ADDHOSTDLG_HPP
#define ADDHOSTDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

////////////////////////////////////////////////////////////////////////////////
//!

class AddHostDlg : public CDialog
{
public:
	//! Default constructor.
	AddHostDlg();
	
	//
	// Members.
	//

	//! The hostname to add.
	tstring m_hostname;

private:
	//
	// Controls.
	//
	CEditBox	m_hostnameEditor;

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();
};

#endif // ADDHOSTDLG_HPP
