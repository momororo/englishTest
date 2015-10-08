//
//  TitleScene.cpp
//  boki-3
//
//  Created by yasutomo on 2015/06/08.
//
//

#include "TitleScene.h"
#include "Config.h"
#include "LoadScene.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocostudio;
using namespace timeline;

Scene* TitleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TitleScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //ボタン効果音
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("button70.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.4f);
    
    //cocostudioのタイトルシーン読み込み
    auto mainScene = CSLoader::getInstance()->createNode("GameScene.csb");
    mainScene -> setName("mainScene");
    this -> addChild(mainScene);
    
    //cocostudioで設定したフレームの読み込み→gotoFrameAndPlay(読み込むフレーム位置、ループの可否)
    auto action = CSLoader::getInstance()->createTimeline("GameScene.csb");
    mainScene -> runAction(action);
    action -> gotoFrameAndPlay(0, true);
    
    //ステージセレクトボタンに動作を設定
    makeStageButton();
    
    //矢印画面をステージに
    
    return true;
}


//ステージセレクトボタンの動作を設定
void TitleScene::makeStageButton(){
    //1面から3面
    for(int idx = 1 ; 1 <= 3; idx++ ){
        
        
        auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene") -> getChildByName("background")->getChildByName(StringUtils::format("Button_%d",idx)));
        
        //動作設定
        button -> addTouchEventListener([&,idx](Ref* pSender,ui::Widget::TouchEventType type){
            
            //タップ終わり時、ゲーム画面へ遷移
            if(type == cocos2d::ui::Widget::TouchEventType::ENDED){
                
                //ユーザーデフォルトにステージ数を記録
                UserDefault::getInstance()->setIntegerForKey("selectStage", idx);
                
                //ゲーム画面に遷移
            }
            
        });
    }
    
    
}

void TitleScene::moveStage(int stageNumber){
    
    //キャラクターを移動させる。
    
    
    
}