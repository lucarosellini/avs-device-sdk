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

#ifndef ALEXA_CLIENT_SDK_ADSL_INCLUDE_ADSL_TAURUSDEVICEPROCESSOR_H_
#define ALEXA_CLIENT_SDK_ADSL_INCLUDE_ADSL_TAURUSDEVICEPROCESSOR_H_

#include <string>
#include <map>
#include <AVSCommon/Utils/LibcurlUtils/HttpPostInterface.h>

namespace alexaClientSDK {
namespace adsl {

/**
 * Implements the logic to parse, decode and confirm volatile identification codes
 * coming embedded in the speech response sent by the MyCook companion skill.
 */ 
class TaurusDeviceProcessor {
public:
    /**
     * Creates a TaurusDeviceProcessor.
     */
    static std::shared_ptr<TaurusDeviceProcessor> create();

    /** 
     * Checks whether the incoming message contains an encoded volatile conformation code
     * and, if yes, decodes it.
     * 
     * @param directivePayload the payload extracted from the directive coming from AVS.
     * 
     * @return the decoded device confirmation code if present or the empty string if not.
     */
    std::string decodeVolatileIdentificationCode(std::string directivePayload);

    /**
     * Confirms this device is a MyCook device by calling the Taurus Device 
     * Identification service.
     * 
     * @param deviceIdentificationCode the decoded device identification code.
     */ 
    void confirmDevice(std::string deviceIdentificationCode);

private:
    /**
     * Constructor.
     */
    TaurusDeviceProcessor();

    // String to identify the object in the config file from which properties will be fetched.
    const std::string SAMPLE_APP_CONFIG_KEY = "sampleApp";

    // Authentication header used to authenticate calls to the device confirmation service.
    const std::string HEADER_LINE_AUTH = "x_taurus_auth: ";

    // Content type header, used to perform calls to the device confirmation service.    
    const std::string HEADER_LINE_CONTENT_TYPE = "Content-Type: application/json";
    
    // Field name of the decoded device uuid sent back to the device confirmation service to confirm the device. 
    const std::string POST_DATA_DEVICE_ID = "tempDeviceUUID";

    // SkillId of the MyCook companion custom skill.
    std::string taurusCompanionSkillId;

    // URL of the device confirmation service.
    std::string taurusConfirmationServiceURL;

    // value of the authentication token used to autenticate against the confirmation service.
    std::string taurusAuthenticationToken;

    // Timeout for calls to the device confirmation service.
    int taurusConfirmationRequestTimeout;

    /* Decoding table used to decode encoded volatile device 
     * identification codes generated by the device confirmation 
     * service through the MyCook companion skill.
     */
    std::map<char, char> decode;

    // Pointer to the http client used to perform calls to the device confirmation service.
    std::shared_ptr<avsCommon::utils::libcurlUtils::HttpPostInterface> m_httpPost;

    /** 
     * Implements the decoding logic for the encoded volatile device identification codes
     * coming in the speech response of the MyCook companion skill.
     * 
     * @param payload the payload extracted from the AVS directive potentially containing the encoded volatile device identification code.
     * @return the decoded code or the empty string if no volatile code is present.
     */
    std::string doDecodeVolatileIdentificationCode(std::string payload);
};

} // namespace adsl
} // namespace alexaClientSDK

#endif // ALEXA_CLIENT_SDK_ADSL_INCLUDE_ADSL_TAURUSDEVICEPROCESSOR_H_