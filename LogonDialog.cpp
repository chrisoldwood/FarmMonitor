////////////////////////////////////////////////////////////////////////////////
//! \file   LogonDialog.cpp
//! \brief  The LogonDialog class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "LogonDialog.hpp"
#include "Resource.h"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

LogonDialog::LogonDialog()
	: CDialog(IDD_LOGON)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_USER,     &m_usernameEditor)
		CTRL(IDC_PASSWORD, &m_passwordEditor)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void LogonDialog::OnInitDialog()
{
	m_usernameEditor.Text(m_logon.m_user);
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool LogonDialog::OnOk()
{
	m_logon.m_password = m_passwordEditor.Text();

	return true;
}
