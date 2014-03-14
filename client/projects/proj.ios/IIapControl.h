/********************************************************************
	Created:	2011/12/21    
	Filename: 	IIapControl.h
	Author:		gelinzhang
	���� IIapControl
	��iap�����������ʵ��game�����Ľӿ���
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

struct TransactionInfo       //��¼��ǰios�豸��δ��ɵĽ���ID�����ڼ����ò�ͬQQ�ŵ�¼���Ƿ���
{
	std::string  Transaction_Uid ;
	//std::string  Transaction_receipt ;
	void* pTransaction ;

	std::string stringReceiptdata  ;  //����Ʊ����Ϣ

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

//struct GameTransactionInfo  //һ����Ϸ�ڲ��Ľ��׼�¼�����������Ѿ���ɸ���ȷδƱ����֤�Ľ���
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
	virtual bool RequesetProductList(bool needloading = false) ;  //��ȡ��ǰappstore���۵���Ʒlist
	virtual void PayforOneTypeProduct(std::string productId) ; //����һ����Ʒ
	const char* getGameServerProductInfo(unsigned int Productindex) ;
	
	static void initIapContext() ;
	int getGameServerProductCount() ;
	
    void SetShowProductListError(bool data) ;
    bool GetShowProductListError();
    
    virtual void clearAppStoreProductInfo()  ;
	virtual void insertAppStoreProductInfo(const char* productID,const char* ProductPrice)  ;
	virtual void processPaymentSuccess(const char* receiptdata, const char*transactionIndertifier , void* pTransaction, bool needInsertTransactionInfo = true)   ;  //������ɹ��ص�����appstroe ���ظ�ios ֧���ɹ�����ã�
	virtual void processShipmentSuccess(const char*transactionIndertifier)  ;  // �������ɹ��ص� ������Ϸ�����������Ѿ��ɹ���������ã�
	virtual void RecheckTransactionInfo() ;//���·��ͷ���������У������
	virtual void processPaymentSuccess_forGame(const char* receiptdata, const char*transactionIndertifier , void* pTransaction)  = 0 ;  //������ɹ��ص�����appstroe ���ظ�ios ֧���ɹ�����ã�
	virtual void processShipmentSuccess_forGame() = 0 ;  // �������ɹ��ص� ������Ϸ�����������Ѿ��ɹ���������ã�
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
	virtual bool CheckIsVaildTransaction(const char*transactionIndertifier,std::string CurrentUserID) ; //��Ҫ���뽻��id �͵�ǰ������QQ����
	virtual void insertOneTransactionInfo(std::string id, TransactionInfo* tempInfo) ;
	virtual void cleanTransactionInfo() ;
	
	friend class CIapControl ;

private:
	
	std::vector<std::string> m_AppStoreProductList ; //��ǰappstore���۵���Ʒlist ȷ����������в�ȥ�����Ѿ��¼ܵ���Ʒ
	std::vector<prdouctInfo> m_GameServerProductList ;//��ǰGameServer���ṩ�������ƷList

	TransactionInfoMap m_PaymentTransactionMap ; //������Ϣ����
    bool  m_bShowProductListError ;
};

#endif // IIapControl_h__