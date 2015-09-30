//
//  ImportCSV.h
//  
//
//  Created by albatrus on 2015/06/11.
//  参考URL http://buildman.xyz/blog/cocos2dx-beginner-18/
//

#ifndef __GitTutorial__ImportCSV__
#define __GitTutorial__ImportCSV__

#include "cocos2d.h"
#include "Config.h"

class ImportCSV : public cocos2d::Node {
    
    
public:

   static cocos2d::Vector<Question*>* importCSV();
   static cocos2d::ValueVector split(const std::string &str, const std::string &delim);
    
};


#endif /* defined(__GitTutorial__ImportCSV__) */
