////////////////////////////////////////////////////////////////////////////////
//! \file   Formats.cpp
//! \brief  The Formats class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Formats.hpp"
#include <Core/Functor.hpp>
#include <Core/Algorithm.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constants.

const tstring Formats::STRING = TXT("%s");
const tstring Formats::DATETIME = TXT("%t");
const tstring Formats::BYTES = TXT("B");
const tstring Formats::K_BYTES = TXT("KB");
const tstring Formats::M_BYTES = TXT("MB");
const tstring Formats::G_BYTES = TXT("GB");
const tstring Formats::KB_AS_MB = TXT("KB2MB");
const tstring Formats::KB_AS_GB = TXT("KB2GB");

const tstring Formats::DEFAULT = STRING;

////////////////////////////////////////////////////////////////////////////////
//! Global variables.

//! The default collection of formats.
const Formats Formats::g_default;

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Formats::Formats()
	: m_collection()
{
	m_collection.push_back(FormatMapping( TXT("%s"),    TXT("String")         ));
	m_collection.push_back(FormatMapping( TXT("%t"),    TXT("Date & Time")    ));
	m_collection.push_back(FormatMapping( TXT("B"),     TXT("Bytes (B)")      ));
	m_collection.push_back(FormatMapping( TXT("KB"),    TXT("Kilobytes (KB)") ));
	m_collection.push_back(FormatMapping( TXT("MB"),    TXT("Megabytes (MB)") ));
	m_collection.push_back(FormatMapping( TXT("GB"),    TXT("Gigabytes (GB)") ));
	m_collection.push_back(FormatMapping( TXT("KB2MB"), TXT("KB as MB")       ));
	m_collection.push_back(FormatMapping( TXT("KB2GB"), TXT("KB as GB")       ));
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

Formats::~Formats()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Match a format on its value.

CORE_DEFINE_PREDICATE_EX(FormatEquals, tstring, tstring, format, const Formats::FormatMapping&, rhs)
{
	return rhs.first == format;
}
CORE_END_PREDICATE_EX

////////////////////////////////////////////////////////////////////////////////
//! Get the display name for a format.

tstring Formats::getDisplayName(const tstring& format) const
{
	ConstIter it = Core::find_if(m_collection, FormatEquals(format));

	ASSERT(it != m_collection.end());

	return it->second;
}

////////////////////////////////////////////////////////////////////////////////
//! Match a format on its display name.

CORE_DEFINE_PREDICATE_EX(DisplayNameEquals, tstring, tstring, displayName, const Formats::FormatMapping&, rhs)
{
	return rhs.second == displayName;
}
CORE_END_PREDICATE_EX

////////////////////////////////////////////////////////////////////////////////
//! Get the format from the display name.

tstring Formats::getFormat(const tstring& displayName) const
{
	ConstIter it = Core::find_if(m_collection, DisplayNameEquals(displayName));

	ASSERT(it != m_collection.end());

	return it->first;
}
