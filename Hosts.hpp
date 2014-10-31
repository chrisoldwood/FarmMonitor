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

#include "Host.hpp"
#include <Core/SharedPtr.hpp>
#include <XML/Document.hpp>

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
private:
	//! The type used to store the collection of hosts.
	typedef std::vector<ConstHostPtr> Collection;

public:
	//! The const iterator type.
	typedef Collection::const_iterator const_iterator;

	//! Default constructor.
	Hosts();

	//! Destructor.
	~Hosts();

	//
	// Properties.
	//

	//! Get the number of hosts.
	size_t size() const;
	
	//! Get the host at a given position.
	const ConstHostPtr& host(size_t index) const;
	
	//! Has the collection of names been modified?
	bool isModified() const;
	
	//
	// Methods.
	//

	//! Load the set of hosts from the application config.
	void load(WCL::IAppConfigReader& config);

	//! Save the set of hosts to the application config.
	void save(WCL::IAppConfigWriter& config);

	//! Load the set of hosts from the XML document.
	void load(const XML::DocumentPtr config);

	//! Save the set of hosts to the XML document.
	void save(XML::DocumentPtr config);

	//! Add a new host.
	size_t add(ConstHostPtr host);

	//! Replace a host by position.
	void replace(size_t index, ConstHostPtr host);

	//! Remove a host by position.
	void remove(size_t index);

private:
	//
	// Members.
	//
	bool		m_modified;		//!< Has the collection been modified?
	Collection	m_hosts;		//!< The collection of hosts to monitor.
};

#endif // APP_HOSTS_HPP
