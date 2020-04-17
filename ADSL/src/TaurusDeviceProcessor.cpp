#include "ADSL/TaurusDeviceProcessor.h"

namespace alexaClientSDK {
namespace adsl {

    using namespace avsCommon;
    using namespace avsCommon::avs;
    using namespace avsCommon::utils::configuration;
    using namespace avsCommon::utils::logger;

    static const std::string TAG("TaurusDeviceProcessor");

    /**
     * Create a LogEntry using this file's TAG and the specified event string.
     *
     * @param The event string for this @c LogEntry.
     */
    #define LX(event) LogEntry(TAG, event)

    TaurusDeviceProcessor::TaurusDeviceProcessor(){
        auto config = ConfigurationNode::getRoot();
        auto sampleAppConfig = config[SAMPLE_APP_CONFIG_KEY];

        sampleAppConfig.getString("taurusCompanionSkillId", &taurusCompanionSkillId);
        sampleAppConfig.getString("taurusConfirmationTag", &taurusConfirmationTag);

        ACSDK_DEBUG1(LX(__func__).d("taurusCompanionSkillId", taurusCompanionSkillId).d("taurusConfirmationTag", taurusConfirmationTag));
    }

    TaurusDeviceProcessor::~TaurusDeviceProcessor(){
        
    }

    bool TaurusDeviceProcessor::containsDeviceConfirmationTag(std::shared_ptr<avsCommon::avs::AVSDirective> directive){
        if (directive == nullptr){
            return false;
        }

        return false;
    }

    void TaurusDeviceProcessor::confirmDevice(){

    }
}
}