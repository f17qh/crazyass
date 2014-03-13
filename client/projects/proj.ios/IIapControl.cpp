/********************************************************************
Created:	2011/12/21    
Filename: 	IIapControl.cpp
Author:		gelinzhang
¿‡√˚ CIapControl
∂‘iapπ∫¬Ú≤Ÿ◊˜Ω¯–– µº game≤Ÿ◊˜µƒΩ”ø⁄¿‡
*********************************************************************/



#include "IIapControl.h"
extern void getRequestProducts(bool haveloading);
extern void requestOnePayment(const char* ProductsId);
extern void FinishOnTransaction(void* ptransaction) ;

extern const char *savefilename ;


IIapControl::~IIapControl()
{
}


bool IIapControl::RequesetProductList(bool needloading)   //ªÒ»°µ±«∞appstore‘⁄ €µƒŒÔ∆∑list
{
#ifdef WIN32
	
	return true ;
#else
    SetShowProductListError(needloading);
	if(m_AppStoreProductList.size() == 0 ) //√ª”–µ√µΩπ˝apple storeµƒ…Ã∆∑œ˚œ¢
    {
        SetProductNameList();
        clearAppStoreProductInfo();
        getRequestProducts(needloading) ;
    }
    else
        DynamicSetPrice();
	return true ;
#endif
}

void IIapControl::PayforOneTypeProduct(std::string productId) //π∫¬Ú“ª÷÷…Ã∆∑
{
	for(unsigned int i = 0 ; i < m_AppStoreProductList.size(); i++)
	{
		if (productId == m_AppStoreProductList[i])
		{
			printf("this product exist at appstore \n") ;
			//todo
#ifdef WIN32
#else
			requestOnePayment(productId.c_str());
#endif
			return ;
		}
	}
#ifdef WIN32
#else
	requestOnePayment(NULL) ;
#endif
	return ;
}

const char* IIapControl::getGameServerProductInfo(unsigned int Productindex)
{
	if (Productindex >= 0 && Productindex < m_GameServerProductList.size())
	{
		return m_GameServerProductList[Productindex].productId.c_str() ;
	}
	return NULL ;
}

int IIapControl::getGameServerProductCount()
{
	return m_GameServerProductList.size() ;
}

void IIapControl::clearAppStoreProductInfo()
{
    m_AppStoreProductList.clear();
}

void IIapControl::insertAppStoreProductInfo(const char* productID,const char* ProductPrice) 
{
	m_AppStoreProductList.push_back(productID);
	if (productID)
	{
		std::string tempProductID = productID ;
		for (unsigned int i = 0 ; i< m_GameServerProductList.size() ; i++)
		{
			if (m_GameServerProductList[i].productId == tempProductID && ProductPrice)
			{
				m_GameServerProductList[i].productPrice = ProductPrice ; 
			}
		}

	}
	
}

void IIapControl::insertOneTransactionInfo(std::string id, TransactionInfo* tempInfo)
{
	TransactionInfoMapIter tempIter = m_PaymentTransactionMap.find(id) ;
	if (tempIter == m_PaymentTransactionMap.end())
	{
		m_PaymentTransactionMap.insert(TransactionInfoPair(id,tempInfo)) ;
	}
	else
	{
		if (tempInfo)
		{
			tempIter->second->pTransaction = tempInfo->pTransaction ;
			SAFE_DELETE(tempInfo) ;
		}
		
	}
}

void IIapControl::cleanTransactionInfo() 
{
	m_PaymentTransactionMap.clear() ;
}



bool IIapControl::CheckIsVaildTransaction(const char* id,std::string CurrentUseId) 
{
	TransactionInfoMapIter tempIter = m_PaymentTransactionMap.find(id ) ;
	if (tempIter == m_PaymentTransactionMap.end())
	{
		return true ;
	}
	else
	{
		std::string transaction_UID = tempIter->second->Transaction_Uid ;
		if (transaction_UID == CurrentUseId)
		{
			return true ;
		}
		else
		{
			return false ;
		}
	}
	return false ;
}

void IIapControl::processPaymentSuccess(const char* receiptdata, const char*transactionIndertifier , void* pTransaction ,bool needInsertTransactionInfo)
{
	std::string userid_string = getCurrentGameUserID() ;

	const char* userId = userid_string.c_str();
	if (userid_string.length() > 1 && CheckIsVaildTransaction(transactionIndertifier,userid_string) )
	{
		processPaymentSuccess_forGame(receiptdata,transactionIndertifier,pTransaction) ;
		if (needInsertTransactionInfo)
		{
			TransactionInfo* temppTransactionInfo = NEW TransactionInfo(userId,pTransaction,receiptdata) ;
			insertOneTransactionInfo(transactionIndertifier,temppTransactionInfo) ;
		}

		WriteTransactionToFile(savefilename);

		printf("CIapControl::processPaymentSuccess \n") ;
	}
	else
	{
		printf("processPaymentSuccess userid is NULL\n") ;
	}

}

extern void presentMessageBox(const char* messageinfo) ;
void IIapControl::processShipmentSuccess(const char*transactionIndertifier) 
{
	//todo
	TransactionInfoMapIter tempIter = m_PaymentTransactionMap.find(transactionIndertifier ) ;
	if (tempIter == m_PaymentTransactionMap.end())
	{

#ifdef WIN32
		printf("CIapControl::processShipmentSuccess have error please check it \n") ;
#else

		//std::string tInfo = "交易处理出现异常，下次启动游戏将会重启这次交易！" ;
		//presentMessageBox(tInfo.c_str()) ;
        ExceptionMessage();
#endif

		return ;
	}
	else
	{
		void* needEndTransaction = tempIter->second->pTransaction ;
		if (needEndTransaction != NULL)
		{
#ifdef WIN32
			SAFE_DELETE(tempIter->second) ;
			m_PaymentTransactionMap.erase(tempIter) ;

#else
			FinishOnTransaction(needEndTransaction) ;
			processShipmentSuccess_forGame();

			SAFE_DELETE(tempIter->second) ;
			m_PaymentTransactionMap.erase(tempIter) ;
#endif	
		}
	}
	return  ;
}

void IIapControl::RecheckTransactionInfo()
{
	if (m_PaymentTransactionMap.size() == 0)
	{
		return ;
	}
	else
	{
		for (TransactionInfoMapIter startIter = m_PaymentTransactionMap.begin() ; startIter != m_PaymentTransactionMap.end() ;startIter++)
		{
			TransactionInfo *tempTransaction = startIter->second ;
			if (tempTransaction && tempTransaction->pTransaction && tempTransaction->stringReceiptdata.length() !=0 )
			{
				if (tempTransaction->Transaction_Uid == getCurrentGameUserID())
				{
					std::string transactionId = startIter->first ;
					processPaymentSuccess(tempTransaction->stringReceiptdata.c_str(),transactionId.c_str(),tempTransaction->pTransaction,false) ;
				}
			}
		}

	}
}

void IIapControl::SetShowProductListError(bool data)
{
    m_bShowProductListError = data ;
}

bool IIapControl::GetShowProductListError()
{
    return m_bShowProductListError ;
}

extern void g_initIap() ;
void IIapControl::initIapContext() 
{
#ifdef WIN32
#else
	g_initIap() ;
    
#endif
}






