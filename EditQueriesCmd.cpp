////////////////////////////////////////////////////////////////////////////////
//! \file   EditQueriesCmd.cpp
//! \brief  The EditQueriesCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "EditQueriesCmd.hpp"
#include "QueriesDialog.hpp"
#include "AppWnd.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

EditQueriesCmd::EditQueriesCmd(AppWnd& appWnd, AppDlg& appDlg, Queries& queries)
	: WCL::UiCommandBase(ID_VIEW_QUERIES)
	, m_appWnd(appWnd)
	, m_appDlg(appDlg)
	, m_queries(queries)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

void EditQueriesCmd::execute()
{
	QueriesDialog dialog;

	dialog.m_queries.deepCopy(m_queries);

	if (dialog.RunModal(m_appWnd) == IDOK)
	{
		if (dialog.m_queries.isModified())
		{
			m_queries.replaceAll(dialog.m_queries);

			m_appDlg.rebuildView();
		}
	}
}
