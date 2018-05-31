#include <vector>
#include <fstream>
#include "MapTest.hpp"

std::vector<MapExpectedResults> getTestData(
	std::vector<std::string> const & v)
{
	std::vector<MapExpectedResults> params;

	for (auto const & test : v) {
		std::string root = test.substr(0, test.size() - 3);
		std::cout << root << "json" << std::endl;
		std::ifstream json_file("../Tests/" + root + "json");
		if (!json_file) {
			params.emplace_back(
				test,
				"could not open JSON file"
			);
		}
		else {
			nlohmann::json json;
			json_file >> json;
			params.emplace_back(test, json);
		}
	}
	return params;
}

std::vector<std::string> mapsForTest = {
	"data/maps/(2)Showdown.scx",
	"data/maps/onlywater.scm",
	"data/maps/onlydirt.scm",
};

std::vector<std::pair<int, int>> mapTileDimensions = {
	{ 64, 192 },
	{ 128, 128 },
	{ 128, 128 },
};

std::vector<std::pair<int, int>> mapWalkDimensions = {
	{ 256, 768 },
	{ 512, 512 },
	{ 512, 512 },
};

std::vector<std::pair<int, int>> mapCenterPositions = {
	{ 1024, 3072 },
	{ 2048, 2048 },
	{ 2048, 2048 },
};

std::vector<std::pair<int, int>> mapAltitudeLimits = {
	{ 0, 426 },
	{ 0, 66 },
	{ 0, 2032 },
};

std::vector<std::string> sscaitMaps = {
	"data/maps/sscai/(2)Benzene.scx",
	"data/maps/sscai/(2)Destination.scx",
	"data/maps/sscai/(2)Heartbreak Ridge.scx",
	
	"data/maps/sscai/(3)Neo Moon Glaive.scx",
	"data/maps/sscai/(3)Tau Cross.scx",
	
	"data/maps/sscai/(4)Andromeda.scx",
	"data/maps/sscai/(4)Circuit Breaker.scx",
	"data/maps/sscai/(4)Electric Circuit.scx",
	"data/maps/sscai/(4)Empire of the Sun.scm",
	"data/maps/sscai/(4)Fighting Spirit.scx",
	"data/maps/sscai/(4)Icarus.scm",
	"data/maps/sscai/(4)Jade.scx",
	"data/maps/sscai/(4)La Mancha1.1.scx",
	"data/maps/sscai/(4)Python.scx",
	"data/maps/sscai/(4)Roadrunner.scx",
};
