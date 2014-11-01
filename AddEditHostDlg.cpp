////////////////////////////////////////////////////////////////////////////////
//! \file   AddEditHostDlg.cpp
//! \brief  The AddEditHostDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AddEditHostDlg.hpp"
#include <NCL/Socket.hpp>
#include <WCL/BusyCursor.hpp>
#include <NCL/AutoWinSock.hpp>
#include <Core/Algorithm.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AddEditHostDlg::AddEditHostDlg(Mode mode)
	: CDialog(IDD_ADDEDIT_HOST)
	, m_mode(mode)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_HOST_NAME,   &m_hostnameEditor)
		CTRL(IDC_ENVIRONMENT, &m_environmentEditor)
		CTRL(IDC_DESCRIPTION, &m_descriptionEditor)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_CHECK_HOST, BN_CLICKED, &AddEditHostDlg::onCheckHost)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void AddEditHostDlg::OnInitDialog()
{
	Title((m_mode == ADD_HOST) ? TXT("Add Host") : TXT("Edit Host"));

	m_hostnameEditor.Text(m_host.m_name);
	m_environmentEditor.Text(m_host.m_environment);
	m_descriptionEditor.Text(m_host.m_description);
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

	tstring hostname = m_hostnameEditor.Text();

	if (Core::exists(m_hosts, hostname))
	{
		AlertMsg(TXT("The host '%s' is already being monitored."), hostname.c_str());
		return false;
	}

	m_host.m_name = hostname;
	m_host.m_environment = m_environmentEditor.Text();
	m_host.m_description = m_descriptionEditor.Text();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Check host button handler.

void AddEditHostDlg::onCheckHost()
{
	if (m_hostnameEditor.TextLength() == 0)
	{
		AlertMsg(TXT("Please enter the name of the host first."));
		return;
	}

	CBusyCursor busyCursor;
	AutoWinSock autoWinSock;

	bool resolved = CSocket::canResolveHostname(m_hostnameEditor.Text());

	if (resolved)
		NotifyMsg(TXT("The hostname appears to be valid."));
	else
		AlertMsg(TXT("The hostname does not appear to be valid."));
}
