/********************************************************************
	Created:	2011/12/21    
	Filename: 	CIapControl.h
	Author:		gelinzhang
	类名 CIapControl
	对iap购买操作进行实际game操作的接口类
*********************************************************************/

#ifndef CIapControl_h__
#define CIapControl_h__

#include "IIapControl.h"

class CIapControl : public IIapControl
{
public:
	static CIapControl* getInstance() ;
	~CIapControl() ;


	virtual void processPaymentSuccess_forGame(const char* receiptdata, const char*transactionIndertifier, void* pTransaction  )   ;  //处理付款成功回调（当appstroe 返回给ios 支付成功后调用）
	virtual void processShipmentSuccess_forGame()  ;  // 处理发货成功回调 （当游戏服务器返回已经成功发货后调用）
	virtual void DynamicSetPrice() ;  //动态设置商品人民币价格

	virtual std::string getCurrentGameUserID() ;
	virtual void WriteTransactionToFile(const char* filename)  ;
	virtual void ReadTransactionInfoFromFile(const char* filename)  ;
    virtual void SetProductNameList() ;
    
    virtual void FailedMessage()  ;
    virtual void ComplietMessage()  ;
    virtual void ExceptionMessage()  ;
    virtual void GetListErrorMessage()  ;
    virtual void OpenIAPMessage();

private:
	CIapControl() ; 
};

#endif // CIapControl