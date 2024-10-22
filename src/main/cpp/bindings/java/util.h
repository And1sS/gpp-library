#ifndef GPP_UTIL_H
#define GPP_UTIL_H

#include <jni.h>
#include <map>
#include <string>

jobject StlStringStringMapToJavaHashMap(JNIEnv *env, const std::map<std::string, std::string> &map) {
    jclass mapClass = env->FindClass("java/util/HashMap");
    if (mapClass == NULL)
        return NULL;

    jmethodID init = env->GetMethodID(mapClass, "<init>", "()V");
    jobject hashMap = env->NewObject(mapClass, init);
    jmethodID put = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    std::map<std::string, std::string>::const_iterator citr = map.begin();
    for (; citr != map.end(); ++citr) {
        jstring keyJava = env->NewStringUTF(citr->first.c_str());
        jstring valueJava = env->NewStringUTF(citr->second.c_str());

        env->CallObjectMethod(hashMap, put, keyJava, valueJava);

        env->DeleteLocalRef(keyJava);
        env->DeleteLocalRef(valueJava);
    }

    jobject hashMapGobal = static_cast<jobject>(env->NewGlobalRef(hashMap));
    env->DeleteLocalRef(hashMap);
    env->DeleteLocalRef(mapClass);

    return hashMapGobal;
}

// Based on android platform code from: /media/jni/android_media_MediaMetadataRetriever.cpp
void JavaHashMapToStlStringStringMap(JNIEnv *env, jobject hashMap, std::map<std::string, std::string> &mapOut) {
    // Get the Map's entry Set.
    jclass mapClass = env->FindClass("java/util/Map");
    if (mapClass == NULL) {
        return;
    }
    jmethodID entrySet =
            env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
    if (entrySet == NULL) {
        return;
    }
    jobject set = env->CallObjectMethod(hashMap, entrySet);
    if (set == NULL) {
        return;
    }
    // Obtain an iterator over the Set
    jclass setClass = env->FindClass("java/util/Set");
    if (setClass == NULL) {
        return;
    }
    jmethodID iterator =
            env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
    if (iterator == NULL) {
        return;
    }
    jobject iter = env->CallObjectMethod(set, iterator);
    if (iter == NULL) {
        return;
    }
    // Get the Iterator method IDs
    jclass iteratorClass = env->FindClass("java/util/Iterator");
    if (iteratorClass == NULL) {
        return;
    }
    jmethodID hasNext = env->GetMethodID(iteratorClass, "hasNext", "()Z");
    if (hasNext == NULL) {
        return;
    }
    jmethodID next =
            env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");
    if (next == NULL) {
        return;
    }
    // Get the Entry class method IDs
    jclass entryClass = env->FindClass("java/util/Map$Entry");
    if (entryClass == NULL) {
        return;
    }
    jmethodID getKey =
            env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
    if (getKey == NULL) {
        return;
    }
    jmethodID getValue =
            env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");
    if (getValue == NULL) {
        return;
    }
    // Iterate over the entry Set
    while (env->CallBooleanMethod(iter, hasNext)) {
        jobject entry = env->CallObjectMethod(iter, next);
        jstring key = (jstring) env->CallObjectMethod(entry, getKey);
        jstring value = (jstring) env->CallObjectMethod(entry, getValue);
        const char *keyStr = env->GetStringUTFChars(key, NULL);
        if (!keyStr) {  // Out of memory
            return;
        }
        const char *valueStr = env->GetStringUTFChars(value, NULL);
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
