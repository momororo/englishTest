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
#include "GameScene.h"

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

/***********主人公の設定****************/
    
    //キャラクターを表示させる。
    auto piyo = Sprite::create("piyo_1.png");
    piyo -> setName("piyo");
    
    //場所を設定
    piyo ->setPosition(mainScene->getChildByName("background")->getChildByName("Button_1")->getPosition());
    
    this->addChild(piyo);

/***********主人公の設定終****************/
    


    //矢印の設定(初期設定)
    auto allow = dynamic_cast<ui::Button*>(mainScene->getChildByName("background")->getChildByName("gostage2"));
    allow -> setVisible(true);
    allow -> setTouchEnabled(true);
    
    //矢印の動作設定
    makeMoveButton();
    
    return true;
}


//ステージセレクトボタンの動作を設定
void TitleScene::makeStageButton(){
    //1面から3面
    for(int idx = 1 ; idx <= 3; idx++ ){
        
        
        auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene") -> getChildByName("background")->getChildByName(StringUtils::format("Button_%d",idx)));
        
        if(idx != 1){
            button -> setTouchEnabled(false);
        }
        
        //動作設定
        button -> addTouchEventListener([&,idx](Ref* pSender,ui::Widget::TouchEventType type){
            
            //タップ終わり時、ゲーム画面へ遷移
            if(type == cocos2d::ui::Widget::TouchEventType::ENDED){
                
                //ユーザーデフォルトにステージ数を記録
                UserDefault::getInstance()->setIntegerForKey("selectStage", idx);
                
                //ゲーム画面に遷移
                Director::getInstance()->replaceScene(TransitionPageTurn::create(1, GameScene::createScene(), 0));

            }
            
        });
    }
    
    
}

void TitleScene::makeMoveButton(){

    
/*********goStage2*************/
    //goStage2の動作設定
    auto goSt2 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("gostage2"));
    //動作設定
    goSt2 -> addTouchEventListener([&](Ref* pSender,ui::Widget::TouchEventType type){
        
        //タップ終わり時、動作開始
        if(type == cocos2d::ui::Widget::TouchEventType::ENDED){

            auto goSt2 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("gostage2"));
            
            
            //矢印を非表示&タップ不可に
            goSt2 -> setVisible(false);
            goSt2 -> setTouchEnabled(false);
            
            //ボタンを押せないように
            auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene") -> getChildByName("background")->getChildByName("Button_1"));
            button -> setTouchEnabled(false);

            
            
            auto bk = this ->getChildByName("mainScene")->getChildByName("background");
            

            //ピヨを歩行モードに
            this -> walkPiyoToRight();
            
            //画面移動
            auto action1 = MoveBy::create(4.0f,Point(-selfFrame.width/2,0));


            
            //移動後の処理
            auto action2 = CallFuncN::create([&](Ref *sender){
                

                this -> stopPiyo();
                
                //後で書こうね
                
                //ステージ2の矢印を表示
                auto goSt3 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("gostage3"));
                goSt3 -> setVisible(true);
                goSt3 -> setTouchEnabled(true);
                auto backSt1 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("backstage1"));
                backSt1 -> setVisible(true);
                backSt1 -> setTouchEnabled(true);
                
                //ボタンを押せるように
                auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene") -> getChildByName("background")->getChildByName("Button_2"));
                button -> setTouchEnabled(true);


                
            });
            
            
        
            auto seq = Sequence::create(action1, action2, NULL);
            bk->runAction(seq);
            
        }
        
    });

/*********goStage2終*************/

    
/*********goStage3*************/
    //goStage3の動作設定
    auto goSt3 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("gostage3"));
    //動作設定
    goSt3 -> addTouchEventListener([&](Ref* pSender,ui::Widget::TouchEventType type){
        
        //タップ終わり時、動作開始
        if(type == cocos2d::ui::Widget::TouchEventType::ENDED){
            
            auto goSt3 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("gostage3"));
            
            //矢印を非表示&タップ不可に
            goSt3 -> setVisible(false);
            goSt3 -> setTouchEnabled(false);
            
            //ボタンを押せないように
            auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene") -> getChildByName("background")->getChildByName("Button_2"));
            button -> setTouchEnabled(false);

            
            auto backSt1 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("backstage1"));

            //矢印を非表示&タップ不可に
            backSt1 -> setVisible(false);
            backSt1 -> setTouchEnabled(false);
            
            //ピヨを歩行モードに
            this -> walkPiyoToRight();
            
            
            auto piyo = this ->getChildByName("walkPiyo");
            //画面移動
            auto action1 = MoveBy::create(4.0f,Point(selfFrame.width/2,0));
            
            //移動後の処理
            auto action2 = CallFuncN::create([&](Ref *sender){
                
                //キャラクターの動作の停止
                this -> stopPiyo();
                
                //後で書こうね
                
                //ステージ2の矢印を表示
                auto backSt2 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("backstage2"));
                backSt2 -> setVisible(true);
                backSt2 -> setTouchEnabled(true);
                
                //ボタンを押せるように
                auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene") -> getChildByName("background")->getChildByName("Button_3"));
                button -> setTouchEnabled(true);

                
                
                
            });
            
            
            auto seq = Sequence::create(action1, action2, NULL);
            piyo->runAction(seq);
            
        }
        
    });
    
/*********goStage3終*************/
    

/*********backStage1*************/
    //backStage1の動作設定
    auto backSt1 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("backstage1"));
    //動作設定
    backSt1 -> addTouchEventListener([&](Ref* pSender,ui::Widget::TouchEventType type){
        
        //タップ終わり時、動作開始
        if(type == cocos2d::ui::Widget::TouchEventType::ENDED){
            
            auto goSt3 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("gostage3"));
            
            //矢印を非表示&タップ不可に
            goSt3 -> setVisible(false);
            goSt3 -> setTouchEnabled(false);
            
            //ボタンを押せないように
            auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene") -> getChildByName("background")->getChildByName("Button_2"));
            button -> setTouchEnabled(false);

            
            auto backSt1 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("backstage1"));
            
            //矢印を非表示&タップ不可に
            backSt1 -> setVisible(false);
            backSt1 -> setTouchEnabled(false);
            
            
            //ピヨを歩行モードに
            this -> walkPiyoToLeft();
            
            
            auto bk = this ->getChildByName("mainScene")->getChildByName("background");
            //画面移動
            auto action1 = MoveBy::create(4.0f,Point(selfFrame.width/2,0));
            //移動後の処理
            auto action2 = CallFuncN::create([&](Ref *sender){
                
                //キャラクターの動作の停止
                this -> stopPiyo();
                
                //後で書こうね
                
                //ステージ1の矢印を表示
                auto goSt2 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("gostage2"));
                goSt2 -> setVisible(true);
                goSt2 -> setTouchEnabled(true);
                
                //ボタンを押せるように
                auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene") -> getChildByName("background")->getChildByName("Button_1"));
                button -> setTouchEnabled(true);

                
                
                
            });
            
            
            auto seq = Sequence::create(action1, action2, NULL);
            bk->runAction(seq);
            
        }
        
    });
    
/*********backStage1終*************/

    
/*********backStage2*************/
    //goStage2の動作設定
    auto backSt2 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("backstage2"));
    //動作設定
    backSt2 -> addTouchEventListener([&](Ref* pSender,ui::Widget::TouchEventType type){
        
        //タップ終わり時、動作開始
        if(type == cocos2d::ui::Widget::TouchEventType::ENDED){
            
            auto backSt2 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("backstage2"));
            
            //矢印を非表示&タップ不可に
            backSt2 -> setVisible(false);
            backSt2 -> setTouchEnabled(false);
            
            //ボタンを押せないように
            auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene") -> getChildByName("background")->getChildByName("Button_3"));
            button -> setTouchEnabled(false);

            
            //ピヨを歩行モードに
            this -> walkPiyoToLeft();
            
            
            auto piyo = this ->getChildByName("walkPiyo");
            //画面移動
            auto action1 = MoveBy::create(4.0f,Point(-selfFrame.width/2,0));
            //移動後の処理
            auto action2 = CallFuncN::create([&](Ref *sender){
                
                //キャラクターの動作の停止
                this -> stopPiyo();
                
                //ステージ2の矢印を表示
                auto goSt3 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("gostage3"));
                goSt3 -> setVisible(true);
                goSt3 -> setTouchEnabled(true);
                
                auto backSt1 = dynamic_cast<ui::Button*>(this->getChildByName("mainScene")->getChildByName("background")->getChildByName("backstage1"));
                backSt1 -> setVisible(true);
                backSt1 -> setTouchEnabled(true);
                
                //ボタンを押せないように
                auto button = dynamic_cast<ui::Button*>(this->getChildByName("mainScene") -> getChildByName("background")->getChildByName("Button_2"));
                button -> setTouchEnabled(true);

                
                
                
            });
            
            
            auto seq = Sequence::create(action1, action2, NULL);
            piyo->runAction(seq);
            
        }
        
    });
    
/*********backStage2終*************/

    
    
}

void TitleScene::walkPiyoToRight(){
    
    //ぴよを非表示にしてアルクピヨを表示
    this -> getChildByName("piyo") -> setVisible(false);
    auto walkPiyo = CSLoader::getInstance()->createNode("RightWalkSkeletal.csb");
    walkPiyo -> setName("walkPiyo");
    walkPiyo -> setPosition(this -> getChildByName("piyo")-> getPosition());
    this -> addChild(walkPiyo);
    
    //cocostudioで設定したフレームの読み込み→gotoFrameAndPlay(読み込むフレーム位置、ループの可否)
    auto walkAction = CSLoader::getInstance()->createTimeline("RightWalkSkeletal.csb");
    walkPiyo -> runAction(walkAction);
    walkAction -> gotoFrameAndPlay(0, true);
    
    
}

void TitleScene::walkPiyoToLeft(){

/*
    //ぴよを非表示にしてアルクピヨを表示
    this -> getChildByName("piyo") -> setVisible(false);
    auto walkPiyo = CSLoader::getInstance()->createNode("WalkSkeletal.csb");
    walkPiyo -> setName("walkPiyo");
    walkPiyo -> setPosition(this -> getChildByName("piyo")-> getPosition());
    
    //各パーツを配列で取得
    auto parts = walkPiyo -> getChildren();
    //パーツ毎に処理
    for (auto part : parts){

        //パーツの中から画像部分を取得
        auto images = part -> getChildren();
        //画像への処理
        for(auto image : images){
            //スプライトへキャスト
            auto spriteOfImage = dynamic_cast<Sprite*>(image);
            //画像を反転
            spriteOfImage -> setFlippedX(true);
        }
 
        
    }
    
    this -> addChild(walkPiyo);
    
    
    //cocostudioで設定したフレームの読み込み→gotoFrameAndPlay(読み込むフレーム位置、ループの可否)
    auto walkAction = CSLoader::getInstance()->createTimeline("WalkSkeletal.csb");
    
    walkPiyo -> runAction(walkAction->reverse());
    
    walkAction -> gotoFrameAndPlay(0, true);
 
 
*/
    
    //ぴよを非表示にしてアルクピヨを表示
    this -> getChildByName("piyo") -> setVisible(false);
    auto walkPiyo = CSLoader::getInstance()->createNode("LeftWalkSkeletal.csb");
    walkPiyo -> setName("walkPiyo");
    walkPiyo -> setPosition(this -> getChildByName("piyo")-> getPosition());
    this -> addChild(walkPiyo);
    
    //cocostudioで設定したフレームの読み込み→gotoFrameAndPlay(読み込むフレーム位置、ループの可否)
    auto walkAction = CSLoader::getInstance()->createTimeline("LeftWalkSkeletal.csb");
    walkPiyo -> runAction(walkAction);
    walkAction -> gotoFrameAndPlay(0, true);

    
}

void TitleScene::stopPiyo(){
    
    //キャラクターの入れ替え
    this -> getChildByName("piyo") -> setPosition(this -> getChildByName("walkPiyo") -> getPosition());
    this -> getChildByName("piyo") -> setVisible(true);
    
    this -> removeChildByName("walkPiyo");
    
}