//
//  LoadScene.cpp
//  FirstFlat
//
//  Created by yasutomo on 2014/12/18.
//
//

#include "LoadScene.h"
#include "TitleScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene *LoadScene::createScene(std::string toScene){
    
    /**
     *  当初static変数に引数を格納し、replaceSceneで条件分岐させる予定だったものの、
     *  謎のエラーが出たため断念。
     *  最終的にsceneのNameプロパティに引数をsetNameすることで対応することとなりました。
     *  targetScene = toScene;
     */
    
    auto scene = Scene::create();
    scene -> setName(toScene);
    auto layer = LoadScene::create();
    scene -> addChild(layer);
    
    
    return scene;
    
}

bool LoadScene::init(){
    
    if (!Layer::init())
    {
        return false;
    }
    
    this -> setColor(Color3B(42, 212, 255));
    

    
    

    return true;
}


//トランジション終了時に呼ばれる
void LoadScene::onEnterTransitionDidFinish(){

    this -> scheduleOnce(schedule_selector(LoadScene::replaceGameScene), 1.0f);
}

//画面の遷移
void LoadScene::replaceGameScene(float delta){
    
    //デストラクタ
    SpriteFrameCache::destroyInstance();

    
    if(this -> getScene() ->getName() == "GameScene"){

        float duration = 0.2f;  //開始→終了にかける時間
        Scene* nextScene = CCTransitionPageTurn::create(duration, GameScene::createScene(),1);
        Director::getInstance()->replaceScene(nextScene);
    }
    
    if(this -> getScene() ->getName() == "TitleScene"){
/*
        float duration = 0.5f;  //開始→終了にかける時間
        Scene* nextScene = CCTransitionPageTurn::create(duration, TitleScene::createScene(),1);
        Director::getInstance()->replaceScene(nextScene);
*/
    }


    
}
