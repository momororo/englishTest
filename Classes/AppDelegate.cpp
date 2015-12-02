#include "AppDelegate.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SaveSQL.h"
#include "TopScene.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
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
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    
    glview -> setDesignResolutionSize(1136, 640, ResolutionPolicy::SHOW_ALL);
    director->setContentScaleFactor(1.0f);


    register_all_packages();

    // create a scene. it's an autorelease object
    //auto scene = TitleScene::createScene();
    //auto scene = TopScene::createScene();


    //初回のみデータ読込
    if(!UserDefault::getInstance()->getBoolForKey("firstFlag")){
        
        
        SaveSQL::sqliteCreateTable();
        SaveSQL::sqliteSetValue();
        
        //二回以降は読み込まないようにフラグ操作
        UserDefault::getInstance()->setBoolForKey("firstFlag", true);
        UserDefault::getInstance()->setIntegerForKey("clearCount1", 0);
        UserDefault::getInstance()->setIntegerForKey("clearCount2", 0);
        UserDefault::getInstance()->setIntegerForKey("clearCount3", 0);
        
    }
    
    //デフォルト値の設定
    UserDefault::getInstance()->setIntegerForKey("stageSelect", 1);
    
    auto scene = TopScene::createScene();



    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
