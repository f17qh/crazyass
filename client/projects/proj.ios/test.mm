#import "RageIAPHelper.h"
#import <StoreKit/StoreKit.h>
#import <UIKit/UIKit.h>
#import "UMFeedback.h"
#import "RootViewController.h"

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

extern void CAProductByNotify(char *, void *target);

@interface CAIAP: NSObject
-(void) getProcustList;
-(BOOL) buyProduct: (NSString *)name;
-(void) productPurchased:(NSNotification *)notification;
-(void) setTarget: (void *)target;
@end

@implementation CAIAP {
    NSArray *_products;
    BOOL _isget;
    void *_target;
}

-(void) setTarget: (void *)target {
    _target = target;
}

-(void) getProcustList {
    [[RageIAPHelper sharedInstance] requestProductsWithCompletionHandler:^(BOOL success, NSArray *products) {
	    if (success) {
		_products = [products retain];
		NSLog(@"Product count %i\n", _products.count);
		_isget = YES;
	    }
	}];

    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(productPurchased:) name:IAPHelperProductPurchasedNotification object:nil];
}

- (void)productPurchased:(NSNotification *)notification {
    NSString * productIdentifier = notification.object;
    if (productIdentifier == nil) {
	CAProductByNotify(NULL, _target);
    } else {
	CAProductByNotify((char *)[productIdentifier UTF8String], _target);
    }
    
}

-(BOOL) buyProduct: (NSString *)name {
    NSLog(@"try to buy %@\n", name);
    if (!_isget)
	return NO;
    if ([_products count] == 0)
	return NO;
    for (SKProduct *product in _products) {
	NSLog(@"buy %@ %@\n", name, product.productIdentifier);
	BOOL res = [name isEqualToString: product.productIdentifier];
	if (res) {
	    NSLog(@"Buying %@...", product.productIdentifier);
	    [[RageIAPHelper sharedInstance] buyProduct:product];
	    break;
	}
    }
    return YES;
}
@end

void *ProductList() {
    CAIAP *obj;
    obj = [CAIAP new];
    [obj getProcustList];
    return (void *)obj;
}

bool ProductBuy(void *obj, char *name, void *target) {
    NSString *nsname = [NSString stringWithUTF8String: name];
    if (!nsname)
	return false;

    CAIAP *iap = (CAIAP *)obj;
    [iap setTarget:target];
    if ([iap buyProduct:nsname])
	return true;
    return false;
}

#define UMENG_APPKEY @"4eeb0c7b527015643b000003"
void ShowFeedback() {
#if 0
    UIViewController *view = [UIApplication sharedApplication].keyWindow.rootViewController;
    [UMFeedback showFeedback:view withAppkey:UMENG_APPKEY];
#endif

    RootViewController *view = (RootViewController *)[UIApplication sharedApplication].keyWindow.rootViewController;
    [view showNativeFeedbackWithAppkey];
}
