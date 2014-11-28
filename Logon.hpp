////////////////////////////////////////////////////////////////////////////////
//! \file   Logon.hpp
//! \brief  The Logon class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_LOGON_HPP
#define APP_LOGON_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/SharedPtr.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The custom credentials to use for querying a host.

class Logon
{
public:
	//! Default constructor.
	Logon();

	//! Constructor.
	Logon(const tstring& user, const tstring& password);

	//
	// Members.
	//
	tstring	m_user;			//!< The user name.
	tstring	m_password;		//!< The password.
};

//! The default Logon const smart pointer type.
typedef Core::SharedPtr<const Logon> ConstLogonPtr;

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline Logon::Logon()
	: m_user()
	, m_password()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

inline Logon::Logon(const tstring& user, const tstring& password)
	: m_user(user)
	, m_password(password)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Create a new logon.

inline ConstLogonPtr makeLogon(const tstring& user, const tstring& password)
{
	return ConstLogonPtr(new Logon(user, password));
}

////////////////////////////////////////////////////////////////////////////////
//! Create a new logon.

inline ConstLogonPtr makeLogon(const Logon& logon)
{
	return ConstLogonPtr(new Logon(logon.m_user, logon.m_password));
}

#endif // APP_LOGON_HPP
