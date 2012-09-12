////////////////////////////////////////////////////////////////////////////////
//! \file   Hosts.cpp
//! \brief  The Hosts class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Hosts.hpp"
#include <WCL/IAppConfigReader.hpp>
#include <WCL/IAppConfigWriter.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Hosts::Hosts()
	: m_modified(false)
	, m_hosts()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

Hosts::~Hosts()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the number of hosts.

size_t Hosts::size() const
{
	return m_hosts.size();
}

////////////////////////////////////////////////////////////////////////////////
//! Get the name of the host at a given position.

const tstring& Hosts::name(size_t index) const
{
	ASSERT(index < m_hosts.size());

	return m_hosts[index];
}
	
////////////////////////////////////////////////////////////////////////////////
//! Has the collection of names been modified?

bool Hosts::isModified() const
{
	return m_modified;
}

////////////////////////////////////////////////////////////////////////////////
//! Load the set of hosts.

void Hosts::load(WCL::IAppConfigReader& config)
{
	Hosts::Names hosts;

	const size_t count = config.readValue<size_t>(TXT("Hosts"), TXT("Count"), 0);

	for (size_t i = 0; i != count; ++i)
	{
		const tstring host = config.readString(TXT("Hosts"), Core::fmt(TXT("Host[%u]"), i), TXT(""));

		if (!host.empty())
			hosts.push_back(host);
	}

	std::swap(m_hosts, hosts);
	m_modified = false;
}

////////////////////////////////////////////////////////////////////////////////
//! Save the set of hosts.

void Hosts::save(WCL::IAppConfigWriter& config)
{
	if (!m_modified)
		return;

	config.deleteSection(TXT("Hosts"));
	config.writeValue<size_t>(TXT("Hosts"), TXT("Count"), m_hosts.size());

	for (size_t i = 0; i != m_hosts.size(); ++i)
	{
		config.writeString(TXT("Hosts"), Core::fmt(TXT("Host[%u]"), i), m_hosts[i]);
	}

	m_modified = false;
}

////////////////////////////////////////////////////////////////////////////////
//! Add a new host. This returns the position it was inserted.

size_t Hosts::add(const tstring& hostname)
{
	m_hosts.push_back(hostname);
	m_modified = true;

	return m_hosts.size()-1;
}

////////////////////////////////////////////////////////////////////////////////
//! Remove a host by position.

void Hosts::remove(size_t index)
{
	ASSERT(index < m_hosts.size());

	m_hosts.erase(m_hosts.begin() + index);
	m_modified = true;
}
