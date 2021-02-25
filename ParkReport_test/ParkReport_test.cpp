#include "stdafx.h"
#include "gtest/gtest.h"

///	@brief
int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();

	return 0;
}