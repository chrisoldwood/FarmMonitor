////////////////////////////////////////////////////////////////////////////////
//! \file   AddEditHostDlg.cpp
//! \brief  The AddEditHostDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AddEditHostDlg.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AddEditHostDlg::AddEditHostDlg(Mode mode)
	: CDialog(IDD_ADDEDIT_HOST)
	, m_mode(mode)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_HOST_NAME, &m_hostnameEditor)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void AddEditHostDlg::OnInitDialog()
{
	Title((m_mode == ADD_HOST) ? TXT("Add Host") : TXT("Edit Host"));

	m_hostnameEditor.Text(m_hostname);
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool AddEditHostDlg::OnOk()
{
	if (m_hostnameEditor.TextLength() == 0)
	{
		AlertMsg(TXT("Please enter the name of the host."));
		return false;
	}

	m_hostname = m_hostnameEditor.Text();

	return true;
}
