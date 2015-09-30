//
//  NSObject+Importcsv.m
//  boki-3
//
//  Created by yasutomo on 2015/06/11.
//
//

#import "NSObject+Importcsv.h"

@implementation NSObject (Importcsv)

- (void)importCsv
{
        _actionTable = [NSMutableArray array];
        NSBundle* bundle = [NSBundle mainBundle];
        NSArray* paths = [bundle pathsForResourcesOfType:@"csv" inDirectory:@"ActionTable"];
        NSAssert(paths.count != 0, @"No action data file found");
        
        for (NSString* path in paths) { //ディレクトリ内全.csvファイルをループ
            NSError* error = nil; // エラーが入る
            NSStringEncoding encoding; //エンコード方式が入る
            NSString* content = [NSString stringWithContentsOfFile:path usedEncoding:&encoding error:&error];
            if (error) {
                NSAssert1(NO, @"Reading action data file failed with error: %@", error.localizedDescription);
            }
            NSArray* rows = [content componentsSeparatedByString:@"\n"]; //ファイル内容を行毎に分解
            NSArray* titleRow = [[rows objectAtIndex:0] componentsSeparatedByString:@","]; //先頭行
            NSMutableArray* actions = [NSMutableArray array]; //結果を入れるArray
            for (int i = 1; i <= rows.count; i++) { //全データ行をループ
                NSArray* row = [[rows objectAtIndex:i] componentsSeparatedByString:@","];
                if ([[row objectAtIndex:0] isEqualToString:@"end"]) {
                    break; // 1ファイル終了
                }
                NSAssert1((row.count == titleRow.count), @"Column count mismatch in action data file %@", path); //列数が異常
                NSDictionary* dictionary = [NSDictionary dictionaryWithObjects:row forKeys:titleRow]; //１行分をDictionaryに入れる
                [actions addObject:dictionary];
            }
            NSString* name = [[path lastPathComponent] stringByDeletingPathExtension]; //パスから拡張子なしのファイル名を取り出す
            Action* action = [[Action alloc] initWithActions:actions name:name];
            [_actionTable addObject:action];
        }
}

@end
