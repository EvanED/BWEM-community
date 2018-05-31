#if 1
#include "bwem.h"
#include "mapImpl.h"

#include "MapTest.hpp"

#include "gtest/gtest.h"

class MapBasesInfo : public ::testing::TestWithParam<MapExpectedResults> {
};

TEST_P(MapBasesInfo, MapFindBasesForStartingLocations) {
	BWEM::detail::MapImpl map;

	auto param = GetParam();
	auto mapName = param.mapName;
	
	runOnMap(mapName, [&](auto game) {
		map.Initialize(game);
	});

	EXPECT_EQ(true, map.FindBasesForStartingLocations());
}

template <typename Container, typename Predicate>
size_t count_if_c(Container const & c, Predicate pred)
{
	return std::count_if(std::begin(c), std::end(c), pred);
}

TEST_P(MapBasesInfo, MapStartLocationCount2PlayersMap) {
	BWEM::detail::MapImpl map;

	auto param = GetParam();
	auto mapName = param.mapName;
	int expectedNumberStarts = count_if_c(
		param.json["bases"],
		[](auto const & base_json) {
		    return base_json["starting_location"].get<bool>();
	});

  runOnMap(mapName, [&](auto game) {
    map.Initialize(game);
  });

    EXPECT_EQ(expectedNumberStarts, map.StartingLocations().size());
	EXPECT_EQ(2, map.StartingLocations().size());
}

TEST_P(MapBasesInfo, MapBaseCount) {
	BWEM::detail::MapImpl map;

	auto param = GetParam();
	auto mapName = param.mapName;
	int expectedBases = param.json["bases"].size();

  runOnMap(mapName, [&](auto game) {
    map.Initialize(game);
  });
	
	EXPECT_EQ(expectedBases, map.BaseCount());
	EXPECT_EQ(8, map.BaseCount());
}

INSTANTIATE_TEST_CASE_P(
	BasicCheck,
	MapBasesInfo,
	::testing::ValuesIn(getTestData({
		"data/maps/(2)Showdown.scx"
		})));

#endif
