//
//  SaveSQL.h
//  
//
//  Created by albatrus on 2014/06/10.
//
//

#ifndef __GitTutorial__SaveSQL__
#define __GitTutorial__SaveSQL__

#include "cocos2d.h"
#include "sqlite3.h"
#include "Config.h"
#include "ImportCSV.h"

class SaveSQL : public cocos2d::Node {
    
    
public:
    
    virtual  bool init();
    
    //データベース接続
    static int sqliteOpen(sqlite3 **db);
    //テーブル作成
    static void sqliteCreateTable();
    //sql insert(初期)
    static void sqliteSetValue();
  
    //sql select(keyによって振る舞いを変える予定)
    static cocos2d::Vector<Question*>*sqliteGetValueForKey(const char *key, int limit);
      
    //sql update
    static void sqliteUpdateValueForKey(const char *key,const char *value);
    
};


#endif /* defined(__GitTutorial__SaveSQL__) */
