/********************************************************************
	Created:	2011/12/21    
	Filename: 	IIapControl.h
	Author:		gelinzhang
	类名 IIapControl
	对iap购买操作进行实际game操作的接口类
*********************************************************************/

#ifndef IIapControl_h__
#define IIapControl_h__


#include <vector>
#include <map>
#include <string>
//#include "tinyxml.h"

#ifdef WIN32
//#define NEW new(__FILE__, __LINE__)
#define NEW new
#else
#define NEW new
#endif
#ifndef NULLPTR
#define NULLPTR 0
#endif
#define SAFE_ASSERT(value) assert(value)
#define SAFE_DELETE(p) { if((p)) delete (p); (p) = NULLPTR; }
#define SAFE_DELETE_ARRAY(p) { if((p))  delete[] (p); (p) = NULLPTR; }
#define CHECK_VALUE_RET(value) {if( !(value) ) { SAFE_ASSERT(false); return false; }}

//class SKPaymentTransaction ;

struct TransactionInfo       //记录当前ios设备上未完成的交易ID，用于鉴别用不同QQ号登录，是否发送
{
	std::string  Transaction_Uid ;
	//std::string  Transaction_receipt ;
	void* pTransaction ;

	std::string stringReceiptdata  ;  //交易票据信息

	TransactionInfo(std::string uid , void* pdata ,std::string CharReceiptData)
	{
		Transaction_Uid = uid ;
		pTransaction = pdata ;
		stringReceiptdata = CharReceiptData ;
	}

	~TransactionInfo()
	{
		Transaction_Uid = "" ;
		stringReceiptdata = "" ;
		pTransaction = NULL ;
	}
};

//struct GameTransactionInfo  //一次游戏内部的交易记录，用于重启已经完成付款确未票据验证的交易
//{
//
//};

struct prdouctInfo
{
	std::string productId ;
	std::string productPrice ;
public:
	prdouctInfo()
	{
		productId = 
		productPrice = "" ;
	}
};

class IIapControl
{
	
public:
	typedef std::map<std::string, TransactionInfo*> TransactionInfoMap ;
	typedef std::pair<std::string, TransactionInfo*> TransactionInfoPair ;
	typedef TransactionInfoMap::iterator TransactionInfoMapIter ;

	virtual ~IIapControl();
	virtual bool RequesetProductList(bool needloading = false) ;  //获取当前appstore在售的物品list
	virtual void PayforOneTypeProduct(std::string productId) ; //购买一种商品
	const char* getGameServerProductInfo(unsigned int Productindex) ;
	
	static void initIapContext() ;
	int getGameServerProductCount() ;
	
    void SetShowProductListError(bool data) ;
    bool GetShowProductListError();
    
    virtual void clearAppStoreProductInfo()  ;
	virtual void insertAppStoreProductInfo(const char* productID,const char* ProductPrice)  ;
	virtual void processPaymentSuccess(const char* receiptdata, const char*transactionIndertifier , void* pTransaction, bool needInsertTransactionInfo = true)   ;  //处理付款成功回调（当appstroe 返回给ios 支付成功后调用）
	virtual void processShipmentSuccess(const char*transactionIndertifier)  ;  // 处理发货成功回调 （当游戏服务器返回已经成功发货后调用）
	virtual void RecheckTransactionInfo() ;//重新发送服务器交易校验请求
	virtual void processPaymentSuccess_forGame(const char* receiptdata, const char*transactionIndertifier , void* pTransaction)  = 0 ;  //处理付款成功回调（当appstroe 返回给ios 支付成功后调用）
	virtual void processShipmentSuccess_forGame() = 0 ;  // 处理发货成功回调 （当游戏服务器返回已经成功发货后调用）
	virtual std::string getCurrentGameUserID() = 0;
	virtual void WriteTransactionToFile(const char* filename) = 0 ;
	virtual void ReadTransactionInfoFromFile(const char* filename) = 0 ;
	virtual void DynamicSetPrice() = 0;
    virtual void SetProductNameList() = 0;
    
    virtual void FailedMessage() = 0 ;
    virtual void ComplietMessage() = 0 ;
    virtual void ExceptionMessage() = 0 ;
    virtual void GetListErrorMessage() = 0 ;
    virtual void OpenIAPMessage() = 0 ;

private:
	virtual bool CheckIsVaildTransaction(const char*transactionIndertifier,std::string CurrentUserID) ; //需要传入交易id 和当前操作的QQ号码
	virtual void insertOneTransactionInfo(std::string id, TransactionInfo* tempInfo) ;
	virtual void cleanTransactionInfo() ;
	
	friend class CIapControl ;

private:
	
	std::vector<std::string> m_AppStoreProductList ; //当前appstore在售的物品list 确保购买过程中不去购买已经下架的商品
	std::vector<prdouctInfo> m_GameServerProductList ;//当前GameServer中提供购买的物品List

	TransactionInfoMap m_PaymentTransactionMap ; //交易信息集合
    bool  m_bShowProductListError ;
};

#endif // IIapControl_h__