LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := dodo-lib #이름은 빌드스크립트내(IAP Lib. 이름 포함)에서 겹치지 않도록만 지정하면 됩니다. 
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libdodo.so #실제 라이브러리 위치 및 이름 
include $(PREBUILT_SHARED_LIBRARY) 
 
include $(CLEAR_VARS) 
LOCAL_MODULE := ustoolkit-lib#이름은 빌드스크립트내(IAP Lib. 이름 포함)에서 겹치지 않도록만 지정하면 됩니다. 
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libUSToolkit.so #실제 라이브러리 위치 및 이름 
include $(PREBUILT_SHARED_LIBRARY) 


include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dlua_shared

LOCAL_MODULE_FILENAME := libcocos2dlua

CLASSES_DIRECTORY := $(LOCAL_PATH)/../../Classes

SOURCE_FILES := $(shell find $(CLASSES_DIRECTORY) -name *.cpp -o -name *.a)
SOURCE_FILES := $(sort $(SOURCE_FILES))
SOURCE_FILES := $(subst $(LOCAL_PATH)/,,$(SOURCE_FILES))

LOCAL_SRC_FILES := hellolua/main.cpp \
    $(SOURCE_FILES)

	#../../Classes/abc.cpp
LOCAL_CFLAGS    := -fviibility=hidden
LOCAL_CFLAGS := -DENABLE_OPENSL
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../boost_1_55_0

#LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../kamcord-android-sdk/kamcord/jni/
#LOCAL_HEADER_FILES := $(LOCAL_PATH)/../../kamcord-android-sdk/kamcord/jni/Kamcord-C-Interface.h
##LOCAL_HEADER_FILES += $(LOCAL_PATH)/../../5rocks-android-sdk/sdk/jni/include/FiveRocksC.h
#LOCAL_LDLIBS :=  -L$(call host-path, $(LOCAL_PATH)/../../kamcord-android-sdk/kamcord/libs/$(TARGET_ARCH_ABI)) -lkamcord


LOCAL_STATIC_LIBRARIES := curl_static_prebuilt
LOCAL_STATIC_LIBRARIES += fiverocks_static

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
#LOCAL_SHARED_LIBRARIES := fiverocks_shared
#LOCAL_SHARED_LIBRARIES += libkamcord
#LOCAL_SHARED_LIBRARIES += libkamcordcore
#LOCAL_HEADER_FILES := $(LOCAL_PATH)/Kamcord-C-Interface.h


include $(BUILD_SHARED_LIBRARY)



#include $(CLEAR_VARS)
#LOCAL_MODULE := libfiverocks.so
#LOCAL_SRC_FILES := ../../5rocks-android-sdk/sdk/jni/fiverocks_shared/$(TARGET_ARCH_ABI)/libfiverocks.so
#LOCAL_EXPORT_C_INCLUDES := ../../5rocks-android-sdk/sdk/jni/include
#LOCAL_EXPORT_CFLAGS     := -DFIVEROCKS_SHARED=1
#include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := libkamcord
#LOCAL_SRC_FILES := libkamcord.so
#include $(PREBUILT_SHARED_LIBRARY)


#include $(CLEAR_VARS)
#LOCAL_MODULE := libkamcordcore
#LOCAL_SRC_FILES := libkamcordcore.so
#include $(PREBUILT_SHARED_LIBRARY)


$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,scripting/lua/proj.android)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
#$(call import-module,fiverocks_shared)
$(call import-module,fiverocks_static)


