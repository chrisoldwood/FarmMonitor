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

// Forward declarations.
namespace WCL
{
class IAppConfigReader;
class IAppConfigWriter;
}

////////////////////////////////////////////////////////////////////////////////
//! The container of hosts being monitored.

class Hosts : private Core::NotCopyable
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

	//! Load the set of hosts.
	void load(WCL::IAppConfigReader& config);

	//! Save the set of hosts.
	void save(WCL::IAppConfigWriter& config);

	//! Add a new host.
	size_t add(const tstring& hostname);

	//! Remove a host by position.
	void remove(size_t index);

private:
	//
	// Members.
	//
	bool		m_modified;		//!< Has the collection been modified?
	Names		m_hosts;		//!< The collection of hosts to monitor.
};

#endif // APP_HOSTS_HPP