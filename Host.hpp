////////////////////////////////////////////////////////////////////////////////
//! \file   Host.hpp
//! \brief  The Host class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_HOST_HPP
#define APP_HOST_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/SharedPtr.hpp>

////////////////////////////////////////////////////////////////////////////////
//!	The definition of a host to monitor.

class Host
{
public:
	//! Default constructor.
	Host();

	//! Constructor.
	Host(const tstring& name);
	
	//! Constructor.
	Host(const tstring& name, const tstring& environment, const tstring& description,
			bool monitor, const tstring& logon);
	
	//
	// Members.
	//
	tstring		m_name;			//! The name of the host.
	tstring		m_environment;	//! The host's environment.
	tstring		m_description;	//! A description of the host.
	bool		m_monitor;		//! Flag to control monitoring.
	tstring		m_logon;		//! The alternate credentials to use.
};

//! The default Host smart pointer type.
typedef Core::SharedPtr<Host> HostPtr;

//! The default Host const smart pointer type.
typedef Core::SharedPtr<const Host> ConstHostPtr;

////////////////////////////////////////////////////////////////////////////////
//! Create a new host.

inline ConstHostPtr makeHost(const tstring& name)
{
	return ConstHostPtr(new Host(name));
}

////////////////////////////////////////////////////////////////////////////////
//! Create a new host.

inline ConstHostPtr makeHost(const tstring& name, const tstring& environment, const tstring& description,
								bool monitor, const tstring& logon)
{
	return ConstHostPtr(new Host(name, environment, description, monitor, logon));
}

////////////////////////////////////////////////////////////////////////////////
//! Create a host by copying another.

inline ConstHostPtr makeHost(const Host& rhs)
{
	return ConstHostPtr(new Host(rhs));
}

#endif // APP_HOST_HPP
