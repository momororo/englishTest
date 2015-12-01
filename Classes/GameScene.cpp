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
    //titleSceneで設定したステージ数を、getIntegerforkeyで取り出し、背景を設定する
    auto stage = UserDefault::getInstance();
    //背景のファイルネームをUserDefaultから取得
    std::string  bkFileName;
    switch (stage ->getIntegerForKey("selectStage")) {
        case 1:
            //ステージ１のゲーム背景
            bkFileName = "bk_1.png";
            break;
        case 2:
            //ステージ2のゲーム背景
            bkFileName = "bk_2.png";
            break;
        case 3:
            //ステージ3のゲーム背景
            bkFileName = "bk_3.png";
            break;
            
        default:
            break;
    }
    
    //背景部
    Sprite *bk = Sprite::create(bkFileName);
    bk->setPosition(Vec2(selfFrame.width/2,selfFrame.height/2));
    bk->setName("bk");
    this->addChild(bk);

    
    //問題文の抜き出し
    questions =  SaveSQL::sqliteGetValueForKey("number");
    
    if( questions->size() == 0){
        log("取得しっぱい");
        return true;
    }
    
    for(int idx = 0 ; idx < questions->size() ; idx++){
        log("%s",questions->at(idx)->english.c_str());
    }

    
    
    //出題リストの作成(questions)のコピー
    ListOfQuestions =  SaveSQL::sqliteGetValueForKey("number");
    
    if(ListOfQuestions ->size() == 0){
        log("取得しっぱい");
        return true;
    }
    
    for(int idx = 0 ; idx < ListOfQuestions->size() ; idx++){
        log("%s",ListOfQuestions->at(idx)->english.c_str());
    }
    
    
  
    //問題文作成
    makeQuestionText();
    //選択肢作成
    makeChoiceText();

    return true;
}


//タッチ開始イベント
bool GameScene::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    
    makeQuestionText();
    makeChoiceText();
    
    
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
    
    //問題文を削除
    
    this->removeChildByTag(100);
    
    //リストから問題を選ぶ
    Question* question = ListOfQuestions->getRandomObject();
    
    
    //リストから選ばれた問題を除外する。
    ListOfQuestions->eraseObject(question);
    
    
    //問題を表示する(ここを画像に変えればよし)
    questionLabel = Label::createWithSystemFont(question->english,defaultFont,30);
    questionLabel->setPosition(selfFrame.width/2,selfFrame.height/2);
    questionLabel->setTextColor(Color4B::BLACK);
    
    //後で使うよ
    questionLabel->setName(question->english);
    //後で使うよ
    questionLabel->setTag(100);
    
    this->addChild(questionLabel);
    
    

}

//選択肢作成
void GameScene::makeChoiceText(){
    
    //前回のお掃除
    for(int idx = 0 ; idx < 4; idx++){
        this->removeChildByTag(200);
    }
    
    //選択肢の配列作っちゃう
//    Vector<Sprite*> *choices = new Vector<Sprite*>;
    Vector<Label*> *choices = new Vector<Label*>;

    
    
    //答えを置く場所を決める
    int answerPoint = arc4random_uniform(4);
    
    //問題文と既存の選択肢が重複しないように選択肢を作成していく。
    for(int count = 0; count < 4;){
        
        
        //リストから選択肢を取る。
        Question* choice = questions->getRandomObject();
        
        
        //問題文と被っていれば再抽選
        if(choice->english == questionLabel->getName()){
            continue;
        }
        
        
        bool continueFlag = false;
        
        //既存の選択肢と被っていれば再抽選のフラグへ
        for(int idx = 0 ; idx < count ; idx++){

            if(choice->english == choices->at(choices->size()-1)->getName()){
                continueFlag = true;
                continue;
            }
            
        }
        //再抽選
        if(continueFlag == true){continue;}
        
        //スプライトを作成して設置
//        Sprite* sp = Sprite::create(choice->english);
        Label* sp = Label::createWithSystemFont(choice->english,defaultFont,30);

        sp->setName(choice->english);
        
        //答えを置く回はスプライトを置き換え
        if(count == answerPoint){
//            sp = Sprite::create(questionLabel->getName());
            sp = Label::createWithSystemFont(questionLabel->getName(),defaultFont,30);
            sp->setName(questionLabel->getName());
        }
        
        sp->setTextColor(Color4B::BLACK);
        
        Vec2 pos;
        //回数に合わせて置く場所を決定する
        switch (count) {
            case 0:pos = Vec2(300, 100);break;
            case 1:pos = Vec2(300, 200);break;
            case 2:pos = Vec2(300, 300);break;
            case 3:pos = Vec2(300, 400);break;
            default:break;
        }
        
        sp->setPosition(pos);
        
        //addchild
        this->addChild(sp);
        //削除用
        sp->setTag(200);
        
        //count足し込み
        count++;
        
        //choicesに追加
        choices->pushBack(sp);
        
        
    }
    

}

//正解時の処理
void GameScene::makeAnswer(){

}
