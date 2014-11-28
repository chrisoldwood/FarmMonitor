////////////////////////////////////////////////////////////////////////////////
//! \file   Credentials.cpp
//! \brief  The Credentials class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Credentials.hpp"
#include <algorithm>
#include <Core/Algorithm.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Predicate for finding a logon by just the user name.

struct FindByUser
{
public:
	FindByUser(const tstring& value)
		: m_value(value)
	{ }

	bool operator()(const ConstLogonPtr& itValue)
	{
		return (itValue->m_user == m_value);
	}

private:
	const tstring& m_value;
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Credentials::Credentials()
	: m_logons()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

Credentials::~Credentials()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Set the logon for a user.

void Credentials::set(ConstLogonPtr logon)
{
	Core::erase_if(m_logons, FindByUser(logon->m_user));

	m_logons.push_back(logon);
}

////////////////////////////////////////////////////////////////////////////////
//! Find the logon for a user.

ConstLogonPtr Credentials::find(const tstring& user) const
{
	ConstLogonPtr logon;

	Collection::const_iterator it = std::find_if(m_logons.begin(), m_logons.end(), FindByUser(user));

	if (it != m_logons.end())
		logon = *it;

	return logon;
}

////////////////////////////////////////////////////////////////////////////////
//! Remove the logon for a user.

void Credentials::remove(const tstring& user)
{
	Core::erase_if(m_logons, FindByUser(user));
}
