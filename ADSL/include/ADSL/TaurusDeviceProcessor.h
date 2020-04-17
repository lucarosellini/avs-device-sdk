#ifndef ALEXA_CLIENT_SDK_ADSL_INCLUDE_ADSL_TAURUSDEVICEPROCESSOR_H_
#define ALEXA_CLIENT_SDK_ADSL_INCLUDE_ADSL_TAURUSDEVICEPROCESSOR_H_

#include <string>
#include <AVSCommon/Utils/LibcurlUtils/HttpPost.h>
#include <AVSCommon/AVS/AVSDirective.h>

namespace alexaClientSDK {
namespace adsl {

class TaurusDeviceProcessor {
public:
    /**
     * Constructor.
     */
    TaurusDeviceProcessor();

    /**
     * Destructor.
     */
    ~TaurusDeviceProcessor();

    /**
    * Checks whether the incoming message contains a Taurus confirmation request from the companion skill.
    */
    bool containsDeviceConfirmationTag(std::shared_ptr<avsCommon::avs::AVSDirective> directive);
private:
    const std::string SAMPLE_APP_CONFIG_KEY = "sampleApp";

    std::string taurusCompanionSkillId;
    std::string taurusConfirmationTag;

    void confirmDevice();
    
};

} // namespace adsl
} // namespace alexaClientSDK

#endif // ALEXA_CLIENT_SDK_ADSL_INCLUDE_ADSL_TAURUSDEVICEPROCESSOR_H_