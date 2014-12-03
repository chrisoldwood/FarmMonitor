////////////////////////////////////////////////////////////////////////////////
//! \file   EditHostDialog.hpp
//! \brief  The EditHostDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef EDITHOSTDIALOG_HPP
#define EDITHOSTDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "Host.hpp"
#include <set>

////////////////////////////////////////////////////////////////////////////////
//! The dialog to create and edit a hostname.

class EditHostDialog : public CDialog
{
public:
	// Dialog mode
	enum Mode
	{
		ADD_HOST,
		EDIT_HOST,
	};

	//! Constructor.
	EditHostDialog(Mode mode);
	
	//
	// Members.
	//

	//! The hostname to add/edit.
	Host m_host;

	//! The unique set of existing hostnames.
	std::set<tstring> m_hosts;

private:
	//
	// Members.
	//
	Mode		m_mode;				//!< Dialog configuration.

	//
	// Controls.
	//
	CEditBox	m_hostnameEditor;
	CCheckBox	m_monitorSwitch;
	CEditBox	m_logonEditor;
	CEditBox	m_environmentEditor;
	CEditBox	m_descriptionEditor;

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();

	//! Check host button handler.
	void onCheckHost();
};

#endif // EDITHOSTDIALOG_HPP
