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

////////////////////////////////////////////////////////////////////////////////
//!	The definition of a host to monitor.

class Host
{
public:
	//! Default constructor.
	Host();

	//! Constructor.
	Host(const tstring& name);
	
	//
	// Members.
	//
	tstring		m_name;			//! The name of the host.
};

//! The default Host smart pointer type.
typedef Core::SharedPtr<Host> HostPtr;

//! The default Host const smart pointer type.
typedef Core::SharedPtr<const Host> ConstHostPtr;

#endif // APP_HOST_HPP
