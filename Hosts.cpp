////////////////////////////////////////////////////////////////////////////////
//! \file   Hosts.cpp
//! \brief  The Hosts class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Hosts.hpp"
#include <XML/XPathIterator.hpp>
#include <XML/TextNode.hpp>
#include <Core/ConfigurationException.hpp>
#include <set>
#include <Core/Algorithm.hpp>
#include <Core/StringUtils.hpp>

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
//! Get the host at a given position.

const ConstHostPtr& Hosts::host(size_t index) const
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
//! Get the collection start iterator.

Hosts::const_iterator Hosts::begin() const
{
	return m_hosts.begin();
}

////////////////////////////////////////////////////////////////////////////////
//! Get the collection end iterator.

Hosts::const_iterator Hosts::end() const
{
	return m_hosts.end();
}

////////////////////////////////////////////////////////////////////////////////
//! Load the set of hosts from the XML document.

void Hosts::load(const XML::DocumentPtr config)
{
	ASSERT(config->hasRootElement());

	Collection hosts;
	std::set<tstring> names;

	XML::XPathIterator it(TXT("/FarmMonitor/Hosts/Host"), config->getRootElement());
	XML::XPathIterator end;

	for(; it != end; ++it)
	{
		XML::ElementNodePtr node = Core::dynamic_ptr_cast<XML::ElementNode>(*it);

		const tstring host = node->findFirstElement(TXT("Name"))->getTextValue();

		if (host.empty())
			throw Core::ConfigurationException(TXT("The name for a host cannot be empty"));

		if (Core::exists(names, host))
			throw Core::ConfigurationException(TXT("The name for a host cannot be duplicated"));

		const tstring environment = node->findFirstElement(TXT("Environment"))->getTextValue();
		const tstring description = node->findFirstElement(TXT("Description"))->getTextValue();
		const bool    monitor = Core::parse<bool>(node->findFirstElement(TXT("Monitor"))->getTextValue());
		const tstring logon = node->findFirstElement(TXT("Logon"))->getTextValue();
		
		hosts.push_back(makeHost(host, environment, description, monitor, logon));
		names.insert(host);
	}

	std::swap(m_hosts, hosts);
	m_modified = false;
}

////////////////////////////////////////////////////////////////////////////////
//! Save the set of hosts to the XML document.

void Hosts::save(XML::DocumentPtr config)
{
	XML::XPathIterator  it(TXT("/FarmMonitor/Hosts"), config->getRootElement());
	XML::ElementNodePtr hosts(Core::dynamic_ptr_cast<XML::ElementNode>(*it));

	for (size_t i = 0; i != m_hosts.size(); ++i)
	{
		XML::NodePtr properties[] =
		{
			XML::makeElement(TXT("Name"), XML::makeText(m_hosts[i]->m_name)),
			XML::makeElement(TXT("Environment"), XML::makeText(m_hosts[i]->m_environment)),
			XML::makeElement(TXT("Description"), XML::makeText(m_hosts[i]->m_description)),
			XML::makeElement(TXT("Monitor"), XML::makeText(Core::format(m_hosts[i]->m_monitor))),
			XML::makeElement(TXT("Logon"), XML::makeText(m_hosts[i]->m_logon)),
		};

		XML::ElementNodePtr	host = XML::makeElement(TXT("Host"), properties, properties+ARRAY_SIZE(properties));

		hosts->appendChild(host);
	}

	m_modified = false;
}

////////////////////////////////////////////////////////////////////////////////
//! Add a new host. This returns the position it was inserted.

size_t Hosts::add(ConstHostPtr host)
{
	m_hosts.push_back(host);
	m_modified = true;

	return m_hosts.size()-1;
}

////////////////////////////////////////////////////////////////////////////////
//! Replace a host by position.

void Hosts::replace(size_t index, ConstHostPtr host)
{
	ASSERT(index < m_hosts.size());

	m_hosts[index] = host;
	m_modified = true;
}

////////////////////////////////////////////////////////////////////////////////
//! Remove a host by position.

void Hosts::remove(size_t index)
{
	ASSERT(index < m_hosts.size());

	m_hosts.erase(m_hosts.begin() + index);
	m_modified = true;
}

////////////////////////////////////////////////////////////////////////////////
//! Swap two hosts in the collection by index.

void Hosts::swap(size_t first, size_t second)
{
	ASSERT(first < m_hosts.size());
	ASSERT(second < m_hosts.size());

	ConstHostPtr temp = m_hosts[first];
	m_hosts[first] = m_hosts[second];
	m_hosts[second] = temp;
	m_modified = true;
}
