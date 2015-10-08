//
//  TitleScene.h
//  boki-3
//
//  Created by yasutomo on 2015/06/08.
//
//

#ifndef __boki_3__TitleScene__
#define __boki_3__TitleScene__

#include "cocos2d.h"

class TitleScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    

    //ステージセレクトボタンに動作を設定する。
    void makeStageButton();
    
    //選択したステージに移動
    void moveStage(int stageNumber);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TitleScene);
    
    
};

#endif // __TITLE_SCENE_H__
