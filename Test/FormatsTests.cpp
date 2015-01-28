////////////////////////////////////////////////////////////////////////////////
//! \file   FormatsTests.cpp
//! \brief  The unit tests for the Formats class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "Formats.hpp"
#include <algorithm>

bool MatchAny(const Formats::FormatMapping&)
{
	return true;
}

TEST_SET(Formats)
{
	Formats formats;

TEST_CASE("the collections of formats can be enumerated")
{
	Formats::ConstIter begin = formats.begin();
	Formats::ConstIter end = formats.end();

	size_t count = std::count_if(begin, end, MatchAny);

	TEST_TRUE(count == formats.size());
}
TEST_CASE_END

TEST_CASE("the display name can be found from the format")
{
	const tstring format = TXT("B");
	const tstring displayName = TXT("Bytes (B)");

	TEST_TRUE(formats.getDisplayName(format) == displayName);
}
TEST_CASE_END

TEST_CASE("the format can be found from the display name")
{
	const tstring format = TXT("B");
	const tstring displayName = TXT("Bytes (B)");

	TEST_TRUE(formats.getFormat(displayName) == format);
}
TEST_CASE_END

}
TEST_SET_END
