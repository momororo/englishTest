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
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("button26.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("button62.mp3");
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
    std::string  dbSelectName;

    switch (stage ->getIntegerForKey("selectStage")) {
        case 1:
            //ステージ１のゲーム背景
            bkFileName = "bk_1.png";
            dbSelectName = "number";
            break;
        case 2:
            //ステージ2のゲーム背景
            bkFileName = "bk_2.png";
            dbSelectName = "animal";
            break;
        case 3:
            //ステージ3のゲーム背景
            bkFileName = "bk_3.png";
            dbSelectName = "food";
            break;
            
        default:
            break;
    }
    
    //背景部
    Sprite *bk = Sprite::create(bkFileName);
    bk->setPosition(Vec2(selfFrame.width/2,selfFrame.height/2));
    bk->setName("bk");
    this->addChild(bk);

    //出題リストの抜き出し 10問なので10
    ListOfQuestions = SaveSQL::sqliteGetValueForKey(dbSelectName.c_str(), 10);
    //問題文の抜き出し 全部抜き出したいから適当に50
    questions =  SaveSQL::sqliteGetValueForKey(dbSelectName.c_str(),50);
    
    //画像のリストを読込
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("PlistOfQuestion.plist");
    
    //スタートの説明画像をのっける
    auto startImage = Sprite::create("startImage.png");
    startImage->setPosition(Vec2(selfFrame.width/2,selfFrame.height/2));
    startImage->setName("start");
    this->addChild(startImage);
    
    //BGM再生
    if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
        
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("gameScene.mp3",true);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);
        
    }
    
    return true;
}


//タッチ開始イベント
bool GameScene::onTouchBegan(Touch *pTouch, Event *pEvent)
{

    if(this->getChildByName("start") != nullptr){
        
        this->getChildByName("start")->removeFromParentAndCleanup(true);
        
        
        //問題文作成
        makeQuestionText();
        //選択肢作成
        makeChoiceText();
        
    }
    

    
    
    
    return true;
}

//タッチ終了イベント
void GameScene::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if(this->getChildByName("correct")){
        return;
    }

    //touchPoint : タッチポイントを取得
    Point touchPoint = Vec2(pTouch->getLocation());
    
    //タッチ判定
    for(int idx = 0;idx < choices->size();idx++){
        
        if(choices->at(idx)->getBoundingBox().containsPoint(touchPoint)){
            
            
            
            log("%s",choices->at(idx)->getName().c_str());
            log("%s",questionLabel->getName().c_str());
            
            //問題カウントアップ
            questionCount++;
            
            //正解か判定
            if(choices->at(idx)->getName() == questionLabel->getName()){
                
                //正解の処理
                log("正解");
                correctCount++;
                
                //正解時の効果音
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button26.mp3");
                
                auto correct = Sprite::create("correct.png");
                auto pos = choices->at(idx)->getPosition();
                correct->setPosition(pos);
                correct->setName("correct");
                this->addChild(correct);
                
                
                //フェードアウト
                auto action1 = FadeOut::create(1);
                //フェードアウト後
                auto callFunc1 = CallFunc::create([&](){
                    
                    if(questionCount == 10){
                        
                        log("10問終了");
                        //クイズの終了
                        makeEnd();

                    }else{
                        
                        
                        //次の問題文の作成
                        makeQuestionText();
                        makeChoiceText();
                        
                    }
                    
                });
                
                auto remove = RemoveSelf::create(true);
                
                //シークエンス作成
                auto seq = Sequence::create(action1, callFunc1,remove, NULL);
                
                correct->runAction(seq);
                
                
            }else{
                
                //不正解の処理
                log("不正解");
                
                //不正解時の効果音
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button62.mp3");
                
                //ループで正解以外の選択肢を黒色に染める
                for(int idxidx = 0; idxidx < choices->size(); idxidx++){
                    
                    if(choices->at(idxidx)->getName() != questionLabel->getName()){
                        choices->at(idxidx)->setColor(Color3B::BLACK);
                    }
                    
                    
                    auto action1 = FadeOut::create(1);
                    
                    CallFunc *callFunc1 = CallFunc::create([&](){});
                    
                    if(idxidx == 3){
                        
                        //フェードアウト後
                        callFunc1 = CallFunc::create([&](){
                            
                                
                            if(questionCount == 10){
                                
                                log("10問終了");
                                //クイズの終了
                                makeEnd();
                                
                            }else{
                                
                                //次の問題文の作成
                                makeQuestionText();
                                makeChoiceText();
                                
                            }

                            
                        });
                    }

                    
                    //シークエンス作成
                    auto seq = Sequence::create(action1, callFunc1, NULL);
                    
                    choices->at(idxidx)->runAction(seq);
                    
                    
                    
                    //不正解時もcorrectのSpriteを作り、操作不能の時間を作る。
                    auto correct = Sprite::create();
                    auto pos = choices->at(idx)->getPosition();
                    correct->setPosition(pos);
                    correct->setName("correct");
                    correct->setVisible("false");
                    this->addChild(correct);
                    //フェードアウト
                    auto correctAction = FadeOut::create(1);
                    auto correctRemove = RemoveSelf::create(true);
                    //シークエンス作成
                    auto correctSeq = Sequence::create(correctAction,correctRemove, NULL);
                    correct->runAction(correctSeq);



                    
                    
                }
                
            }
            
            
            

            
        }
        
    }

}

//タッチキャンセルイベント
void GameScene::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    CCLOG("タップキャンセル");
}

//問題文作成
void GameScene::makeQuestionText(){
    
    //問題文を削除
    if(questionLabel != nullptr){
        questionLabel->removeFromParentAndCleanup(true);
    }
    
    
    //リストから問題を選ぶ
    Question* question = ListOfQuestions->getRandomObject();
    
    
    //リストから選ばれた問題を除外する。
    ListOfQuestions->eraseObject(question);
    
    //問題を表示する(ここを画像に変えればよし)
    questionLabel = Label::createWithSystemFont(question->english,defaultFont,100);
    questionLabel->setPosition(selfFrame.width/2,selfFrame.height/2);
    questionLabel->setTextColor(Color4B::BLACK);
    
    //後で使うよ
    questionLabel->setName(question->english);
    
    this->addChild(questionLabel);
    
    
    

}

//選択肢作成
void GameScene::makeChoiceText(){
    
    //前回のお掃除
    for(int idx = 0 ; idx < choices->size(); idx++){
        choices->at(idx)->removeFromParentAndCleanup(true);
    }
    //配列のお掃除
    choices->clear();
    
    
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

            if(choice->english == choices->at(idx)->getName()){
                continueFlag = true;
                break;
            }
            
        }
        //再抽選
        if(continueFlag == true){continue;}
        
        auto st = StringUtils::format("question/%s.png",choice->english.c_str());
        
        //スプライトを作成して設置
        Sprite* sp = Sprite::createWithSpriteFrameName(st);
        sp->setName(choice->english);
        
        //答えを置く回はスプライトを置き換え
        if(count == answerPoint){
            sp = Sprite::createWithSpriteFrameName(StringUtils::format("question/%s.png",questionLabel->getName().c_str()));
            sp->setName(questionLabel->getName());
        }
        
        Vec2 pos;
        //回数に合わせて置く場所を決定する
        switch (count) {
            case 0:pos = Vec2(200, 150);break;
            case 1:pos = Vec2(936, 150);break;
            case 2:pos = Vec2(936, 470);break;
            case 3:pos = Vec2(200, 470);break;
            default:break;
        }
        
        sp->setPosition(pos);
        
        //addchild
        this->addChild(sp);
        
        //count足し込み
        count++;
        
        //choicesに追加
        choices->pushBack(sp);
        
        
    }
    

}

//正解時の処理
void GameScene::makeAnswer(){

}
void GameScene::makeEnd(){
    
    //お掃除
    if(questionLabel != nullptr){
        questionLabel->removeFromParentAndCleanup(true);
    }
    
    //お掃除
    for(int idx = 0 ; idx < choices->size(); idx++){
        choices->at(idx)->removeFromParentAndCleanup(true);
    }
    //配列のお掃除
    choices->clear();
    
    
    //スコアボードの作成
    Sprite *scoreBoard = Sprite::create("scoreBoard.png");
    scoreBoard -> setPosition(Vec2(selfFrame.width/2,selfFrame.height/2));
    this-> addChild(scoreBoard);
    
    //************* やりなおしボタン作成 ***************//
    auto retryBt = Sprite::create("retryBt.png");
    
    auto retryBtTaped = Sprite::create("retryBt.png");
    retryBtTaped -> setOpacity(150);
    
    //メニューアイテムの作成
    auto retryBtnItem = MenuItemSprite::create(retryBt, retryBtTaped, [](Ref *ref){
        
        //効果音
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button70.mp3");

        Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(1, GameScene::createScene()));
        
    });
    
    //メニューの作成　pMenuの中にpBtnItemを入れる
    auto retryMenu = Menu::create(retryBtnItem, NULL);
    
    //pMenuを画面中央に配置
    retryMenu->setPosition(Vec2(180, 60));
    retryMenu->setName("retry");
    scoreBoard -> addChild(retryMenu);
    
    //************* やりなおしボタン作成 (終) ***************//
    
    //************* もどるボタン作成 ********************//
    auto exitBt = Sprite::create("exitBt.png");
    
    auto exitBtTaped = Sprite::create("exitBt.png");
    exitBtTaped -> setOpacity(150);
    
    //メニューアイテムの作成
    auto exitBtnItem = MenuItemSprite::create(exitBt, exitBtTaped, [](Ref *ref){
        
        //効果音
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button70.mp3");
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        
        Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(1, TitleScene::createScene()));
        
    });
    
    //メニューの作成　pMenuの中にpBtnItemを入れる
    auto exitMenu = Menu::create(exitBtnItem, NULL);
    
    //pMenuを画面中央に配置
    exitMenu->setPosition(Vec2(480, 60));
    exitMenu->setName("exit");
    scoreBoard -> addChild(exitMenu);
    
    //************* もどるボタン作成 (終) ********************//
    
    //正解数表示
    std::string scoreStr = StringUtils::format("%d",correctCount);
    Label *scoreLabel = Label::createWithSystemFont(scoreStr, funwariFont, 220);
    scoreLabel -> setPosition(Vec2(180,220));
    scoreLabel -> setColor(Color3B(255,85,85));
    scoreBoard -> addChild(scoreLabel);
    
    
    //クリアしている場合はクリア回数に追加
    if(correctCount == 10){

        auto userDef = UserDefault::getInstance();
        auto clearCount = userDef->getIntegerForKey(StringUtils::format("clearCount%d",userDef->getIntegerForKey("selectStage")).c_str());
        
        clearCount++;
 
        userDef->setIntegerForKey(StringUtils::format("clearCount%d",userDef->getIntegerForKey("selectStage")).c_str(),clearCount);
 
    }
    
}
