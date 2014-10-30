////////////////////////////////////////////////////////////////////////////////
//! \file   DataDocument.hpp
//! \brief  Helper functions for manipulating the XML based data file.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_DATADOCUMENT_HPP
#define APP_DATADOCUMENT_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <XML/Document.hpp>

//! Create the shell of the data document.
XML::DocumentPtr createDocumentShell();

#endif // APP_DATADOCUMENT_HPP
