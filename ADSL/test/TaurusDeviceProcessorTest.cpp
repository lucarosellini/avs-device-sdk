
// @file TaurusDeviceProcessorTest.cpp

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ADSL/TaurusDeviceProcessor.h"

using namespace ::testing;

namespace alexaClientSDK {
namespace adsl {
namespace test {

/**
 * TaurusDeviceProcessorTest
 */
class TaurusDeviceProcessorTest : public ::testing::Test {
public:
    void SetUp() override;

    /// A DirectiveProcessor instance to test with.
    std::shared_ptr<TaurusDeviceProcessor> m_processor;
};

void TaurusDeviceProcessorTest::SetUp() {
    m_processor = std::make_shared<TaurusDeviceProcessor>();
}

/**
 * Tests an empty string.  Expect that it is ignored and a failure status (false) is returned.
 */
TEST_F(TaurusDeviceProcessorTest, test_emptyMessage) {
    ASSERT_FALSE(m_processor->containsDeviceConfirmationTag(nullptr));
}


}
}
}