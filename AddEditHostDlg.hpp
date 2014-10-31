////////////////////////////////////////////////////////////////////////////////
//! \file   AddEditHostDlg.hpp
//! \brief  The AddEditHostDlg class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef ADDEDITHOSTDLG_HPP
#define ADDEDITHOSTDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "Host.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The dialog to create and edit a hostname.

class AddEditHostDlg : public CDialog
{
public:
	// Dialog mode
	enum Mode
	{
		ADD_HOST,
		EDIT_HOST,
	};

	//! Constructor.
	AddEditHostDlg(Mode mode);
	
	//
	// Members.
	//

	//! The hostname to add/edit.
	Host m_host;

private:
	//
	// Members.
	//
	Mode		m_mode;				//!< Dialog configuration.

	//
	// Controls.
	//
	CEditBox	m_hostnameEditor;
	CEditBox	m_environmentEditor;
	CEditBox	m_descriptionEditor;

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();
};

#endif // ADDEDITHOSTDLG_HPP
