#include <internship.h>
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

using namespace internship;
using json = nlohmann::json;

BOOST_AUTO_TEST_CASE( testA )
{
    char[] testjson = R"([{
      "name": "almalinux",
      "os": true,
      "versions": [{
          "cycle": "9",
          "releaseDate": "2022-05-26",
          "eol": "2032-05-31"
        },{
          "cycle": "8",
          "releaseDate": "2021-03-30",
          "eol": "2029-03-01"
        }
      ]
    }])";
    json j = json::parse(testjson);

    BOOST_CHECK( differenceInDays("2021-10-5","2021-10-10") == 6 );

    BOOST_CHECK( loadOSfromJSON("test.json") == j );

    addSupportPeriod(j);

    BOOST_CHECK(j[0]["versions"][0]["supportCycle"] == 3659 );

    json n = getNewestVersion(j);

    BOOST_CHECK( n[0]["cycle"] == 9);


}