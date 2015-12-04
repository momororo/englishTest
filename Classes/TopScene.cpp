//
//  TopScene.cpp
//  EnglishTestVer2
//
//  Created by 新井脩司 on 2015/11/19.
//
//

#include "TopScene.h"
#include "Config.h"
#include "LoadScene.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "TitleScene.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocostudio;
using namespace timeline;

Scene* TopScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TopScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TopScene::init()
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
    
    //背景の生成
    Sprite *bkWhite = Sprite::create();//下地
    bkWhite->setTextureRect(Rect(0,0,640,1136));
    bkWhite->setPosition(Vec2(selfFrame.width/2,selfFrame.height/2));
    bkWhite->setColor(Color3B::WHITE);//ノート
    this->addChild(bkWhite);
    Sprite *bk = Sprite::create(defaultBk);
    bk->setPosition(Vec2(selfFrame.width/2,selfFrame.height/2));
    bkWhite->addChild(bk);
    
    //cocostudioのタイトルシーン読み込み
    auto mainScene = CSLoader::getInstance()->createNode("TitleScene.csb");
    mainScene -> setName("mainScene");
    this -> addChild(mainScene);
        
    //ステージセレクトボタンに動作を設定
    makeStageButton();
    
    
    return true;
}


//ステージセレクトボタンの動作を設定
void TopScene::makeStageButton(){
    //1面から3面
    
    for(int idx = 1 ; idx <= 3; idx++){
        
        
        auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName(StringUtils::format("Button_%d",idx)));
        
        /*
        if(idx != 1){
            button -> setTouchEnabled(false);
        }
        */
        
        //動作設定
        button -> addTouchEventListener([&,idx](Ref* pSender,ui::Widget::TouchEventType type){
            
            //タップ終わり時、ゲーム画面へ遷移
            if(type == cocos2d::ui::Widget::TouchEventType::ENDED){
                
                switch (idx) {
                    case 1:
                        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, TitleScene::createScene(), Color3B::BLACK));
                        break;
                    case 2:
                        //遊び方の実装
                        makeTutorial();
                    
                        break;
                    case 3:
                        //記録の設定
                        break;
                    default:
                        break;
                }
                
                //ゲーム画面に遷移
            }
            
        });
    }
    
    
}

void TopScene::makeTutorial(){
    
    //説明文の画像の可視化
    auto tutorial = dynamic_cast<Sprite*>(this->getChildByName("mainScene") -> getChildByName("tutorial"));
    tutorial -> setVisible(true);
    
    //説明文の閉じるボタンの可視化・操作可能
    auto closeBt = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName(StringUtils::format("Button_4")));

    closeBt -> setVisible(true);
    closeBt -> setTouchEnabled(true);
    
    //他のボタンを押せないように設定
    for (int idx = 1; idx <= 3; idx++) {
        
        auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName(StringUtils::format("Button_%d",idx)));
        
        button -> setTouchEnabled(false);
        
    }
    
    //閉じるボタンを押した時の動作
    closeBt -> addTouchEventListener([&](Ref* pSender,ui::Widget::TouchEventType type){
        
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            
            //説明画像とボタンのスプライト作成（上記のスプライトを引用したら落ちました）
            auto turorialFalse = dynamic_cast<Sprite*>(this->getChildByName("mainScene") -> getChildByName("tutorial"));
            
            auto closeBtFalse = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName(StringUtils::format("Button_4")));
            
            //説明画像とボタンの不可視化・操作不可能
            turorialFalse -> setVisible(false);
            closeBtFalse->setVisible(false);
            closeBtFalse->setTouchEnabled(false);
            
            //その他ボタンの動作を可能に
            for (int idx = 1; idx <= 3; idx++) {
                
                auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName(StringUtils::format("Button_%d",idx)));
                
                button -> setTouchEnabled(true);
                
            }
            
        }
        
    
    });

    
    
}