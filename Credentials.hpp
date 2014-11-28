////////////////////////////////////////////////////////////////////////////////
//! \file   Credentials.hpp
//! \brief  The Credentials class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_CREDENTIALS_HPP
#define APP_CREDENTIALS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Logon.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The collection of custom logon credentials.

class Credentials
{
private:
	//! The type used to store the collection of logons.
	typedef std::vector<ConstLogonPtr> Collection;

public:
	//! Default constructor.
	Credentials();

	//! Destructor.
	~Credentials();

	//
	// Methods.
	//

	//! Set the logon for a user.
	void set(ConstLogonPtr logon);

	//! Find the logon for a user.
	ConstLogonPtr find(const tstring& user) const;

	//! Remove the logon for a user.
	void remove(const tstring& user);

private:
	//
	// Members.
	//
	Collection	m_logons;		//!< The collection of logons.
};

#endif // APP_CREDENTIALS_HPP
