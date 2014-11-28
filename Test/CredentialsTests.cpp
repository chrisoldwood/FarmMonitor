////////////////////////////////////////////////////////////////////////////////
//! \file   CredentialsTests.cpp
//! \brief  The unit tests for the Credentials class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "Credentials.hpp"

TEST_SET(Credentials)
{

TEST_CASE("the credentials cache starts empty")
{
	Credentials credentials;

	TEST_TRUE(credentials.find(TXT("anyone")).empty());
}
TEST_CASE_END

TEST_CASE("credentials can be added to the cache")
{
	const tstring user = TXT("username");
	const tstring password = TXT("password");

	Credentials credentials;

	credentials.set(makeLogon(user, password));

	ConstLogonPtr logon = credentials.find(user);

	TEST_TRUE(!logon.empty());
	TEST_TRUE(logon->m_user == user);
	TEST_TRUE(logon->m_password == password);
}
TEST_CASE_END

TEST_CASE("adding credentials for an existing logon replaces the previous one")
{
	const tstring user = TXT("username");
	const tstring oldPassword = TXT("old password");
	const tstring newPassword = TXT("new password");

	Credentials credentials;

	credentials.set(makeLogon(user, oldPassword));

	credentials.set(makeLogon(user, newPassword));

	ConstLogonPtr logon = credentials.find(user);

	TEST_TRUE(!logon.empty());
	TEST_TRUE(logon->m_user == user);
	TEST_TRUE(logon->m_password == newPassword);
}
TEST_CASE_END

TEST_CASE("credentials can be removed from the cache by username")
{
	const tstring user = TXT("username");
	const tstring password = TXT("password");

	Credentials credentials;

	credentials.set(makeLogon(user, password));

	ConstLogonPtr logon = credentials.find(user);

	credentials.remove(logon->m_user);

	logon = credentials.find(user);

	TEST_TRUE(logon.empty());
}
TEST_CASE_END

}
TEST_SET_END
