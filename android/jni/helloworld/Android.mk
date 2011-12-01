LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := game

LOCAL_SRC_FILES := main.cpp \
../../../Classes/Singletons/AppDelegate.cpp \
../../../Classes/HelloWorldScene.cpp \
../../../Classes/Layers/GameplayLayer.cpp \
../../../Classes/Layers/BackgroundLayer.cpp \
../../../Classes/Scenes/GameScene.cpp \
../../../Classes/Constants/StringSplit.cpp \
../../../Classes/EnemyObjects/RadarDish.cpp \
../../../Classes/GameObjects/GameCharacter.cpp \
../../../Classes/GameObjects/GameObject.cpp \
../../../Classes/GameObjects/Viking.cpp \
../../../Classes/JoystickClasses/SneakyButton.cpp \
../../../Classes/JoystickClasses/SneakyJoystick.cpp \
../../../Classes/JoystickClasses/SneakyButtonSkinnedBase.cpp \
../../../Classes/JoystickClasses/SneakyJoystickSkinnedBase.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../cocos2dx \
                    $(LOCAL_PATH)/../../../../cocos2dx/platform \
                    $(LOCAL_PATH)/../../../../cocos2dx/include \
                    $(LOCAL_PATH)/../../../../CocosDenshion/include \
                    $(LOCAL_PATH)/../../../Classes 

# it is used for ndk-r5  
# if you build with ndk-r4, comment it  
# because the new Windows toolchain doesn't support Cygwin's drive
# mapping (i.e /cygdrive/c/ instead of C:/)  
LOCAL_LDLIBS := -L$(call host-path, $(LOCAL_PATH)/../../libs/armeabi) \
                -lcocos2d -llog -lcocosdenshion \
                -L$(call host-path, $(LOCAL_PATH)/../../../../cocos2dx/platform/third_party/android/libraries) -lcurl
            
include $(BUILD_SHARED_LIBRARY)
