////////////////////////////////////////////////////////////////////////////////
//! \file   ModelTests.cpp
//! \brief  The unit tests for the Model class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "Model.hpp"

TEST_SET(Model)
{

TEST_CASE("saving the model sets the document version")
{
	XML::DocumentPtr document = XML::makeDocument();

	Model model;

	model.saveConfig(document);

	TEST_TRUE(document->hasRootElement());

	XML::ElementNodePtr root = document->getRootElement();

	TEST_TRUE(root->name() == TXT("FarmMonitor"));

	TEST_TRUE(root->getAttributes().count() == 1);
	TEST_TRUE(root->getAttributeValue(TXT("Version")) == TXT("1"));
}
TEST_CASE_END

}
TEST_SET_END
