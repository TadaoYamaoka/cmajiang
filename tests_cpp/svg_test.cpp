#include "pch.h"
#include "../src_cpp/svg.h"

TEST(SVGTest, shoupai_to_svg) {
	EXPECT_NO_THROW(shoupai_to_svg(Shoupai("m235678p369s1z124")));
	EXPECT_NO_THROW(shoupai_to_svg(Shoupai("m123p123z1z1,s1-23,z222=")));
	EXPECT_NO_THROW(shoupai_to_svg(Shoupai("m123p456s789z4567*")));
	EXPECT_NO_THROW(shoupai_to_svg(Shoupai("m234z1177,p1111,s9999")));
	// â¡û»
	EXPECT_NO_THROW(shoupai_to_svg(Shoupai("m123p123z1z1,s1-23,z222+2")));
}