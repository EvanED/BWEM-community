#if 1
#include <tuple>
#include "bwem.h"
#include "mapImpl.h"

#include "MapTest.hpp"

#include "gtest/gtest.h"

class MapDimensionsTest : public ::testing::TestWithParam<MapExpectedResults> {
};

std::pair<int, int>
jsonCoordinatesToPair(nlohmann::json const & j)
{
	return { j[1], j[2] };
}

TEST_P(MapDimensionsTest, VerifyDimensionsLoadedCorrectly) {
	BWEM::detail::MapImpl map;

	auto param = GetParam();
	auto mapName = param.mapName;
	auto tileDimensions = jsonCoordinatesToPair(param.json["tile_dimensions"]);
	auto walkDimensions = jsonCoordinatesToPair(param.json["walk_dimensions"]);
	auto expectedCenter = jsonCoordinatesToPair(param.json["center_position"]);
	int expectedMaxAltitude = param.json["altitude_limit"];

	runOnMap(mapName, [&](auto game) {
		map.Initialize(game);
	});

	auto tileSize = map.Size();
	EXPECT_EQ(tileDimensions.first, tileSize.x);
	EXPECT_EQ(tileDimensions.second, tileSize.y);

	auto walkSize = map.WalkSize();
	EXPECT_EQ(walkDimensions.first, walkSize.x);
	EXPECT_EQ(walkDimensions.second, walkSize.y);

	auto center = map.Center();
	EXPECT_EQ(expectedCenter.first, center.x);
	EXPECT_EQ(expectedCenter.second, center.y);

	auto maxAltitude = map.MaxAltitude();
	EXPECT_EQ(expectedMaxAltitude, maxAltitude);
}

TEST(AltitudeTest, VerifyAltitudeCalculatedCorrectly) {
	BWEM::detail::MapImpl map;

	auto mapName = "data/maps/onlydirt.scm";
	runOnMap(mapName, [&](auto game) {
		map.Initialize(game);
	});

	auto leftTopCorner = map.GetMiniTile(BWAPI::WalkPosition(0, 0));
	EXPECT_EQ(8, leftTopCorner.Altitude());
	auto nearLeftTopCorner = map.GetMiniTile(BWAPI::WalkPosition(1, 1));
	EXPECT_EQ(16, nearLeftTopCorner.Altitude());

	// Around edge of the map in the bottom, there non walkable and non-accessible area
	// which is covered by the UI. That's why we don't have simple formula for calculation
	// altitude on trivial maps.
	auto nearRightBottomCorner = map.GetMiniTile(BWAPI::WalkPosition(510, 510));
	EXPECT_EQ(0, nearRightBottomCorner.Altitude());
	auto rightBottomCorner = map.GetMiniTile(BWAPI::WalkPosition(511, 511));
	EXPECT_EQ(0, rightBottomCorner.Altitude());

	// This is first walkable area not hidden by the UI
	// This is first in sense that if you find closest non-walkcable area hidden by UI
	// then if continue in the next direction closer to center.
	auto farRightBottomPosition = map.GetMiniTile(BWAPI::WalkPosition(491, 506));
	EXPECT_EQ(8, farRightBottomPosition.Altitude());

	auto maxAltitudePoint = map.GetMiniTile(BWAPI::WalkPosition(254, 254));
	EXPECT_EQ(2032, maxAltitudePoint.Altitude());

	auto mapCenter = map.GetMiniTile(BWAPI::WalkPosition(255, 255));
	EXPECT_EQ(2024, mapCenter.Altitude());
}

INSTANTIATE_TEST_CASE_P(
	SynteticCheck,
	MapDimensionsTest,
	::testing::ValuesIn(getTestData(mapsForTest)));

INSTANTIATE_TEST_CASE_P(
	SSCAITMaps,
	MapDimensionsTest,
	::testing::ValuesIn(getTestData(sscaitMaps)));
#endif
