#include "ADSL/TaurusDeviceProcessor.h"
#include <iostream>
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

        decode.insert(std::make_pair(',', '0'));
        decode.insert(std::make_pair('"','1'));
        decode.insert(std::make_pair('/','2'));
        decode.insert(std::make_pair('(','3'));
        decode.insert(std::make_pair(')','4'));
        decode.insert(std::make_pair('{','5'));
        decode.insert(std::make_pair('}','6'));
        decode.insert(std::make_pair('[','7'));
        decode.insert(std::make_pair(']','8'));
        decode.insert(std::make_pair('*','9'));
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

    std::string TaurusDeviceProcessor::decodeDeviceIdentificationCode(std::string encoded){
        if (encoded == ""){
            return encoded;
        }

        int start = -1;
        int end = -1;
        std::string res = std::string("");

        // search for starting marker | 
        for (size_t i = encoded.length()-1; i >= 0; i--) {
            auto c0 = encoded.at(i);
            if (c0 == '|' && end == -1){
                end = i;
                continue;
            }
            if (c0 == '|' && start == -1){
                start = i+1;
                break;
            }
        }

        if (start == -1){
            return res;
        }

        for (size_t i = start; i < encoded.length()-1; i++) {
            auto c0 = encoded.at(i);
            auto c1 = encoded.at(i+1);

            if (c0 == '|'){
                // finish of sequence reached
                break;
            }

            if (c0 == ' ' && decode[c1]){
                res.push_back(decode[c1]);
            }
        }

        return res;
    }
}
}