/********************************************************************
	Created:	2011/12/21    
	Filename: 	CIapControl.h
	Author:		gelinzhang
	���� CIapControl
	��iap�����������ʵ��game�����Ľӿ���
*********************************************************************/

#ifndef CIapControl_h__
#define CIapControl_h__

#include "IIapControl.h"

class CIapControl : public IIapControl
{
public:
	static CIapControl* getInstance() ;
	~CIapControl() ;


	virtual void processPaymentSuccess_forGame(const char* receiptdata, const char*transactionIndertifier, void* pTransaction  )   ;  //������ɹ��ص�����appstroe ���ظ�ios ֧���ɹ�����ã�
	virtual void processShipmentSuccess_forGame()  ;  // �������ɹ��ص� ������Ϸ�����������Ѿ��ɹ���������ã�
	virtual void DynamicSetPrice() ;  //��̬������Ʒ����Ҽ۸�

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