//
//  Question.h
//  boki-3
//
//  Created by yasutomo on 2015/06/09.
//
//

#ifndef boki_3_Question_h
#define boki_3_Question_h

#include "cocos2d.h"

class Question : public cocos2d::Sprite{
    public :
        //id
        int questionId;
        //種別
        std::string type;
        //日本語
        std::string japanese;
        //英語
        std::string english;
};


#endif
