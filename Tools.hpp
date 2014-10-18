////////////////////////////////////////////////////////////////////////////////
//! \file   Tools.hpp
//! \brief  The Tools class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_TOOLS_HPP
#define APP_TOOLS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Tool.hpp"
#include <Core/SharedPtr.hpp>
#include <XML/Document.hpp>

// Forward declarations.
namespace WCL
{
class IAppConfigReader;
class IAppConfigWriter;
}

////////////////////////////////////////////////////////////////////////////////
//! The container type for the collection of invocable tools.

class Tools : private Core::NotCopyable
{
private:
	//! The type used to store the collection of tools.
	typedef std::vector<ConstToolPtr> Collection;

public:
	//! The const iterator type.
	typedef Collection::const_iterator const_iterator;

	//! Default constructor.
	Tools();

	//! Destructor.
	~Tools();
	
	//
	// Properties.
	//

	//! Get the number of tools.
	size_t size() const;
	
	//! Get the tool at a given position.
	const ConstToolPtr& tool(size_t index) const;

	//! Has the collection of tools been modified?
	bool isModified() const;

	//! Get the collection start iterator.
	const_iterator begin() const;

	//! Get the collection end iterator.
	const_iterator end() const;

	//
	// Methods.
	//

	//! Add a tool to the collection.
	void append(ConstToolPtr tool);

	//! Replace a tool in the collection.
	void replace(size_t index, ConstToolPtr tool);

	//! Remove a tool from the collection.
	void remove(size_t index);

	//! Load the set of tools from the application config.
	void load(WCL::IAppConfigReader& config);

	//! Save the set of tools to the application config.
	void save(WCL::IAppConfigWriter& config);

	//! Load the set of tools from the XML document.
	void load(const XML::DocumentPtr config);

	//! Save the set of tools to the XML document.
	void save(XML::DocumentPtr config);

	//! Create a deep copy of another collection.
	void deepCopy(const Tools& rhs);

	//! Replace the entire collection with another.
	void replaceAll(const Tools& rhs);

private:
	//
	// Members.
	//
	bool		m_modified;		//!< Has the collection been modified?
	Collection	m_tools;		//!< The collection of tool definitions.

	//! Create a deep copy of another collection.
	void deepCopy(const Tools& rhs, bool modified);
};

#endif // APP_TOOLS_HPP
