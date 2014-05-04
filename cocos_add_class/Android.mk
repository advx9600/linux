LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := BanQiuCPP_shared

LOCAL_MODULE_FILENAME := libBanQiuCPP

LOCAL_SRC_FILES := BanQiuCPP/main.cpp \
../../Classes/Two.cpp \
../../Classes/One.cpp \
                   BanQiuCPP/BallTest.cpp \
                   BanQiuCPP/FielderSprite.cpp \
                   BanQiuCPP/NewGameLayer.cpp \
                   BanQiuCPP/WinLayer.cpp \
                   BanQiuCPP/LoseLayer.cpp \
                   BanQiuCPP/BeginLayer.cpp \
                   BanQiuCPP/HelpLayer.cpp \
                   BanQiuCPP/AchieveLayer.cpp \
                   BanQiuCPP/Prop.cpp \
                   BanQiuCPP/DialogLayer.cpp \
                   BanQiuCPP/OutLayer.cpp \
                   BanQiuCPP/SetLayer.cpp \
                   BanQiuCPP/OpenLayer.cpp
                                                             
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)

