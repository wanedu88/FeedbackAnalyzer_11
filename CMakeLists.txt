cmake_minimum_required(VERSION 3.14)
project(FeedbackAnalyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Forward slashes so PROJECT_ROOT is safe inside C++ string literals on Windows.
string(REPLACE "\\" "/" PROJECT_ROOT_FORWARD "${CMAKE_SOURCE_DIR}")

add_executable(feedback_analyzer
    src/cpp/main.cpp
    src/cpp/Constants.cpp
    src/cpp/infra/KeywordFileDb.cpp
    src/cpp/Filters.cpp
    src/cpp/Logger.cpp
    src/cpp/Session.cpp
    src/cpp/TextAnalyzer.cpp
    src/cpp/UIComponents.cpp
    src/cpp/ParseUtils.cpp
    src/cpp/SentimentClassifier.cpp
    src/cpp/HtmlRenderer.cpp
    src/cpp/services/TrendLoader.cpp
    src/cpp/services/TrendAggregator.cpp
    src/cpp/services/TrendStore.cpp
    src/cpp/handlers/Handlers.cpp
)

target_include_directories(feedback_analyzer PRIVATE src/cpp)
target_compile_definitions(feedback_analyzer PRIVATE
    "PROJECT_ROOT=\"${PROJECT_ROOT_FORWARD}\"")
target_link_libraries(feedback_analyzer PRIVATE nlohmann_json::nlohmann_json)

# Windows: httplib mmap uses CreateFile2 / CreateFileMappingFromApp (Win8+).
# MinGW defaults to older WINVER unless raised here.
if(WIN32)
    target_compile_definitions(feedback_analyzer PRIVATE
        _WIN32_WINNT=0x0A00
        WINVER=0x0A00
    )
    target_link_libraries(feedback_analyzer PRIVATE ws2_32)
endif()

# --- Mission 2: unit tests (GoogleTest via FetchContent) ---
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

enable_testing()

add_executable(feedback_analyzer_tests
    tests/text_analyzer_test.cpp
    tests/filters_test.cpp
    tests/parse_utils_test.cpp
    tests/coverage_gap_test.cpp
    tests/regression_neutral_filter_test.cpp
    tests/keyword_file_db_test.cpp
    tests/trend_loader_test.cpp
    src/cpp/Constants.cpp
    src/cpp/infra/KeywordFileDb.cpp
    src/cpp/Filters.cpp
    src/cpp/TextAnalyzer.cpp
    src/cpp/ParseUtils.cpp
    src/cpp/SentimentClassifier.cpp
    src/cpp/services/TrendLoader.cpp
    src/cpp/services/TrendAggregator.cpp
)

target_include_directories(feedback_analyzer_tests PRIVATE src/cpp)
target_compile_definitions(feedback_analyzer_tests PRIVATE
    "PROJECT_ROOT=\"${PROJECT_ROOT_FORWARD}\"")
target_link_libraries(feedback_analyzer_tests PRIVATE GTest::gtest_main nlohmann_json::nlohmann_json)

# Line coverage for gcovr (Mission 2 — domain sources in test target only)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(feedback_analyzer_tests PRIVATE --coverage -fprofile-arcs -ftest-coverage)
    target_link_options(feedback_analyzer_tests PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_discover_tests(feedback_analyzer_tests)
