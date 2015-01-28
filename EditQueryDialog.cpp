////////////////////////////////////////////////////////////////////////////////
//! \file   EditQueryDialog.cpp
//! \brief  The EditQueryDialog class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "EditQueryDialog.hpp"
#include "Resource.h"
#include <Core/Algorithm.hpp>
#include "Formats.hpp"

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

	populateFormats();

	m_wmiClassEditor.Add(TXT("Win32_OperatingSystem"));
	m_wmiClassEditor.Add(TXT("Win32_LogicalMemoryConfiguration"));
	m_wmiClassEditor.Add(TXT("Win32_LogicalDisk"));

	displayQuery(m_query);
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool EditQueryDialog::OnOk()
{
	m_query.m_title = m_titleEditor.Text();
	m_query.m_wmiClass = m_wmiClassEditor.Text();
	m_query.m_wmiProperty = m_wmiPropertyEditor.Text();
	m_query.m_format = Formats::g_default.getFormat(m_formatChoice.Text().c_str());
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

	displayQuery(query.getRef());
}

////////////////////////////////////////////////////////////////////////////////
//! Build the collection of template queries.

void EditQueryDialog::buildTemplates()
{
	ASSERT(m_queryTemplates.empty());

	m_queryTemplates.push_back(QueryTemplate(
		TXT("Total Memory"), makeQuery(TXT("Total Mem"), TXT("Win32_LogicalMemoryConfiguration"), TXT("TotalPhysicalMemory"), TXT(""), TXT(""), Formats::KB_AS_MB)
	));

	m_queryTemplates.push_back(QueryTemplate(
		TXT("Free Memory"), makeQuery(TXT("Free Mem"), TXT("Win32_LogicalMemoryConfiguration"), TXT("AvailableVirtualMemory"), TXT(""), TXT(""), Formats::KB_AS_MB)
	));

	m_queryTemplates.push_back(QueryTemplate(
		TXT("Drive C: Total Size"), makeQuery(TXT("C: Size"), TXT("Win32_LogicalDisk"), TXT("Size"), TXT("DeviceID"), TXT("C:"), Formats::G_BYTES)
	));

	m_queryTemplates.push_back(QueryTemplate(
		TXT("Drive C: Free Space"), makeQuery(TXT("C: Free"), TXT("Win32_LogicalDisk"), TXT("FreeSpace"), TXT("DeviceID"), TXT("C:"), Formats::G_BYTES)
	));

	m_queryTemplates.push_back(QueryTemplate(
		TXT("Time Rebooted"), makeQuery(TXT("Rebooted"), TXT("Win32_OperatingSystem"), TXT("LastBootUpTime"), TXT(""), TXT(""), Formats::DATETIME)
	));
}

////////////////////////////////////////////////////////////////////////////////
//! Populate the set of WMI query templates.

void EditQueryDialog::populateTemplates()
{
	typedef QueryTemplates::const_iterator ConstIter;

	for (ConstIter it = m_queryTemplates.begin(); it != m_queryTemplates.end(); ++it)
		m_templateChoice.Add(it->first);
}

////////////////////////////////////////////////////////////////////////////////
//! Populate the set of value formats.

void EditQueryDialog::populateFormats()
{
	for (Formats::ConstIter it = Formats::g_default.begin(); it != Formats::g_default.end(); ++it)
		m_formatChoice.Add(it->second);
}

////////////////////////////////////////////////////////////////////////////////
//! Display the query details.

void EditQueryDialog::displayQuery(const Query& query)
{
	if (m_wmiClassEditor.FindExact(query.m_wmiClass) == Core::npos)
		m_wmiClassEditor.Add(query.m_wmiClass);

	m_titleEditor.Text(query.m_title);
	m_wmiClassEditor.Select(query.m_wmiClass);
	m_wmiPropertyEditor.Text(query.m_wmiProperty);
	m_formatChoice.Select(Formats::g_default.getDisplayName(query.m_format));
	m_filterPropertyEditor.Text(query.m_filterProperty);
	m_filterValueEditor.Text(query.m_filterValue);
}
