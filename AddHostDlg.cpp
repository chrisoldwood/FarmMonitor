////////////////////////////////////////////////////////////////////////////////
//! \file   AddHostDlg.cpp
//! \brief  The AddHostDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AddHostDlg.hpp"
#include "Resource.h"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

AddHostDlg::AddHostDlg()
	: CDialog(IDD_ADD_HOST)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_HOST_NAME, &m_hostnameEditor)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void AddHostDlg::OnInitDialog()
{
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool AddHostDlg::OnOk()
{
	if (m_hostnameEditor.TextLength() == 0)
	{
		AlertMsg(TXT("Please enter the name of the host."));
		return false;
	}

	m_hostname = m_hostnameEditor.Text();

	return true;
}
