////////////////////////////////////////////////////////////////////////////////
//! \file   DataDocument.cpp
//! \brief  Helper functions for manipulating the XML based data file.
//! \author Chris Oldwood

#include "Common.hpp"
#include "DataDocument.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Create the shell of the data document.

XML::DocumentPtr createDocumentShell()
{
	XML::NodePtr sections[] =
	{
		XML::makeElement(TXT("Hosts")),
		XML::makeElement(TXT("Tools")),
		XML::makeElement(TXT("Queries")),
	};

	XML::ElementNodePtr root = XML::makeElement
	(
		TXT("FarmMonitor"), sections, sections+ARRAY_SIZE(sections)
	);

	root->setAttribute(TXT("Version"), TXT("1"));

	return XML::makeDocument(root);
}
