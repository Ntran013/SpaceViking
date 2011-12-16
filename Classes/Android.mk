LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := game_logic

LOCAL_SRC_FILES := Singletons/AppDelegate.cpp \
Singletons/GameManager.cpp \
Scenes/Scene1/Layers/GameplayLayer.cpp \
Scenes/Scene1/Layers/BackgroundLayer.cpp \
Scenes/Scene1/GameScene.cpp \
Constants/StringSplit.cpp \
EnemyObjects/RadarDish.cpp \
GameObjects/GameCharacter.cpp \
GameObjects/GameObject.cpp \
GameObjects/Viking.cpp \
GameObjects/SpaceCargoShip.cpp \
EnemyObjects/EnemyRobot.cpp \
EnemyObjects/PhaserBullet.cpp \
PowerUps/Health.cpp \
PowerUps/Mallet.cpp \
Scenes/Intro/IntroLayer.cpp \
Scenes/Intro/IntroScene.cpp \
Scenes/LevelComplete/LevelCompleteLayer.cpp \
Scenes/LevelComplete/LevelCompleteScene.cpp \
Scenes/MainMenu/MainMenuLayer.cpp \
Scenes/MainMenu/MainMenuScene.cpp \
Scenes/Options/OptionsLayer.cpp \
Scenes/Options/OptionsScene.cpp \
Scenes/Scene2/GameControlLayer.cpp \
Scenes/Scene2/GameplayScrollingLayer.cpp \
Scenes/Scene2/GameScene2.cpp \
Scenes/Scene2/StaticBackgroundLayer.cpp \
Scenes/CutSceneForLevel2/PlatformScene.cpp \
Scenes/CutSceneForLevel2/PlatformScrollingLayer.cpp \
JoystickClasses/SneakyButton.cpp \
JoystickClasses/SneakyJoystick.cpp \
JoystickClasses/SneakyButtonSkinnedBase.cpp \
JoystickClasses/SneakyJoystickSkinnedBase.cpp \
eplibs/EPGestureRecognizerLayer.cpp \
eplibs/EPNotificationCenter.cpp \
eplibs/EPResourceManager.cpp \
eplibs/EPScrollListLayer.cpp \
eplibs/EPSpriteButton.cpp \
eplibs/GestureRecognizer/GestureRecognizer.cpp \
eplibs/GestureRecognizer/LongPressGestureRecognizer.cpp \
eplibs/GestureRecognizer/PanGestureRecognizer.cpp \
eplibs/GestureRecognizer/PinchGestureRecognizer.cpp \
eplibs/GestureRecognizer/RotationGestureRecognizer.cpp \
eplibs/GestureRecognizer/TapGestureRecognizer.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../cocos2dx \
                    $(LOCAL_PATH)/../../cocos2dx/platform \
                    $(LOCAL_PATH)/../../cocos2dx/include \
                    $(LOCAL_PATH)/../../CocosDenshion/include \
                    $(LOCAL_PATH)/../../cocos2dx/lua_support 

LOCAL_LDLIBS := -L$(call host-path, $(LOCAL_PATH)/../android/libs/$(TARGET_ARCH_ABI)) \
                -lcocos2d -lcocosdenshion \
				-lGLESv1_CM \
                -L$(call host-path, $(LOCAL_PATH)/../../cocos2dx/platform/third_party/android/libraries/$(TARGET_ARCH_ABI)) -lcurl
            
include $(BUILD_SHARED_LIBRARY)