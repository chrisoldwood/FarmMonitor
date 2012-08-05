////////////////////////////////////////////////////////////////////////////////
//! \file   Hosts.hpp
//! \brief  The Hosts class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_HOSTS_HPP
#define APP_HOSTS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/SharedPtr.hpp>

namespace WCL
{
class AppConfig;
}

////////////////////////////////////////////////////////////////////////////////
//! The container of hosts being monitored.

class Hosts
{
public:
	//! The type used to store the collection of host names.
	typedef std::vector<tstring> Names;

public:
	//! Default constructor.
	Hosts();

	//! Destructor.
	~Hosts();

	//
	// Properties.
	//

	//! Get the number of hosts.
	size_t size() const;
	
	//! Get the name of the host at a given position.
	const tstring& name(size_t index) const;
	
	//! Has the collection of names been modified?
	bool isModified() const;
	
	//
	// Methods.
	//

	//! Load the list of hosts.
	void load(WCL::AppConfig& config);

	//! Save the list of hosts.
	void save(WCL::AppConfig& config);

	//! Add a new host.
	size_t addHost(const tstring& hostname);

	//! Remove a host by position.
	void removeHost(size_t index);

private:
	//
	// Members.
	//
	bool		m_modified;		//!< Has the list of names been modified?
	Names		m_hostList;		//!< The collection of hosts to monitor.
};

//! The Hosts default smart pointer type.
typedef Core::SharedPtr<Hosts> HostsPtr;

////////////////////////////////////////////////////////////////////////////////
//! Get the number of hosts.

inline size_t Hosts::size() const
{
	return m_hostList.size();
}

////////////////////////////////////////////////////////////////////////////////
//! Get the name of the host at a given position.

inline const tstring& Hosts::name(size_t index) const
{
	ASSERT(index >= 0 && index < m_hostList.size());

	return m_hostList[index];
}
	
////////////////////////////////////////////////////////////////////////////////
//! Has the collection of names been modified?

inline bool Hosts::isModified() const
{
	return m_modified;
}

#endif // APP_HOSTS_HPP
