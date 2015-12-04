//
//  TopScene.hpp
//  EnglishTestVer2
//
//  Created by 新井脩司 on 2015/11/19.
//
//

#ifndef TopScene_h
#define TopScene_h

#include <stdio.h>
#include "cocos2d.h"

class TopScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    
    //ステージセレクトボタンに動作を設定する。
    void makeStageButton();
        
    // implement the "static create()" method manually
    CREATE_FUNC(TopScene);
    
    void makeTutorial();
  
};

#endif /* TopScene_h */
