/*
 * Copyright (c) 2019, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SYSTEM_SECURITY_CREDENTIAL_H_
#define SYSTEM_SECURITY_CREDENTIAL_H_

#include <string>
#include <vector>

#include <android/security/identity/BnCredential.h>

#include <android/hardware/identity/1.0/IIdentityCredentialStore.h>
#include <android/hardware/identity/1.0/types.h>

#include "CredentialData.h"

namespace android {
namespace security {
namespace identity {

using ::android::sp;
using ::android::binder::Status;
using ::std::string;
using ::std::vector;

using ::android::hardware::identity::V1_0::IIdentityCredential;
using ::android::hardware::identity::V1_0::IIdentityCredentialStore;

class Credential : public BnCredential {
  public:
    Credential(const string& dataPath, const string& credentialName);
    ~Credential();

    Status loadCredential(sp<IIdentityCredentialStore> halStoreBinder);

    // ICredential overrides
    Status createEphemeralKeyPair(vector<uint8_t>* _aidl_return) override;

    Status setReaderEphemeralPublicKey(const vector<uint8_t>& publicKey) override;

    Status deleteCredential(vector<uint8_t>* _aidl_return) override;

    Status getCredentialKeyCertificateChain(vector<uint8_t>* _aidl_return) override;

    Status selectAuthKey(bool allowUsingExhaustedKeys, int64_t* _aidl_return) override;

    Status getEntries(const vector<uint8_t>& requestMessage,
                      const vector<RequestNamespaceParcel>& requestNamespaces,
                      const vector<uint8_t>& sessionTranscript,
                      const vector<uint8_t>& readerSignature, bool allowUsingExhaustedKeys,
                      GetEntriesResultParcel* _aidl_return) override;

    Status setAvailableAuthenticationKeys(int32_t keyCount, int32_t maxUsesPerKey) override;
    Status getAuthKeysNeedingCertification(vector<AuthKeyParcel>* _aidl_return) override;
    Status storeStaticAuthenticationData(const AuthKeyParcel& authenticationKey,
                                         const vector<uint8_t>& staticAuthData) override;
    Status getAuthenticationDataUsageCount(vector<int32_t>* _aidl_return) override;

  private:
    string dataPath_;
    string credentialName_;

    const AuthKeyData* selectedAuthKey_ = nullptr;
    uint64_t selectedChallenge_ = 0;

    sp<CredentialData> data_;

    sp<IIdentityCredential> halBinder_;
};

}  // namespace identity
}  // namespace security
}  // namespace android

#endif  // SYSTEM_SECURITY_IDENTITY_CREDENTIAL_H_