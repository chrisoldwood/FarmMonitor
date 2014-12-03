////////////////////////////////////////////////////////////////////////////////
//! \file   EditHostDialog.cpp
//! \brief  The EditHostDialog class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "EditHostDialog.hpp"
#include <NCL/Socket.hpp>
#include <WCL/BusyCursor.hpp>
#include <NCL/AutoWinSock.hpp>
#include <Core/Algorithm.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

EditHostDialog::EditHostDialog(Mode mode)
	: CDialog(IDD_ADDEDIT_HOST)
	, m_mode(mode)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_HOST_NAME,   &m_hostnameEditor)
		CTRL(IDC_MONITOR,     &m_monitorSwitch)
		CTRL(IDC_LOGON,	      &m_logonEditor)
		CTRL(IDC_ENVIRONMENT, &m_environmentEditor)
		CTRL(IDC_DESCRIPTION, &m_descriptionEditor)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_CHECK_HOST, BN_CLICKED, &EditHostDialog::onCheckHost)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void EditHostDialog::OnInitDialog()
{
	Title((m_mode == ADD_HOST) ? TXT("Add Host") : TXT("Edit Host"));

	m_hostnameEditor.Text(m_host.m_name);
	m_monitorSwitch.Check(m_host.m_monitor);
	m_logonEditor.Text(m_host.m_logon);
	m_environmentEditor.Text(m_host.m_environment);
	m_descriptionEditor.Text(m_host.m_description);
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool EditHostDialog::OnOk()
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
	m_host.m_monitor = m_monitorSwitch.IsChecked();
	m_host.m_logon = m_logonEditor.Text();;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Check host button handler.

void EditHostDialog::onCheckHost()
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
