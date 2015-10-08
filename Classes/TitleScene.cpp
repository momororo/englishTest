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

    
    //背景の生成
    Sprite *bkWhite = Sprite::create();//下地
    bkWhite->setTextureRect(Rect(0,0,640,1136));
    bkWhite->setPosition(Vec2(selfFrame.width/2,selfFrame.height/2));
    bkWhite->setColor(Color3B::WHITE);//ノート
    this->addChild(bkWhite);
    Sprite *bk = Sprite::create(defaultBk);
    bk->setPosition(Vec2(selfFrame.width/2,selfFrame.height/2));
    bkWhite->addChild(bk);
    
    //トレーニングボタンの作成
    auto *trainingButton = createTrainingButton();
    trainingButton->setPosition(Vec2(selfFrame.width/2,selfFrame.height/11*3.5));
    this->addChild(trainingButton);
    
    //復習ボタンの作成
    auto *reviewButton = createReviewButton();
    reviewButton->setPosition(Vec2(selfFrame.width/2,selfFrame.height/11*2));
    this->addChild(reviewButton);
    
    //cocostudioのタイトルシーン読み込み
    auto mainScene = CSLoader::getInstance()->createNode("GameScene.csb");
    mainScene -> setName("mainScene");
    this -> addChild(mainScene);
    
    //cocostudioで設定したフレームの読み込み→gotoFrameAndPlay(読み込むフレーム位置、ループの可否)
    auto action = CSLoader::getInstance()->createTimeline("GameScene.csb");
    mainScene -> runAction(action);
    action -> gotoFrameAndPlay(0, true);
    
    //cocostudioのbutton設定
    auto button = mainScene -> getChildByName("background") -> getChildByName<ui::Button*>("Button_1");
    
    button -> addTouchEventListener([this](Ref* pSender,ui::Widget::TouchEventType type){
    
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                CCLOG("タッチビギャン確認！");
                break;
                
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                CCLOG("タッチエンド確認！");
                break;
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                CCLOG("タッチム〜ブ確認！");
                break;
                
            default:
                break;
        }
    });
    
    return true;
}


//トレーニングボタンの生成
Menu* TitleScene::createTrainingButton(){
  
//トレーニングボタンの生成(後ほど画像に差し替えたい)
    //枠組みの生成
    Sprite *trainingButton = Sprite::create();
    trainingButton->setTextureRect(Rect(0,0,300,100));
    trainingButton->setColor(Color3B::BLACK);
    
    //テキストの生成
    Label *trainingLabel = Label::createWithSystemFont("トレーニング",defaultFont,30);
    trainingLabel->setTextColor(Color4B::WHITE);
    trainingLabel->setPosition(Vec2(trainingButton->getContentSize().width/2,trainingButton->getContentSize().height/2));
    
    //add
    trainingButton->addChild(trainingLabel);
    
    //枠組みの生成(押下後)
    Sprite *trainingButtonTp = Sprite::create();
    trainingButtonTp->setTextureRect(Rect(0,0,300,100));
    trainingButtonTp->setColor(Color3B::BLACK);
    
    //テキストの生成(押下後)
    Label *trainingLabelTp = Label::createWithSystemFont("トレーニング",defaultFont,30);
    trainingLabelTp->setTextColor(Color4B::WHITE);
    trainingLabelTp->setPosition(Vec2(trainingButtonTp->getContentSize().width/2,trainingButtonTp->getContentSize().height/2));
    
    //add
    trainingButtonTp->addChild(trainingLabelTp);
    //オパシティ調整
    trainingButtonTp->setOpacity(150);

    //メニューアイテムの作成
    auto pBtnItem = MenuItemSprite::create(trainingButton, trainingButtonTp, [](Ref *ref){
        //ボタン効果音再生
        SimpleAudioEngine::getInstance()->playEffect("button70.mp3");
        Director::getInstance()->replaceScene(TransitionPageTurn::create(1, LoadScene::createScene("GameScene"), 0));
        
    });
    
    //メニューの作成　pMenuの中にpBtnItemを入れる
    auto returnButton = Menu::create(pBtnItem, NULL);
    
    return returnButton;
}

//復習チェックボタンの生成
Menu* TitleScene::createReviewButton(){
    
//復習チェックの生成(後ほど画像に差し替えたい)
    //枠組みの生成
    Sprite *reviewButton = Sprite::create();
    reviewButton->setTextureRect(Rect(0,0,300,100));
    reviewButton->setColor(Color3B::BLACK);
    
    //テキストの生成
    Label *reviewLabel = Label::createWithSystemFont("復習チェック10",defaultFont,30);
    reviewLabel->setTextColor(Color4B::WHITE);
    reviewLabel->setPosition(Vec2(reviewButton->getContentSize().width/2,reviewButton->getContentSize().height/2));
    
    //add
    reviewButton->addChild(reviewLabel);
    
    //枠組みの生成(押下後)
    Sprite *reviewButtonTp = Sprite::create();
    reviewButtonTp->setTextureRect(Rect(0,0,300,100));
    reviewButtonTp->setColor(Color3B::BLACK);
    
    //テキストの生成(押下後)
    //Label *reviewLabelTp = Label::createWithSystemFont("復習チェック10",defaultFont,30);
    Label *reviewLabelTp = Label::createWithSystemFont("復習チェック10", defaultFont, 30);

    reviewLabelTp->setTextColor(Color4B::WHITE);
    reviewLabelTp->setPosition(Vec2(reviewButtonTp->getContentSize().width/2,reviewButtonTp->getContentSize().height/2));
    
    //add
    reviewButtonTp->addChild(reviewLabelTp);
    //オパシティ調整
    reviewButtonTp->setOpacity(150);
    
    //メニューアイテムの作成
    auto pBtnItem = MenuItemSprite::create(reviewButton, reviewButtonTp, [](Ref *ref){
        //ボタン効果音再生
        SimpleAudioEngine::getInstance()->playEffect("button70.mp3");
        //  Director::getInstance()->replaceScene(TransitionPageTurn::create(1, StorySelect::createScene(), 0));
        
    });
    
    //メニューの作成　pMenuの中にpBtnItemを入れる
    auto returnButton = Menu::create(pBtnItem, NULL);
    
    return returnButton;
    
}