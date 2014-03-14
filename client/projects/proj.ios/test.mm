#import <UIKit/UIKit.h>

void CAWriteFile(char *filename, char *content) {
    NSString *nsfilename = [NSString stringWithUTF8String:filename];
    NSString *nscontent = [NSString stringWithUTF8String:content];

    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentDirectory = [paths objectAtIndex:0];
    NSString * file = [documentDirectory stringByAppendingPathComponent:nsfilename];
    NSData *data = [nscontent dataUsingEncoding: NSUTF8StringEncoding];
    [data writeToFile:file atomically:YES];
}

void CAReadFile(char *filename, char *content, size_t len) {
    NSString *nsfilename = [NSString stringWithUTF8String:filename];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentDirectory = [paths objectAtIndex:0];
    NSString * file = [documentDirectory stringByAppendingPathComponent:nsfilename];
    NSData *data=[NSData dataWithContentsOfFile:file options:0 error:NULL];
    char *b = (char *)[data bytes];
    if (b)
	strncpy(content, b, len);
}
