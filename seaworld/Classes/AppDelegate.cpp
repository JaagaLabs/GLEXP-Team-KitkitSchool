#include "AppDelegate.h"

#include "Games/FishBowl/FishBowlScene.hpp"
#ifdef TEST_MODE
    #include "MainScene.hpp"
#endif

//#include "CCAppController.hpp"
// #define USE_AUDIO_ENGINE 1
#define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

using namespace std;

static cocos2d::Size designResolutionSize = cocos2d::Size(2560, 1800);
static cocos2d::Size desktopSize = cocos2d::Size(2560, 1800)/2; // pixel-c

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("KitKit_FB", cocos2d::Rect(0, 0, desktopSize.width, desktopSize.height));
#else
        glview = GLViewImpl::create("KitKit_FB");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);
    
    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);

    register_all_packages();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    string path = FileUtils::getInstance()->getWritablePath() + "KitkitSchoolSample";

    if (FileUtils::getInstance()->isFileExist(path+"/location.txt")) {
        path = FileUtils::getInstance()->getStringFromFile(path+"/location.txt");
        path.erase(std::remove(path.begin(),path.end(),'\n'),path.end());
    }

    FileUtils::getInstance()->setDefaultResourceRootPath(path);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string devicePath = "/sdcard/KitkitSchool/";
    if (FileUtils::getInstance()->isFileExist(devicePath + "resourceSync.py")) {
        FileUtils::getInstance()->setDefaultResourceRootPath(devicePath);
    }
    
    
#endif

#ifdef TEST_MODE
    Scene *scene = MainScene::createScene();
#else
    Scene *scene = FishBowlScene::createScene(0);
#endif
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    __NotificationCenter::getInstance()->postNotification("GET_FISH_DATA");
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
