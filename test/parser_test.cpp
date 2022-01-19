#include "../src/JsEngine.h"
#include <ctime>
#include <spdlog/spdlog.h>
#include <doctest/doctest.h>

TEST_CASE("parsing a number") {
    Engine engine = Engine();
    Context ctx = engine.getContext();
    Value val = ctx.eval("1");
    CHECK(getTypeName(val) == "number");
    CHECK(toNumber(val) == 1);
}
