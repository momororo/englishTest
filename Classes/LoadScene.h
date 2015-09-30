//
//  LoadScene.h
//  FirstFlat
//
//  Created by yasutomo on 2014/12/18.
//
//

#ifndef __FirstFlat__LoadScene__
#define __FirstFlat__LoadScene__

#include <stdio.h>
#include "cocos2d.h"


class LoadScene : public cocos2d::Layer{
    
protected:

    
    
    public :
    
    static cocos2d::Scene *createScene(std::string toScene);
    
    virtual bool init();

    virtual void onEnterTransitionDidFinish();

    void replaceGameScene(float delta);

/*諸事情でボツ
    //飛ぶ対象を格納する変数
    static std::string targetScene;
*/

    
    CREATE_FUNC(LoadScene);

    
};


#endif /* defined(__FirstFlat__LoadScene__) */
