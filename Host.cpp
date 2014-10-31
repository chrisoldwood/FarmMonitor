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
{
}

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

Host::Host(const tstring& name, const tstring& environment, const tstring& description)
	: m_name(name)
	, m_environment(environment)
	, m_description(description)
{
}
