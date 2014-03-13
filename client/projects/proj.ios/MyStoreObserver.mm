//
//  MyStoreObserver.m
//  SignatureN


#import "MyStoreObserver.h"
#import<UIKit/UIKit.h>
#import<UIKit/UIAlert.h>



//extern void IAP_ShowLoadingIndicator(bool bForceShow = false);
//extern void IAP_HideLoadingIndicator();

void g_initIap()
{
	[MyStoreObserver initIap] ;
}

void getRequestProducts(bool haveloading)
{
	
	NSSet * idSet = [NSSet set];
	
	NSString * idString = NULL;
	int iCount = CIapControl::getInstance()->getGameServerProductCount();
	for (int i = 0; i < iCount; i ++)
	{
		const char* tempProductId  = CIapControl::getInstance()->getGameServerProductInfo(i);
		idString = [NSString stringWithUTF8String:tempProductId];
		idSet = [idSet setByAddingObject:idString];
        NSLog(@"requestProducts:%@", idString);
	}
    if (haveloading)
    {
 //       IAP_ShowLoadingIndicator();
    }
	[[MyStoreObserver sGetInstance] requestProductData:idSet] ;
}

void requestOnePayment(const char* ProductsId)
{
	
	[[MyStoreObserver sGetInstance] startOnePayment:ProductsId] ;
	return ;	
}

IIapControl* getStoreObserverControl()
{
	return [MyStoreObserver getS_pIapControlObject] ;
}

//void presentMessageBox(const char* messageInfo)
//{
//	NSString* NSStringMessageInfo = [NSString stringWithUTF8String:messageInfo];
//	[[MyStoreObserver sGetInstance] presentMessageBox:NSStringMessageInfo] ;
//	return ;
//}

void FinishOnTransaction(void *transaction)
{
    SKPaymentTransaction* pPaymentTransaction = reinterpret_cast<SKPaymentTransaction*>(transaction) ;
//	IAP_HideLoadingIndicator();
	//printf("FinishOnTransaction transaction = %d \n",*transaction);
	[[SKPaymentQueue defaultQueue] finishTransaction: pPaymentTransaction];
    
    [transaction release];
	//presentMessageBox("交易已完成，谢谢！");
    getStoreObserverControl()->ComplietMessage();
}



@implementation MyStoreObserver

bool isWaitingProductList = false ;
bool isWaitingPaymentResult = false ;

NSTimer* m_pTimerForRequestProductList ;
NSTimer* m_pTimerForPayMent ;

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
	NSLog(@"paymetnQueue");
	for (SKPaymentTransaction* transaction in transactions)
	{
		switch (transaction.transactionState)
		{
			case SKPaymentTransactionStatePurchased:
				isWaitingPaymentResult = false ;
			//	[m_pTimerForPayMent fire] ;
				NSLog(@"Complete Transaction");
				[self completeTransaction:transaction];
				break;
			case SKPaymentTransactionStateFailed:
				isWaitingPaymentResult = false ;
		//		[m_pTimerForPayMent fire] ;
				NSLog(@"Transaction StateFailed");
				[self failedTransaction:transaction];
				break;
			case SKPaymentTransactionStateRestored:
				NSLog(@"Transaction  SKPaymentTransactionStateRestored ");
				[self restoreTransaction:transaction] ;
				
				break;
				
			case SKPaymentTransactionStatePurchasing:
				NSLog(@"Transaction  SKPaymentTransactionStatePurchasing ");
				break ;
				
			default:
				break;
		}
	}
	
}

-(void) PurchasedTransaction: (SKPaymentTransaction *)transaction{
	NSArray *transactions =[[NSArray alloc] initWithObjects:transaction, nil];
	[self paymentQueue:[SKPaymentQueue defaultQueue] updatedTransactions:transactions];
	[transactions release];
}
				
- (void) completeTransaction: (SKPaymentTransaction *)transaction
{
	NSString *transactionReceipttese = [[NSString alloc] initWithData:transaction.transactionReceipt encoding:NSUTF8StringEncoding];
	NSLog(@"transactionReceipt = %@", transactionReceipttese);
	if (getStoreObserverControl() != NULL)
	{
        [transaction retain];
        
		const char* receipteData = [transactionReceipttese UTF8String] ;
		const char* transaction_id = [transaction.transactionIdentifier UTF8String] ;

		getStoreObserverControl()->processPaymentSuccess(receipteData,transaction_id,transaction) ;
//		IAP_HideLoadingIndicator();
	}
	[transactionReceipttese release];
}

- (void) failedTransaction: (SKPaymentTransaction *)transaction
{
//	IAP_HideLoadingIndicator();
	if (transaction.error.code != SKErrorPaymentCancelled) 
	{
//		UIAlertView * alert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"交易失败，稍候再试谢谢！" delegate:self
//											   cancelButtonTitle:@"确定" otherButtonTitles:nil];
//		
//		[alert show];
//		[alert release];
        getStoreObserverControl()->FailedMessage();
	}
	
	getStoreObserverControl()->RecheckTransactionInfo();
	[[NSNotificationCenter defaultCenter] postNotificationName:@"faliedTransaction" object:nil];
	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void) restoreTransaction: (SKPaymentTransaction *)transaction
{
//	IAP_HideLoadingIndicator();
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

-(void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentTransaction *)transaction{

}

-(void) paymentQueue:(SKPaymentQueue *) paymentQueue restoreCompletedTransactionsFailedWithError:(NSError *)error
{

}


#pragma mark connection delegate
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
	NSLog(@"didReceiveData%@",  [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease]);
}
- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{

}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response  //now no use
{  
	switch([(NSHTTPURLResponse *)response statusCode])
	{
		case 200:
		case 206:
			break;
		case 304:
	
			break;
		case 400:

			break;
			
			
		case 404:
		break;
		case 416:
			break;
		case 403:
			break;
		case 401:
		case 500:
			break;
		default:
			break;
	}	
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error 
{
	

}

#pragma mark -
#pragma mark SotoreKit method

- (void) requestProductData:(NSSet *)productIDs
{
	NSLog(@"requestProductData");
	SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers: productIDs];
	request.delegate = self;
	[request start];
	
	m_pTimerForRequestProductList = [NSTimer scheduledTimerWithTimeInterval:20 target:self selector:@selector(handleProductListTimerOut) userInfo:self repeats:NO];
	isWaitingProductList = true ;

}

- (void) handleProductListTimerOut
{
	if (isWaitingProductList)
	{
	//	m_pTimerForPayMent = NULL ;
	//	IAP_HideLoadingIndicator();
		isWaitingPaymentResult = false ;
//		UIAlertView * alert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"取商品列表出错，网络不好或机器破解后可能会造成这种问题！" delegate:self
//                                               cancelButtonTitle:@"确定" otherButtonTitles:nil];
//        
//        [alert show];
//        [alert release];
        getStoreObserverControl()->GetListErrorMessage();
	}
	return  ;
}

- (void) handlePaymentTimerOut
{
	
	if (isWaitingPaymentResult)
	{
	//	IAP_HideLoadingIndicator();
		isWaitingPaymentResult = false ;
	}
	
	return  ;
}

#pragma mark request delegate
//!收到产品的消息
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
	isWaitingProductList = false ;
//	IAP_HideLoadingIndicator();
	NSLog(@"ProductsRequest did receiveResponse");
	NSArray *myProduct = response.products;
	int ProductCount = [myProduct count] ;
	NSLog(@"the count of products is %d", ProductCount);
		
	// populate UI
	for(SKProduct *product in myProduct)
	{

			
		if(m_pIapControlObject != NULL)
		{
			const char* tempProductID = [[product productIdentifier] UTF8String] ;
			float pricedata = [[product price] doubleValue] ;
			NSNumber* nsPriceData = [NSNumber numberWithFloat:pricedata] ;
			
			//	NSLocale *templocale = [[[NSLocale alloc] initWithLocaleIdentifier:@"sv_SE"] autorelease];
			NSLocale* localedata = [product priceLocale] ;
			NSNumberFormatter* fmtr = [[[NSNumberFormatter alloc] init] autorelease];
			[fmtr setNumberStyle:NSNumberFormatterCurrencyStyle];
			[fmtr setLocale:localedata];
			NSString* resultPrice =  [fmtr stringFromNumber:nsPriceData] ;
			
			const char* cpricedata = [resultPrice UTF8String] ;

			
			NSLog( @"%@", resultPrice );
			
			
			m_pIapControlObject->insertAppStoreProductInfo(tempProductID,cpricedata);
		}
		
	}
	
	for (NSString *invalidProductId in response.invalidProductIdentifiers)
	{
		NSLog(@"Invalid product id: %@" , invalidProductId);
	}
	
	
	if (ProductCount == 0) 
	{
//		UIAlertView * alert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"取商品列表出错，网络不好或机器破解后可能会造成这种问题！" delegate:self
//                                               cancelButtonTitle:@"确定" otherButtonTitles:nil];
//        
//        [alert show];
//        [alert release];
        getStoreObserverControl()->GetListErrorMessage();
		return  ;
	}
	else 
	{
		m_pIapControlObject->DynamicSetPrice();
	}

	
	[request autorelease];
	
}

- (void) setMyIapControl: (IIapControl*) pIapControlObject
{
	if(pIapControlObject != NULL)
		m_pIapControlObject = pIapControlObject ;
	else 
		NSLog(@"setMyIapControl have error pIapControlObject == NULL") ;

}

+ (MyStoreObserver *)sGetInstance 
{
	static MyStoreObserver* s_mStoreObserver = NULL ;
	if(s_mStoreObserver == NULL)
	{
		s_mStoreObserver = [[MyStoreObserver alloc] init ] ;
		[s_mStoreObserver setMyIapControl : CIapControl::getInstance()];
	}
	return s_mStoreObserver ;
}

+ (void)initIap 
{
	static bool HaveInitIap = false ; //从游戏开始到整个游戏进程关闭我们只初始化一次Iap
	if (!HaveInitIap) 
	{
		MyStoreObserver *tempObserver = [MyStoreObserver sGetInstance];
		[[SKPaymentQueue defaultQueue] addTransactionObserver:tempObserver];
		if ([MyStoreObserver checkCanMakePurchases])
		{
			NSLog(@"SkpaymentQueue canMakePayments") ;
		}
		else 
		{
			NSLog(@"SkpaymentQueue close makePayments") ;
		}
		HaveInitIap = true ;
	}
}

+ (IIapControl*) getS_pIapControlObject
{
	MyStoreObserver * tempStoreObserver = [MyStoreObserver sGetInstance] ;
	return tempStoreObserver->m_pIapControlObject ;
}

- (void) startOnePayment:(const char*) prodouctId 
{
	if ([MyStoreObserver checkCanMakePurchases] == FALSE)
	{
		return ;
	}
	if(prodouctId != NULL)
	{
//		IAP_ShowLoadingIndicator();
		NSString *tempProdouctId = [[NSString alloc] initWithUTF8String:prodouctId] ;
		SKPayment *payment = [SKPayment paymentWithProductIdentifier:tempProdouctId];
		[[SKPaymentQueue defaultQueue] addPayment:payment];
		
		[tempProdouctId release] ;
		isWaitingPaymentResult = true ;
		//m_pTimerForPayMent = [NSTimer scheduledTimerWithTimeInterval:40 target:self selector:@selector(handlePaymentTimerOut) userInfo:self repeats:NO];
		
	}
	else 
	{
//		UIAlertView * alert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"取商品列表出错，网络不好或机器破解后可能会造成这种问题！" delegate:self
//                                               cancelButtonTitle:@"确定" otherButtonTitles:nil];
//        
//        [alert show];
//        [alert release];
        getStoreObserverControl()->GetListErrorMessage();
	}

	
}

+(bool)checkCanMakePurchases
{
    if ([SKPaymentQueue canMakePayments] == false)
    {
      //  UIAlertView * alert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"请开启手机IAP购买功能" delegate:self
       //                                        cancelButtonTitle:@"确定" otherButtonTitles:nil];
        
       // [alert show];
       // [alert release];
        getStoreObserverControl()->OpenIAPMessage();
		return FALSE ;
    }
	else 
	{
		return TRUE ;
	}

}

//- (void) presentMessageBox : (NSString *) messageInfo
//{
//	UIAlertView * alert = [[UIAlertView alloc] initWithTitle:@"提示" message:messageInfo delegate:self
//										   cancelButtonTitle:@"确定" otherButtonTitles:nil];
//	
//	[alert show];
//	[alert release];
//	
//}


@end
