//
//  GameScene.cpp
//  boki-3
//
//  Created by yasutomo on 2015/06/08.
//
//

#include "GameScene.h"
#include "TitleScene.h"
#include "LoadScene.h"
#include "sqlite3.h"
#include "SaveSQL.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool GameScene::init()
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
    
    
    //リスナー登録
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchCancelled =
    CC_CALLBACK_2(GameScene::onTouchCancelled, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority
    (listener, this);
    
    //Update
    this->scheduleUpdate();
    
//背景の生成
    //下地
    Sprite *bkWhite = Sprite::create();//下地
    bkWhite->setTextureRect(Rect(0,0,640,1136));
    bkWhite->setPosition(Vec2(selfFrame.width/2,selfFrame.height/2));
    bkWhite->setColor(Color3B::WHITE);//ノート
    this->addChild(bkWhite);
    //背景部
    Sprite *bk = Sprite::create(defaultBk);
    bk->setPosition(Vec2(selfFrame.width/2,selfFrame.height/2));
    bk->setName("bk");
    
    this->addChild(bk);

    
    
    questions =  SaveSQL::sqliteGetValueForKey("number");
    
    if( questions->size() == 0){
        log("取得しっぱい");
        return true;
    }
/*
    for(int idx = 0 ; idx < questions->size(); idx++){
        log("no = %d、 questionText=%s、choiceText=%s,answertTag=%d,answertText=%s,commentary=%s,correctCount=%d、 incorrectCount=%d、reviewFlag=%d\n",questions->at(idx)->questionId,questions->at(idx)->questionText.c_str(),questions->at(idx)->choiceText.c_str(),questions->at(idx)->answerTag,questions->at(idx)->answerText.c_str(),questions->at(idx)->commentary.c_str(), questions->at(idx)->correctCount,questions->at(idx)->incorrectCount,questions->at(idx)->reviewFlag);
    };
*/
    
/*文章を生成する処理
    
    //1行を"¥n"を区切りにして分割する
    vector<string> params = split(questions->at(0)->commentary.c_str(), "¥n");
    //ラベルに文字を入れていく
    for (int idx = 0; idx < params.size(); idx++){
        auto label = Label::createWithTTF(params.at(idx), defaultFont, 30);
        label -> setTextColor(Color4B::BLACK);
        label -> setAnchorPoint(Vec2(0.0f, 0.5f));
        if(commentaryLabels->size() == 0){
            label->setPosition(Vec2(selfFrame.width/10,selfFrame.height/5*3));
        }else{
            label->setPosition(Vec2(selfFrame.width/10,commentaryLabels->at(commentaryLabels->size()-1)->getPositionY()-commentaryLabels->at(commentaryLabels->size()-1)->getContentSize().height/2 - label->getContentSize().height/2));
            
        }
        bk->addChild(label);
        commentaryLabels->pushBack(label);

    }
*/
  
    //問題文作成
    makeQuestionText();
    //選択肢作成
    makeChoiceText();
    

    return true;
}


//タッチ開始イベント
bool GameScene::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    
    return true;
}

//タッチ終了イベント
void GameScene::onTouchEnded(Touch *pTouch, Event *pEvent)
{

    //touchPoint : タッチポイントを取得
    Point touchPoint = Vec2(pTouch->getLocation());


}

//タッチキャンセルイベント
void GameScene::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    CCLOG("タップキャンセル");
}

//問題文作成
void GameScene::makeQuestionText(){
    
    questions = SaveSQL::sqliteGetValueForKey("number");
    
    Label *test1 = Label::createWithSystemFont(questions->at(0)->japanese,defaultFont,30);
    test1->setPosition(selfFrame.width/2,selfFrame.height/2);
    test1->setTextColor(Color4B::BLACK);
    this->addChild(test1);
    
    Label *test2 = Label::createWithSystemFont(questions->at(0)->english,defaultFont,30);
    test2->setPosition(selfFrame.width/2,selfFrame.height/3*2);
    test2->setTextColor(Color4B::BLACK);
    this->addChild(test2);
    
    
    
    

}

//選択肢作成
void GameScene::makeChoiceText(){

}

//正解時の処理
void GameScene::makeAnswer(){

}
