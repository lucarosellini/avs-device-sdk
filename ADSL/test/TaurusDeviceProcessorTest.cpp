
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
    m_processor = adsl::TaurusDeviceProcessor::create();
}

/**
 * Tests an empty string.  Expect that it is ignored and a failure status (false) is returned.
 */
TEST_F(TaurusDeviceProcessorTest, test_emptyMessage) {
    ASSERT_TRUE(m_processor->extractVolatileCode("").empty());
}

/**
 * Tests an empty string.  Expect that it is ignored and a failure status (false) is returned.
 */
TEST_F(TaurusDeviceProcessorTest, test_messageFromDifferentDomain) {
    ASSERT_EQ("", m_processor->extractVolatileCode("{\"directive\":{\"header\":{\"namespace\":\"SpeechRecognizer\",\"name\":\"ExpectSpeech\",\"messageId\":\"1d88adb4-7b55-4e04-886e-871a3c3ca2ac\",\"dialogRequestId\":\"daa4b73c-a06d-48ff-af56-aad2793ac350\"},\"payload\":{\"timeoutInMilliseconds\":8000,\"initiator\":{\"type\":\"EXPECT_SPEECH_PROMPT\"}}}}"));
}

TEST_F(TaurusDeviceProcessorTest, test_messageFromDifferentSkill) {
    ASSERT_EQ("", m_processor->extractVolatileCode("{\"directive\":{\"header\":{\"namespace\":\"SpeechSynthesizer\",\"name\":\"Speak\",\"messageId\":\"6740ab33-4614-4096-92fb-14a7b6468b0f\",\"dialogRequestId\":\"548c59dd-bacd-45ee-b435-49e7394178d1\"},\"payload\":{\"playBehavior\":\"ENQUEUE\",\"url\":\"cid:ValidatedSpeakDirective_amzn1.ask.skill.4e9ebdbc-1da8-4e7d-b0f2-6e51ddfb09a8_4eb57d15-22c9-4c3e-8004-44c8b41aad9a_VoiceInitiated_1127527384\",\"format\":\"AUDIO_MPEG\",\"token\":\"amzn1.as-ct.v1.ThirdPartySdkSpeechlet#ACRI#ValidatedSpeakDirective_amzn1.ask.skill.4e9ebdbc-1da8-4e7d-b0f2-6e51ddfb09a8_4eb57d15-22c9-4c3e-8004-44c8b41aad9a_VoiceInitiated\",\"caption\":{\"content\":\"WEBVTT\n\n1\n00:00.000 --> 00:02.223\nTe doy la bienvenida a Bambú.\n\n2\n00:02.223 --> 00:08.989\nPuedes pedirme hacer una meditación rápida o una meditación para ayudarte a dormir.\n\n3\n00:08.989 --> 00:13.339\nSi quieres algo diferente, pídeme sonidos relajantes.\n\n4\n00:13.339 --> 00:15.079\nQué te gustaría hacer?\",\"type\":\"WEBVTT\"}}}}"));
}

TEST_F(TaurusDeviceProcessorTest, test_decodeDeviceIdentificationCodeFromTaurusSkill) {
    ASSERT_EQ("|  [  (  [  }  {  }  |", m_processor->extractVolatileCode("{\"directive\":{\"header\":{\"namespace\":\"SpeechSynthesizer\",\"name\":\"Speak\",\"messageId\":\"249d026a-32ba-43f9-806d-d9d2a853c89a\",\"dialogRequestId\":\"daa4b73c-a06d-48ff-af56-aad2793ac350\"},\"payload\":{\"playBehavior\":\"ENQUEUE\",\"url\":\"cid:ValidatedSpeakDirective_amzn1.ask.skill.dfd47d8f-3eae-43fe-97fd-8ad2994f0a6b_0916c7d7-1588-4dfe-8788-3d4a0953ed95_VoiceInitiated_1745848389\",\"format\":\"AUDIO_MPEG\",\"token\":\"amzn1.as-ct.v1.ThirdPartySdkSpeechlet#ACRI#ValidatedSpeakDirective_amzn1.ask.skill.dfd47d8f-3eae-43fe-97fd-8ad2994f0a6b_0916c7d7-1588-4dfe-8788-3d4a0953ed95_VoiceInitiated\",\"caption\":{\"content\":\"WEBVTT\n\n1\n00:00.000 --> 00:03.093\nTe damos la bienvenida a Taurus MyCook.\n\n2\n00:03.093 --> 00:04.543\nQué quieres hacer?\n\n3\n00:04.543 --> 00:04.543\n|  [  (  [  }  {  }  |\",\"type\":\"WEBVTT\"}}}}"));
}

TEST_F(TaurusDeviceProcessorTest, test_decodeNonEmptyIdentificationCode02) {

    std::string message = std::string("{\"directive\":{\"header\":{\"namespace\":\"SpeechSynthesizer\",\"name\":\"Speak\",\"messageId\":\"bc9a8fa2-c803-4a82-b94a-4f1ac65fc01f\",\"dialogRequestId\":\"89b94c40-f8c0-4bff-9e06-3daa309a40d1\"},\"payload\":{\"playBehavior\":\"ENQUEUE\",\"url\":\"cid:ValidatedSpeakDirective_amzn1.ask.skill.dfd47d8f-3eae-43fe-97fd-8ad2994f0a6b_23f65873-d570-4766-bbe9-710f14b7bb4b_VoiceInitiated_1646221572\",\"format\":\"AUDIO_MPEG\",\"token\":\"amzn1.as-ct.v1.ThirdPartySdkSpeechlet#ACRI#ValidatedSpeakDirective_amzn1.ask.skill.dfd47d8f-3eae-43fe-97fd-8ad2994f0a6b_23f65873-d570-4766-bbe9-710f14b7bb4b_VoiceInitiated\",\"caption\":{\"content\":\"WEBVTT\n\n1\n00:00.000 --> 00:03.093\nTe damos la bienvenida a Taurus MyCook.\n\n2\n00:03.093 --> 00:04.543\nQué quieres hacer?\n\n3\n00:04.543 --> 00:04.543\n|  \"  \"  ,  \"  ,  ]  |\",\"type\":\"WEBVTT\"}}}}");
    
    ASSERT_EQ("|  \"  \"  ,  \"  ,  ]  |", m_processor->extractVolatileCode(message));
}

}
}
}