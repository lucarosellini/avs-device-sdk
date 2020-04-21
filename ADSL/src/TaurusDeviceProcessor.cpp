/*
 * Copyright 2017-2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include "ADSL/TaurusDeviceProcessor.h"
#include <AVSCommon/Utils/LibcurlUtils/HttpPost.h>
#include <iostream>
namespace alexaClientSDK {
namespace adsl {

    using namespace avsCommon;
    using namespace avsCommon::utils::configuration;
    using namespace avsCommon::utils::logger;
    using namespace alexaClientSDK::avsCommon::utils::libcurlUtils;

    static const std::string TAG("TaurusDeviceProcessor");

    /**
     * Create a LogEntry using this file's TAG and the specified event string.
     *
     * @param The event string for this @c LogEntry.
     */
    #define LX(event) LogEntry(TAG, event)

    std::shared_ptr<TaurusDeviceProcessor> TaurusDeviceProcessor::create(){
        return std::shared_ptr<TaurusDeviceProcessor>(new TaurusDeviceProcessor());
    }

    TaurusDeviceProcessor::TaurusDeviceProcessor(){
        m_httpPost = HttpPost::create();
        auto config = ConfigurationNode::getRoot();
        auto sampleAppConfig = config[SAMPLE_APP_CONFIG_KEY];

        sampleAppConfig.getString("taurusCompanionSkillId", &taurusCompanionSkillId);
        sampleAppConfig.getString("taurusConfirmationServiceURL", &taurusConfirmationServiceURL);
        sampleAppConfig.getString("taurusAuthenticationToken", &taurusAuthenticationToken);
        sampleAppConfig.getInt("taurusConfirmationRequestTimeout", &taurusConfirmationRequestTimeout);
        
        ACSDK_DEBUG1(LX(__func__).d("taurusCompanionSkillId", taurusCompanionSkillId));
        ACSDK_DEBUG1(LX(__func__).d("taurusConfirmationServiceURL", taurusConfirmationServiceURL));
       
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

    std::string TaurusDeviceProcessor::decodeVolatileIdentificationCode(std::string directivePayload){
        std::string res = std::string{""};
        
        if (directivePayload.empty()){
            ACSDK_DEBUG1(LX(__func__).m("Payload is empty"));
            return res;
        }
        
        if (directivePayload.find(taurusCompanionSkillId) == std::string::npos){
            ACSDK_DEBUG1(LX(__func__).m("Request not from Taurus companion skill"));
            return res;
        }

        ACSDK_DEBUG1(LX(__func__).m("Request coming from Taurus companion skill"));

        return doDecodeVolatileIdentificationCode(directivePayload);
    }

    void TaurusDeviceProcessor::confirmDevice(std::string deviceIdentificationCode){

        ACSDK_DEBUG1(LX(__func__).d("Confirming device with id", deviceIdentificationCode));
        
        const std::vector<std::string> headerLines = {HEADER_LINE_CONTENT_TYPE,
                                                  HEADER_LINE_AUTH + taurusAuthenticationToken};

        const std::string postData = "{\""+POST_DATA_DEVICE_ID+"\":\""+deviceIdentificationCode+"\"}";

        auto response = m_httpPost->doPost(taurusConfirmationServiceURL, 
                                           headerLines, 
                                           postData, 
                                           std::chrono::seconds{taurusConfirmationRequestTimeout});

        // TODO: check response
        ACSDK_DEBUG1(LX(__func__).d("Confirmation result code", response.code));
        ACSDK_DEBUG1(LX(__func__).d("Confirmation result body", response.body));
    }

    std::string TaurusDeviceProcessor::doDecodeVolatileIdentificationCode(std::string encoded){
        if (encoded == ""){
            return encoded;
        }

        int start = -1;
        int end = -1;
        std::string res = std::string("");

        // search for starting marker | 
        for (int i = encoded.length()-1; i >= 0; i--) {
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
            ACSDK_DEBUG1(LX(__func__).m("device identification code not found."));
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

        ACSDK_DEBUG1(LX(__func__).d("device identification code found",res));

        return res;
    }
}
}