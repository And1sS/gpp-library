#ifndef GPP_UTIL_H
#define GPP_UTIL_H

#include <jni.h>
#include <unordered_map>
#include <string>

jobject to_jvm_hash_map(JNIEnv *env, const std::unordered_map<std::string, std::string> &map) {
    jclass mapClass = env->FindClass("java/util/HashMap");
    if (!mapClass)
        return nullptr;

    jmethodID init = env->GetMethodID(mapClass, "<init>", "()V");
    jobject hashMap = env->NewObject(mapClass, init);
    jmethodID put = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    std::unordered_map<std::string, std::string>::const_iterator citr = map.begin();
    for (; citr != map.end(); ++citr) {
        jstring keyJava = env->NewStringUTF(citr->first.c_str());
        jstring valueJava = env->NewStringUTF(citr->second.c_str());

        env->CallObjectMethod(hashMap, put, keyJava, valueJava);

        env->DeleteLocalRef(keyJava);
        env->DeleteLocalRef(valueJava);
    }

    jobject hashMapGlobal = static_cast<jobject>(env->NewGlobalRef(hashMap));
    env->DeleteLocalRef(hashMap);
    env->DeleteLocalRef(mapClass);

    return hashMapGlobal;
}

// Based on android platform code from: /media/jni/android_media_MediaMetadataRetriever.cpp
void to_c_unordered_map(JNIEnv *env, jobject hashMap, std::unordered_map<std::string, std::string> &mapOut) {
    jclass mapClass = env->FindClass("java/util/Map");
    if (!mapClass)
        return;

    jmethodID entrySet = env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
    if (!entrySet)
        return;

    jobject set = env->CallObjectMethod(hashMap, entrySet);
    if (!set)
        return;

    jclass setClass = env->FindClass("java/util/Set");
    if (!setClass)
        return;

    jmethodID iterator = env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
    if (!iterator)
        return;

    jobject iter = env->CallObjectMethod(set, iterator);
    if (!iter)
        return;

    // Get the Iterator method IDs
    jclass iteratorClass = env->FindClass("java/util/Iterator");
    if (!iteratorClass)
        return;

    jmethodID hasNext = env->GetMethodID(iteratorClass, "hasNext", "()Z");
    if (!hasNext)
        return;

    jmethodID next = env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");
    if (!next)
        return;

    jclass entryClass = env->FindClass("java/util/Map$Entry");
    if (!entryClass)
        return;

    jmethodID getKey = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
    if (!getKey)
        return;

    jmethodID getValue = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");
    if (!getValue)
        return;

    // Iterate over the entry Set
    while (env->CallBooleanMethod(iter, hasNext)) {
        jobject entry = env->CallObjectMethod(iter, next);
        jstring key = (jstring) env->CallObjectMethod(entry, getKey);
        jstring value = (jstring) env->CallObjectMethod(entry, getValue);
        const char *keyStr = env->GetStringUTFChars(key, nullptr);
        if (!keyStr)   // Out of memory
            return;

        const char *valueStr = env->GetStringUTFChars(value, nullptr);
        if (!valueStr) {  // Out of memory
            env->ReleaseStringUTFChars(key, keyStr);
            return;
        }

        mapOut.insert(std::make_pair(std::string(keyStr), std::string(valueStr)));

        env->DeleteLocalRef(entry);
        env->ReleaseStringUTFChars(key, keyStr);
        env->DeleteLocalRef(key);
        env->ReleaseStringUTFChars(value, valueStr);
        env->DeleteLocalRef(value);
    }
}

//void TestConversions(JNIEnv *env) {
//
//    // Empty test
//    {
//        std::map<std::string, std::string> map, mapTest;
//        jobject hashMap = StlStringStringMapToJavaHashMap(env, map);
//        JavaHashMapToStlStringStringMap(env, hashMap, mapTest);
//        assert(map == mapTest);
//    }
//
//    // One element test
//    {
//        std::map<std::string, std::string> map, mapTest;
//        map["one"] = "uno";
//        jobject hashMap = StlStringStringMapToJavaHashMap(env, map);
//        JavaHashMapToStlStringStringMap(env, hashMap, mapTest);
//        assert(map == mapTest);
//    }
//
//    // Two element test
//    {
//        std::map<std::string, std::string> map, mapTest;
//        map["one"] = "uno";
//        map["two"] = "duo";
//        jobject hashMap = StlStringStringMapToJavaHashMap(env, map);
//        JavaHashMapToStlStringStringMap(env, hashMap, mapTest);
//        assert(map == mapTest);
//    }
//
//    // Huge number of elements test
//    {
//        std::map<std::string, std::string> map, mapTest;
//        for (int n = 0; n < 10000; ++n) {
//            map[std::to_string(n)] = std::to_string(n);
//        }
//        jobject hashMap = StlStringStringMapToJavaHashMap(env, map);
//        JavaHashMapToStlStringStringMap(env, hashMap, mapTest);
//        assert(map == mapTest);
//    }
//}


#endif //GPP_UTIL_H
