////////////////////////////////////////////////////////////////////////////////
//! \file   EditQueryDialog.cpp
//! \brief  The EditQueryDialog class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "EditQueryDialog.hpp"
#include "Resource.h"
#include <Core/Algorithm.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

EditQueryDialog::EditQueryDialog()
	: CDialog(IDD_QUERY)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_TEMPLATES, &m_templateChoice)
		CTRL(IDC_TITLE, &m_titleEditor)
		CTRL(IDC_WMI_CLASS, &m_wmiClassEditor)
		CTRL(IDC_WMI_PROPERTY, &m_wmiPropertyEditor)
		CTRL(IDC_FORMAT, &m_formatChoice)
		CTRL(IDC_FILTER, &m_filterPropertyEditor)
		CTRL(IDC_VALUE, &m_filterValueEditor)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_APPLY, BN_CLICKED, &EditQueryDialog::onApplyTemplate)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void EditQueryDialog::OnInitDialog()
{
	buildTemplates();
	populateTemplates();

	m_wmiClassEditor.Add(TXT("Win32_OperatingSystem"));
	m_wmiClassEditor.Add(TXT("Win32_LogicalMemoryConfiguration"));
	m_wmiClassEditor.Add(TXT("Win32_LogicalDisk"));

	if (m_wmiClassEditor.FindExact(m_query.m_wmiClass) == Core::npos)
		m_wmiClassEditor.Add(m_query.m_wmiClass);

	m_formatChoice.Add(TXT("%s"));
	m_formatChoice.Add(TXT("%t"));
	m_formatChoice.Add(TXT("B"));
	m_formatChoice.Add(TXT("KB"));
	m_formatChoice.Add(TXT("MB"));
	m_formatChoice.Add(TXT("GB"));
	m_formatChoice.Add(TXT("KB2MB"));
	m_formatChoice.Add(TXT("KB2GB"));

	m_titleEditor.Text(m_query.m_title);
	m_wmiClassEditor.Select(m_query.m_wmiClass);
	m_wmiPropertyEditor.Text(m_query.m_wmiProperty);
	m_formatChoice.Select(m_query.m_format);
	m_filterPropertyEditor.Text(m_query.m_filterProperty);
	m_filterValueEditor.Text(m_query.m_filterValue);
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool EditQueryDialog::OnOk()
{
	m_query.m_title = m_titleEditor.Text();
	m_query.m_wmiClass = m_wmiClassEditor.Text();
	m_query.m_wmiProperty = m_wmiPropertyEditor.Text();
	m_query.m_format = m_formatChoice.Text();
	m_query.m_filterProperty = m_filterPropertyEditor.Text();
	m_query.m_filterValue = m_filterValueEditor.Text();

	if (m_query.m_title.empty())
	{
		AlertMsg(TXT("You must provide a column heading for the query."));
		m_titleEditor.Focus();
		return false;
	}
	
	if (Core::exists(m_usedTitles, m_query.m_title))
	{
		AlertMsg(TXT("There is already a query with that column heading defined."));
		m_titleEditor.Focus();
		return false;
	}
	
	if (m_query.m_wmiClass.empty())
	{
		AlertMsg(TXT("You must supply the name for the WMI class to query."));
		m_wmiClassEditor.Focus();
		return false;
	}

	if (m_query.m_wmiProperty.empty())
	{
		AlertMsg(TXT("You must supply the name for the WMI property to display."));
		m_wmiPropertyEditor.Focus();
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Apply button handler.

void EditQueryDialog::onApplyTemplate()
{
	size_t selected = m_templateChoice.CurSel();

	if (selected == Core::npos)
		return;

	ConstQueryPtr query = m_queryTemplates[selected].second;

	m_titleEditor.Text(query->m_title);
	m_wmiClassEditor.Select(query->m_wmiClass);
	m_wmiPropertyEditor.Text(query->m_wmiProperty);
	m_formatChoice.Select(query->m_format);
	m_filterPropertyEditor.Text(query->m_filterProperty);
	m_filterValueEditor.Text(query->m_filterValue);
}

////////////////////////////////////////////////////////////////////////////////
//! Build the collection of template queries.

void EditQueryDialog::buildTemplates()
{
	if (m_queryTemplates.empty())
	{
		m_queryTemplates.push_back(std::make_pair(
			TXT("Total Memory"), makeQuery(TXT("Total Mem"), TXT("Win32_LogicalMemoryConfiguration"), TXT("TotalPhysicalMemory"), TXT(""), TXT(""), TXT("KB2MB")) ));

		m_queryTemplates.push_back(std::make_pair(
			TXT("Free Memory"), makeQuery(TXT("Free Mem"), TXT("Win32_LogicalMemoryConfiguration"), TXT("AvailableVirtualMemory"), TXT(""), TXT(""), TXT("KB2MB")) ));

		m_queryTemplates.push_back(std::make_pair(
			TXT("Drive C: Total Size"), makeQuery(TXT("C: Size"), TXT("Win32_LogicalDisk"), TXT("Size"), TXT("DeviceID"), TXT("C:"), TXT("GB")) ));

		m_queryTemplates.push_back(std::make_pair(
			TXT("Drive C: Free Space"), makeQuery(TXT("C: Free"), TXT("Win32_LogicalDisk"), TXT("FreeSpace"), TXT("DeviceID"), TXT("C:"), TXT("GB")) ));

		m_queryTemplates.push_back(std::make_pair(
			TXT("Time Rebooted"), makeQuery(TXT("Rebooted"), TXT("Win32_OperatingSystem"), TXT("LastBootUpTime"), TXT(""), TXT(""), Query::DATETIME_FORMAT) ));
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Populate the set of WMI query templates.

void EditQueryDialog::populateTemplates()
{
	typedef QueryTemplates::const_iterator ConstIter;

	for (ConstIter it = m_queryTemplates.begin(); it != m_queryTemplates.end(); ++it)
		m_templateChoice.Add(it->first);
}
