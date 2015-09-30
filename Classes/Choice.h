//
//  Choice.h
//  boki-3
//
//  Created by yasutomo on 2015/06/09.
//
//

#ifndef boki_3_Choice_h
#define boki_3_Choice_h

class Choice : public cocos2d::Sprite{
    public :
        int questionNo;
        std::string text;
        int answerTag;
        std::string answerText;
        std::string commentary;

};

#endif
