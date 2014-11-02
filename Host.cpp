////////////////////////////////////////////////////////////////////////////////
//! \file   Host.cpp
//! \brief  The Host class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Host.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Host::Host()
	: m_name()
	, m_environment()
	, m_description()
	, m_monitor(true)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

Host::Host(const tstring& name, const tstring& environment, const tstring& description, bool monitor)
	: m_name(name)
	, m_environment(environment)
	, m_description(description)
	, m_monitor(monitor)
{
}
