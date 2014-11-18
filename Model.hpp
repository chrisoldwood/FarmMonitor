////////////////////////////////////////////////////////////////////////////////
//! \file   Model.hpp
//! \brief  The Model class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_MODEL_HPP
#define APP_MODEL_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Hosts.hpp"
#include "Tools.hpp"
#include "Queries.hpp"

// Forward declarations.
class CPath;

////////////////////////////////////////////////////////////////////////////////
//! The application data model.

class Model
{
public:
	//! Default constructor.
	Model();

	//! Destructor.
	~Model();
	
public:
	//
	// Members.
	//
	Hosts		m_hosts;		//!< The hosts being monitored.
	Tools		m_tools;		//!< The external tools to invoke.
	Queries		m_queries;		//!< The WMI queries to execute.

	//
	// Methods.
	//

	//! Query if the application data file exists.
	bool configFileExists();

	//! Load the application settings.
	void loadConfig();

	//! Load the application settings.
	void loadConfig(XML::DocumentPtr config);

	//! Save the application settings.
	void saveConfig();

	//! Save the application settings.
	void saveConfig(XML::DocumentPtr config);

private:
	//
	// Internal methods.
	//

	//! Load the application settings from the app config store.
	void loadConfigFromAppConfig();

	//! Save the application settings to the app config store.
	void saveConfigToAppConfig();

	//! Load the application settings from the default XML file.
	void loadConfigFromXmlFile(const CPath& configFile);

	//! Save the application settings to the default XML file.
	void saveConfigToXmlFile(const CPath& configFile);
};

#endif // APP_MODEL_HPP
