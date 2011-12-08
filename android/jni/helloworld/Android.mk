LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := game

LOCAL_SRC_FILES := main.cpp \
../../../Classes/Singletons/AppDelegate.cpp \
../../../Classes/Singletons/GameManager.cpp \
../../../Classes/HelloWorldScene.cpp \
../../../Classes/Scenes/Scene1/Layers/GameplayLayer.cpp \
../../../Classes/Scenes/Scene1/Layers/BackgroundLayer.cpp \
../../../Classes/Scenes/GameScene.cpp \
../../../Classes/Constants/StringSplit.cpp \
../../../Classes/EnemyObjects/RadarDish.cpp \
../../../Classes/GameObjects/GameCharacter.cpp \
../../../Classes/GameObjects/GameObject.cpp \
../../../Classes/GameObjects/Viking.cpp \
../../../Classes/GameObjects/SpaceCargoShip.cpp \
../../../Classes/EnemyObjects/EnemyRobot.cpp \
../../../Classes/EnemyObjects/PhaserBullet.cpp \
../../../Classes/PowerUps/Health.cpp \
../../../Classes/PowerUps/Mallet.cpp \
../../../Classes/Scenes/Intro/IntroLayer.cpp \
../../../Classes/Scenes/Intro/IntroScene.cpp \
../../../Classes/Scenes/LevelComplete/LevelCompleteLayer.cpp \
../../../Classes/Scenes/LevelComplete/LevelCompleteScene.cpp \
../../../Classes/Scenes/MainMenu/MainMenuLayer.cpp \
../../../Classes/Scenes/MainMenu/MainMenuScene.cpp \
../../../Classes/Scenes/Options/OptionsLayer.cpp \
../../../Classes/Scenes/Options/OptionsScene.cpp \
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
