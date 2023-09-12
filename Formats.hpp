////////////////////////////////////////////////////////////////////////////////
//! \file   Formats.hpp
//! \brief  The Formats class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_FORMATS_HPP
#define APP_FORMATS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
//! The set of standard formatting types.

class Formats
{
public:
	//! A format mapping.
	typedef std::pair<tstring, tstring> FormatMapping;
	//! A collection of format mappings.
	typedef std::vector<FormatMapping> Collection;
	//! A const iterator for the format mappings.
	typedef Collection::const_iterator ConstIter;
	
public:
	//! Default constructor.
	Formats();

	//! Destructor.
	~Formats();

	//
	// Properties.
	//

	//! The size of the collection.
	size_t size() const;

	//! The iterator to the start of the collection.
	ConstIter begin() const;

	//! The iterator for the end of the collection.
	ConstIter end() const;

	//
	// Methods.
	//

	//! Get the display name for a format.
	tstring getDisplayName(const tstring& format) const;
	
	//! Get the format from the display name.
	tstring getFormat(const tstring& displayName) const;
	
	//
	// Constants.
	//

	//! Format any value as a verbatim string.
	static const tstring STRING;
	//! Format the WMI datetime as localised date & time.
	static const tstring DATETIME;
	//! Format the byte count in bytes.
	static const tstring Formats::BYTES;
	//! Format the byte count in kilobytes.
	static const tstring Formats::K_BYTES;
	//!Format the byte count in megabytes.
	static const tstring Formats::M_BYTES;
	//! Format the byte count in gigabytes.
	static const tstring Formats::G_BYTES;
	//! Format the kilobyte count as megabytes.
	static const tstring Formats::KB_AS_MB;
	//! Format the kilobyte count as gigabytes.
	static const tstring Formats::KB_AS_GB;
	//! Format the megabyte count as gigabytes.
	static const tstring Formats::MB_AS_GB;

	//! The default format.
	static const tstring DEFAULT;

	//
	// Global variables.
	//

	//! The default collection of formats.
	static const Formats g_default;

private:
	//
	// Members.
	//
	Collection	m_collection;		//!< The collection of formats.
};

////////////////////////////////////////////////////////////////////////////////
//! The size of the collection.

inline size_t Formats::size() const
{
	return m_collection.size();
}

////////////////////////////////////////////////////////////////////////////////
//! The iterator to the start of the collection.

inline Formats::ConstIter Formats::begin() const
{
	return m_collection.begin();
}

////////////////////////////////////////////////////////////////////////////////
//! The iterator for the end of the collection.

inline Formats::ConstIter Formats::end() const
{
	return m_collection.end();
}

#endif // APP_FORMATS_HPP
