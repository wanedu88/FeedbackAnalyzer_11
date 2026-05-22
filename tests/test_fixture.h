#pragma once

#include <gtest/gtest.h>
#include "Constants.h"
#include "Filters.h"

class FeedbackAnalyzerTestFixture : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        Constants::init();
        Filters::initFilterKeywords();
    }
};
