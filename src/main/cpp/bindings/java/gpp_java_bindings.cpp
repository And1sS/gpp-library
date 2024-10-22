#include "org_prebid_server_GppRegulations.h"
#include "../../lib/GppDecoder.h"
#include <iostream>

void String_Intern(JNIEnv *env, jstring str) {
    jclass stringClass = env->FindClass("Ljava/lang/String;");
    jmethodID internMethodId = env->GetMethodID(stringClass, "intern", "()Ljava/lang/String;");
    env->CallObjectMethod(str, internMethodId);
}

jobject JNICALL
Java_org_prebid_server_GppRegulations_decodeSections(JNIEnv *env, jclass clazz, jstring jEncoded) {
    if (jEncoded == nullptr)
        throw "THROW PROPER EXCEPTION";

    std::cout << "HELLO FROM C++!" << std::endl;

    const char *cEncoded = env->GetStringUTFChars(jEncoded, nullptr);
    if (!cEncoded)
        throw "THROW PROPER EXCEPTION";

    std::string encoded = cEncoded;

    decode_gpp_string(encoded);
    std::cout << encoded << std::endl;

    env->ReleaseStringUTFChars(jEncoded, cEncoded);

    return nullptr;
}

JNIEXPORT jstring JNICALL
Java_org_prebid_server_GppRegulations_encodeSections(JNIEnv *env, jclass clazz, jobject self) {
    std::cout << "HELLO FROM C++!" << std::endl;

    jstring str = env->NewStringUTF("test");
    String_Intern(env, str);

    return str;
}

