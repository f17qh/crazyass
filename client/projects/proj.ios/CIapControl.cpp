/********************************************************************
	Created:	2011/12/21    
	Filename: 	CIapControl.cpp
	Author:		gelinzhang
	¿‡√˚ CIapControl
	∂‘iapπ∫¬Ú≤Ÿ◊˜Ω¯–– µº game≤Ÿ◊˜µƒ µœ÷¿‡
*********************************************************************/



#include <iostream>
#include <string>
#include <fstream>
#include "CIapControl.h"
//#include "PopupGUI.h"
//#include "ConfigCenter.h"
//#include "TranslationConfig.h"
//#include "GameNetHandler.h"
//#include "Player.h"
//#include "CsvStream.h"
//#include "UI/PanelCenter.h"
//#include "UI/UIPages/BuyShopImpl.h"
//#include "ConfigCenter.h"


extern void ShowLoadingIndicator(bool bForceShow = false);
extern void HideLoadingIndicator();
#if 0
void IAP_ShowLoadingIndicator(bool bForceShow = false)//出现loading圈的接口，根据不同的游戏项目表现不同的loading界面
{
    std::string tmp = RedGame::ConfigCenter::Instance()->GetTranslationConfig()->GetTranslation(100025) ;
    RedGame::PopupGUI::Instance()->ShowNetworkLoadWithOutStop(tmp, 77777);
}
void IAP_HideLoadingIndicator()//隐藏loading圈的接口，根据不同的游戏项目隐藏loading界面
{
    RedGame::PopupGUI::Instance()->HideNetworkLoad(77777);
}
#endif

const char *savefilename = "allTransactionInfo.txt" ;  //保存交易信息的文件名

CIapControl::CIapControl()
{
	ReadTransactionInfoFromFile(savefilename) ;
}

CIapControl::~CIapControl()
{
	CIapControl* tempIapControl = getInstance() ;
	SAFE_DELETE(tempIapControl) ;
}

CIapControl* CIapControl::getInstance()
{
	static CIapControl* singleIapControl = NEW CIapControl() ;
	return singleIapControl ;
}

std::string CIapControl::getCurrentGameUserID()   //得到当前用户的登陆QQ号根据不同的项目实现不同
{
	// std::string result = RedGame::GameNetHandler::GetInstance()->GetPlayer()->GetUin() ;
	// return result ;
    return "";
}

void CIapControl::SetProductNameList()
{
#if 0
    m_GameServerProductList.clear();
    std::vector<RedGame::IapProductData> data = RedGame::ConfigCenter::Instance()->GetRedGameConfig()->GetIapProductList();
    for(int i = 0 ; i < data.size() ; i++)
    {
        struct prdouctInfo tempProduct ;
		tempProduct.productId = data[i].szProductName ;
		tempProduct.productPrice = "0.0" ;
        m_GameServerProductList.push_back(tempProduct) ;
    }
#endif
}

void CIapControl::processPaymentSuccess_forGame(const char* receiptdata, const char*transactionIndertifier , void* pTransaction )  //玩家付款成功后得到交易票据信息后的相应处理根据不同的项目实现不同
{
	//todo
	printf("get receiptdata %s \n",receiptdata);
    
    // RedGame::GameNetHandler::GetInstance()->IapCheck(receiptdata, transactionIndertifier) ;
	//Json::Value json_temp;      // ¡Ÿ ±∂‘œÛ£¨π©»Áœ¬¥˙¬Î π”√
//	
//	std::string userid_string = getCurrentGameUserID() ;
//
//	const char* userId = userid_string.c_str();
//
//	
//	json_temp["uid"] = Json::Value(userId) ;
//
//	json_temp["receipt"] = Json::Value(receiptdata);
//
//	json_temp["transaction_id"] = Json::Value(transactionIndertifier);
//
//	Json::FastWriter fast_writer;
//
//	std::string SendData = fast_writer.write(json_temp)  ;
//
//	std::cout << SendData << std::endl;
//
//	GameNetHandler::GetInstance()->SendIapRecipteCheck(SendData) ;

}

void CIapControl::processShipmentSuccess_forGame()   //IAP服务器正确处理完一个交易后相对应的处理 不同的项目不同的实现
{
//	const char* pUin = getCurrentGameUserID().c_str();
//	if (pUin)
//		GameNetHandler::GetInstance()->UpdateMyBaseInfo(pUin);
//	return  ;
}

extern void GetSaveFilePath(char* filePath, const char* filename);
void CIapControl::WriteTransactionToFile(const char* nousefilename) //写入最新的交易信息根据不同的项目不同实现
{
/*
	printf("WriteTransactionToFile \n");
#ifdef APPLE_STORE_VERSION
	std::string filename = "Transaction.csv";
    
    char tempname[250] ;
    m3eResult tempresult = m3eGetUserDirectory(M3E_STORAGE_EXTERNAL,tempname,250) ;
	std::string CPlatpormPath = tempname;
    CPlatpormPath += "/" ;
    CPlatpormPath += filename ;
	FileStream fs(CPlatpormPath.c_str(), "w");
	for (TransactionInfoMapIter startIter = m_PaymentTransactionMap.begin() ; startIter != m_PaymentTransactionMap.end() ; startIter++)
	{
		std::string Transaction = "" ;
		Transaction += startIter->first ;
		Transaction += "," ;
		Transaction += startIter->second->Transaction_Uid ;
		Transaction += ",\n";
		fs.Write(Transaction);
	}
	fs.Close();
    
#endif
 */
 
	//char buf[1024];
//	GetSaveFilePath(buf,filename);
//	ofstream ofs(buf) ;
//	std::string allTranaticonInfo = "all TransactionInfo" ;
//	for (TransactionInfoMapIter startIter = m_PaymentTransactionMap.begin() ; startIter != m_PaymentTransactionMap.end() ; startIter++)
//	{
//		std::string productInfo = startIter->first ;
//		productInfo += ParserSpacer::SPACER_COLON ;
//		std::string productUserId = startIter->second->Transaction_Uid ;
//		productUserId += ParserSpacer::SPACER_DOLLAR ;
//		productInfo += productUserId ;
//		allTranaticonInfo += productInfo ;
//	}
//	ofs << allTranaticonInfo ;
//	ofs.close() ;
}
void CIapControl::ReadTransactionInfoFromFile(const char* nousefilename) //读取交易消息根据不同项目不同实现
{
    
    /*
#ifdef APPLE_STORE_VERSION
	printf("ReadTransactionInfoFromFile \n");
    std::string filename = "Transaction.csv";
    
    char tempname[250] ;
    m3eResult tempresult = m3eGetUserDirectory(M3E_STORAGE_EXTERNAL,tempname,250) ;
	std::string CPlatpormPath = tempname;
    CPlatpormPath += "/" ;
    CPlatpormPath += filename ;
    RedGame::CsvStream csvStream;
	if (csvStream.Open( CPlatpormPath.c_str() ))
	{
		cleanTransactionInfo() ;
        while (csvStream.ReadLine())
        {
            std::string Transaction_Id = "" ;
            std::string User_Id = "" ;
            csvStream.QueryStringValue( 0, &Transaction_Id);
            csvStream.QueryStringValue( 1, &User_Id);
            TransactionInfo* tempTransactionInfo = NEW TransactionInfo(User_Id,NULL,"") ;
            insertOneTransactionInfo(Transaction_Id,tempTransactionInfo) ;
        }
	}

	
#endif
     */
    
	//char buf[1024];
//	GetSaveFilePath(buf,filename);
//	cleanTransactionInfo() ;
//	ifstream ifs(buf);
//	std::string allTranaticonInfo(istreambuf_iterator<char>(ifs), (istreambuf_iterator<char>()));
//	Array<string> splitArray = splitString(allTranaticonInfo,ParserSpacer::SPACER_DOLLAR);
//	for(unsigned int i = 0 ; i<splitArray.length() ; i++)
//	{
//		Array<string> subSplitArray = splitString(splitArray[i],ParserSpacer::SPACER_COLON);
//		std::string Transaction_Id = subSplitArray[0] ;
//		std::string User_Id = subSplitArray[1] ;
//		TransactionInfo* tempTransactionInfo = NEW TransactionInfo(User_Id,NULL,"") ;
//		insertOneTransactionInfo(Transaction_Id,tempTransactionInfo) ;
//	}
//	cout<<allTranaticonInfo<<endl;
//	ifs.close();
}

void CIapControl::DynamicSetPrice() 
{
#if 0
	for (int i = 0 ; i < m_GameServerProductList.size() ; i++)
	{
        std::string id = m_GameServerProductList[i].productId ;
		std::string price = m_GameServerProductList[i].productPrice ;
        RedGame::ConfigCenter::Instance()->GetRedGameConfig()->SetDynamicPrice(id, price);
		
	}
    
    RedGame::BuyShopImpl* pBuyShop =  RedGame::PanelCenter::Instance()->GetPanel< RedGame::BuyShopImpl >( PT_BuyShop);
    if (pBuyShop)
    {
        pBuyShop->RefreshAllShop() ;
    }
#endif
}

void CIapControl::FailedMessage()
{
#if 0
    std::vector<std::string> messageContextVec;
    messageContextVec.push_back(RedGame::ConfigCenter::Instance()->GetTranslationConfig()->GetTranslation(500001));
    RedGame::PopupGUI::Instance()->ShowMessage(RedGame::PopupGUI::MT_NORMAL, messageContextVec, RedGame::PopupGUI::BT_CONFIRM);
#endif
}

#if 1
void CIapControl::ComplietMessage()
{
#if 0
    std::vector<std::string> messageContextVec;
    messageContextVec.push_back(RedGame::ConfigCenter::Instance()->GetTranslationConfig()->GetTranslation(500002));
    RedGame::PopupGUI::Instance()->ShowMessage(RedGame::PopupGUI::MT_NORMAL, messageContextVec, RedGame::PopupGUI::BT_CONFIRM);
#endif
}

void CIapControl::ExceptionMessage()
{
#if 0
    std::vector<std::string> messageContextVec;
    messageContextVec.push_back(RedGame::ConfigCenter::Instance()->GetTranslationConfig()->GetTranslation(500005));
    RedGame::PopupGUI::Instance()->ShowMessage(RedGame::PopupGUI::MT_NORMAL, messageContextVec, RedGame::PopupGUI::BT_CONFIRM);
#endif
}

void CIapControl::GetListErrorMessage()
{
#if 0
    if (GetShowProductListError())
    {
        std::vector<std::string> messageContextVec;
        messageContextVec.push_back(RedGame::ConfigCenter::Instance()->GetTranslationConfig()->GetTranslation(500003));
        RedGame::PopupGUI::Instance()->ShowMessage(RedGame::PopupGUI::MT_NORMAL, messageContextVec, RedGame::PopupGUI::BT_CONFIRM);
    }
#endif
}

void CIapControl::OpenIAPMessage()
{
#if 0
    std::vector<std::string> messageContextVec;
    messageContextVec.push_back(RedGame::ConfigCenter::Instance()->GetTranslationConfig()->GetTranslation(500004));
    RedGame::PopupGUI::Instance()->ShowMessage(RedGame::PopupGUI::MT_NORMAL, messageContextVec, RedGame::PopupGUI::BT_CONFIRM);
#endif
}
#endif





