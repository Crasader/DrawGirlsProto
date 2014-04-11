LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dlua_shared

LOCAL_MODULE_FILENAME := libcocos2dlua

CLASSES_DIRECTORY := $(LOCAL_PATH)/../../Classes

SOURCE_FILES := $(shell find $(CLASSES_DIRECTORY) -name *.cpp -o -name *.a)
SOURCE_FILES := $(sort $(SOURCE_FILES))
SOURCE_FILES := $(subst $(LOCAL_PATH)/,,$(SOURCE_FILES))

LOCAL_SRC_FILES := hellolua/main.cpp \
    $(SOURCE_FILES)
	
LOCAL_CFLAGS    := -fvisibility=hidden
LOCAL_CFLAGS := -DENABLE_OPENSL
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../boost_1_55_0

LOCAL_STATIC_LIBRARIES := curl_static_prebuilt

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,scripting/lua/proj.android)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
