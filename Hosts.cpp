////////////////////////////////////////////////////////////////////////////////
//! \file   Hosts.cpp
//! \brief  The Hosts class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Hosts.hpp"
#include <WCL/AppConfig.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Hosts::Hosts()
	: m_modified(false)
	, m_hostList()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

Hosts::~Hosts()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Load the list of hosts.

void Hosts::load(WCL::AppConfig& config)
{
	Hosts::Names hosts;

	size_t count = config.readValue<size_t>(TXT("Hosts"), TXT("Count"), 0);

	for (size_t i = 0; i != count; ++i)
	{
		tstring entry = Core::fmt(TXT("Host[%u]"), i);
		tstring host = config.readString(TXT("Hosts"), entry, TXT(""));

		if (!host.empty())
			hosts.push_back(host);
	}

	std::copy(hosts.begin(), hosts.end(), std::back_insert_iterator<Names>(m_hostList));
}

////////////////////////////////////////////////////////////////////////////////
//! Save the list of hosts.

void Hosts::save(WCL::AppConfig& config)
{
	config.writeValue<size_t>(TXT("Hosts"), TXT("Count"), m_hostList.size());

	for (size_t i = 0; i != m_hostList.size(); ++i)
	{
		tstring entry = Core::fmt(TXT("Host[%u]"), i);

		config.writeString(TXT("Hosts"), entry, m_hostList[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Add a new host. This returns the position it was inserted.

size_t Hosts::addHost(const tstring& hostname)
{
	m_hostList.push_back(hostname);
	m_modified = true;

	return m_hostList.size()-1;
}

////////////////////////////////////////////////////////////////////////////////
//! Remove a host by position.

void Hosts::removeHost(size_t index)
{
	ASSERT(index >= 0 && index < m_hostList.size());

	m_hostList.erase(m_hostList.begin() + index);
	m_modified = true;
}
