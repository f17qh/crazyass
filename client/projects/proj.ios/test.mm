#import "RageIAPHelper.h"
#import <StoreKit/StoreKit.h>
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

#if 0
#include <string>
#include <vector>
bool ProductList(std::vector<std::string> *productlist) {
    [[RageIAPHelper sharedInstance] requestProductsWithCompletionHandler:^(BOOL success, NSArray *products) {
	    if (success) {
		_products = products;
		NSLog(@"Product count %i\n", products.count);
		for (SKProduct *product in products) {
		    productlist->push_back(std::string([product.productIdentifier UTF8String])); 
#if 0
		    bool res = [nsname isEqualToString: product.productIdentifier];
		    if (res) {
			NSLog(@"Buying %@...", product.productIdentifier);
			[[RageIAPHelper sharedInstance] buyProduct:product];
			break;
		    }
#endif
		}
	    }
	    // [self.refreshControl endRefreshing];
	}];
    return true;
}

#if 0
- (void)buyButtonTapped:(id)sender {

    UIButton *buyButton = (UIButton *)sender;
    SKProduct *product = _products[buyButton.tag];

    NSLog(@"Buying %@...", product.productIdentifier);
    [[RageIAPHelper sharedInstance] buyProduct:product];

}
#endif

bool ProductBuy(char *name) {
    NSString *nsname = [NSString stringWithUTF8String: name];
    if (!nsname)
	return false;

    for (int i = 0; i < _products.count; i++) {
	//for (SKProduct *product in _products) {
	SKProduct *product = (SKProduct *)_products[i];
	NSLog(@"buy %@ %@\n", nsname, product.productIdentifier);
	bool res = [nsname isEqualToString: product.productIdentifier];
	if (res) {
	    NSLog(@"Buying %@...", product.productIdentifier);
	    [[RageIAPHelper sharedInstance] buyProduct:product];
	    return true;
	}
    }
    return false;
}
#endif
