////////////////////////////////////////////////////////////////////////////////
//! \file   Model.cpp
//! \brief  The Model class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Model.hpp"
#include <WCL/AppConfig.hpp>
#include <WCL/Path.hpp>
#include <shlobj.h>
#include <XML/Reader.hpp>
#include <XML/Writer.hpp>
#include <WCL/File.hpp>
#include <Core/FileSystemException.hpp>

////////////////////////////////////////////////////////////////////////////////
// Constants.

namespace
{
//! The configuration data format version.
const tchar* CONFIG_VERSION = TXT("1");
//! The default config file folder.
const tchar* DEFAULT_CONFIG_FOLDER = TXT("FarmMonitor");
//! The default config file name.
const tchar* DEFAULT_CONFIG_FILE = TXT("FarmMonitor.xml");
}

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Model::Model()
	: m_hosts()
	, m_tools()
	, m_queries()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

Model::~Model()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Query if the application data file exists.

bool Model::configFileExists()
{
	const CPath configFile = CPath::SpecialDir(CSIDL_APPDATA) / DEFAULT_CONFIG_FOLDER / DEFAULT_CONFIG_FILE;

	return configFile.Exists();
}

////////////////////////////////////////////////////////////////////////////////
//! Load the application settings.

void Model::loadConfig()
{
	const CPath configFile = CPath::SpecialDir(CSIDL_APPDATA) / DEFAULT_CONFIG_FOLDER / DEFAULT_CONFIG_FILE;

	if (configFile.Exists())
	{
		loadConfigFromXmlFile(configFile);
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Load the application settings.

void Model::loadConfig(XML::DocumentPtr config)
{
	m_hosts.load(config);
	m_tools.load(config);
	m_queries.load(config);
}

////////////////////////////////////////////////////////////////////////////////
//! Save the application settings.

void Model::saveConfig()
{
	if (m_hosts.isModified() || m_tools.isModified() || m_queries.isModified())
	{
		const CPath userDataFolder = CPath::SpecialDir(CSIDL_APPDATA);

		if (!userDataFolder.Exists())
		{
			throw Core::FileSystemException(Core::fmt(TXT("The Application Data folder is invalid - '%s'"), userDataFolder.c_str()));
		}

		const CPath dataFolder = userDataFolder / DEFAULT_CONFIG_FOLDER;

		if (!dataFolder.Exists())
		{
			if (!CFile::CreateFolder(dataFolder))
			{
				throw Core::FileSystemException(Core::fmt(TXT("Failed to create the folder - '%s'"), dataFolder.c_str()));
			}
		}

		const CPath configFile = dataFolder / DEFAULT_CONFIG_FILE;

		saveConfigToXmlFile(configFile);
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Save the application settings.

void Model::saveConfig(XML::DocumentPtr config)
{
	XML::ElementNodePtr root = XML::makeElement(TXT("FarmMonitor"));

	root->setAttribute(TXT("Version"), CONFIG_VERSION);
	root->appendChild(XML::makeElement(TXT("Hosts")));
	root->appendChild(XML::makeElement(TXT("Tools")));
	root->appendChild(XML::makeElement(TXT("Queries")));
	config->appendChild(root);

	m_hosts.save(config);
	m_tools.save(config);
	m_queries.save(config);
}

////////////////////////////////////////////////////////////////////////////////
//! Load the application settings from the default XML file.

void Model::loadConfigFromXmlFile(const CPath& configFile)
{
	const tstring          content = CFile::ReadTextFile(configFile);
	const XML::DocumentPtr appConfig = XML::Reader::readDocument(content, XML::Reader::DISCARD_WHITESPACE);

	loadConfig(appConfig);
}

////////////////////////////////////////////////////////////////////////////////
//! Save the application settings to the default XML file.

void Model::saveConfigToXmlFile(const CPath& configFile)
{
	XML::DocumentPtr appConfig = XML::makeDocument();

	saveConfig(appConfig);

	const CPath backupFile = configFile.PathWithoutExt() + TXT(".bak");
	const CPath tempFile = configFile.PathWithoutExt() + TXT(".$$$");

	if (tempFile.Exists())
	{
		if (!CFile::Delete(tempFile))
		{
			throw Core::FileSystemException(Core::fmt(TXT("Failed to delete previous temporary file:\n\n%s"),
															tempFile.c_str()));
		}
	}

	const tstring content = XML::Writer::writeDocument(appConfig);
	CFile::WriteTextFile(tempFile, content.c_str(), ANSI_TEXT); 

	if (backupFile.Exists())
	{
		if (!CFile::Delete(backupFile))
		{
			throw Core::FileSystemException(Core::fmt(TXT("Failed to delete backup file:\n\n%s"),
															backupFile.c_str()));
		}
	}

	if (configFile.Exists())
	{
		if (!CFile::Move(configFile, backupFile))
		{
			throw Core::FileSystemException(Core::fmt(TXT("Failed to rename file:\n\n%s\n\nto\n\n%s"),
															configFile.c_str(), backupFile.c_str()));
		}
	}
	
	if (!CFile::Move(tempFile, configFile))
	{
		CFile::Move(backupFile, configFile);

		throw Core::FileSystemException(Core::fmt(TXT("Failed to rename file:\n\n%s\n\nto\n\n%s"),
														tempFile.c_str(), configFile.c_str()));
	}
}
