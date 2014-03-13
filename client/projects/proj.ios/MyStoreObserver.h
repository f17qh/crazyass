//
//  MyStoreObserver.h
//  SignatureN


#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h> 
#import <StoreKit/SKPaymentTransaction.h> 

#include "CIapControl.h"


//@class IIapControl ;

@interface MyStoreObserver : NSObject< SKProductsRequestDelegate,SKPaymentTransactionObserver> 
{
	IIapControl* m_pIapControlObject ;

}

+ (MyStoreObserver *)sGetInstance ;
+ (IIapControl*) getS_pIapControlObject;
+ (void)initIap ;
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
-(void) PurchasedTransaction: (SKPaymentTransaction *)transaction;
- (void) completeTransaction: (SKPaymentTransaction *)transaction;
- (void) failedTransaction: (SKPaymentTransaction *)transaction;

- (void) restoreTransaction: (SKPaymentTransaction *)transaction;

-(void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentTransaction *)transaction;
-(void) paymentQueue:(SKPaymentQueue *) paymentQueue restoreCompletedTransactionsFailedWithError:(NSError *)error;


//- (void) requestProductData ;
- (void) requestProductData:(NSSet *)productIDs ;
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response  ;

- (void) setMyIapControl: (IIapControl*) pIapControlObject;
- (void) startOnePayment:(const char*) prodouctId ;

//- (void) presentMessageBox : (NSString*) messagesinfo ;

+ (bool) checkCanMakePurchases ;

- (void) handleProductListTimerOut;
- (void) handlePaymentTimerOut;
@end
