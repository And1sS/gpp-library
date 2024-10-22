#include "org_prebid_server_GppRegulations.h"
#include "../../lib/gpp.h"
#include "util.h"
#include <iostream>

jobject JNICALL
Java_org_prebid_server_GppRegulations_decodeSections(JNIEnv *env, jclass clazz, jstring jEncoded) {
    if (!jEncoded)
        throw "THROW PROPER EXCEPTION";

    const char *cEncoded = env->GetStringUTFChars(jEncoded, nullptr);
    if (!cEncoded)
        throw "THROW PROPER EXCEPTION";

    std::string encoded = cEncoded;
    env->ReleaseStringUTFChars(jEncoded, cEncoded);

    return to_jvm_hash_map(env, decode_gpp_string(std::move(encoded)));
}

JNIEXPORT jstring JNICALL
Java_org_prebid_server_GppRegulations_encodeSections(JNIEnv *env, jclass clazz, jobject jMap) {
    std::cout << "HELLO FROM C++!" << std::endl;

    std::unordered_map<std::string, std::string> cMap;
    to_c_unordered_map(env, jMap, cMap);

    std::string encoded = encode_gpp_sections(std::move(cMap));
    return env->NewStringUTF(encoded.c_str());
}

