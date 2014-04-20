// #include "net_web_socket.h"

// //bool EnablePanty = false;
// //bool EnableTapjoy = true;
// //bool EnableSound = true;
// CAWS * CAWS::caws_ = NULL;
// CAWS * CAWS::Instance() {
//   if (caws_)
//     return caws_;
//   caws_ = new CAWS;
//   return caws_;
// }

// CADelegate * sharedDelegate() {
//   return CAWS::Instance()->delegate();
// }

// void MyTarget::CARecv(const CSJson::Value& result) {
//   CCLOG("%s\n", __FUNCTION__);
//   if(result.get("ErrCode", -1).asInt() == 0) {
//     CSJson::Value body = result["Body"];
//     EnablePanty = body.get("Panty", false).asBool();
//     EnableTapjoy = body.get("Tapjoy", false).asBool();
//     EnableSound = body.get("Sound", false).asBool();

//     CSJson::Value control;
//     CSJson::StyledWriter writer;
//     control["Panty"] = EnablePanty;
//     control["Tapjoy"] = EnableTapjoy;
//     control["Sound"] = EnableSound;
//     std::string buf = writer.write(control);
//     CCLOG("Save Control info %s\n", buf.c_str());
//     CCUserDefault::sharedUserDefault()->setStringForKey("control_data", buf);
//     return;
//   }
// }
// void MyTarget::CAOpen() {
//   CSJson::Reader reader;
//   CSJson::Value control;
//   std::string buf = CCUserDefault::sharedUserDefault()->getStringForKey("control_data");
//   CCLOG("load:%s\n", buf.c_str());

//   if (!reader.parse(buf, control, false)) {
//     CCLOG("parse error");
//     return;
//   }

//   EnablePanty = control["Panty"].asBool();
//   EnableTapjoy = control["Tapjoy"].asBool();
//   EnableSound = control["Sound"].asBool();
// }
// void MyTarget::CARecvTimeout() {
//   //CCLOG("%s\n", __FUNCTION__);
//   //CSJson::Value control;
//   //CSJson::StyledWriter writer;
//   //control["Panty"] = EnablePanty;
//   //control["Tapjoy"] = EnableTapjoy;
//   //control["Sound"] = EnableSound;
//   //std::string buf = writer.write(control);
//   //CCLOG("Save Control info %s\n", buf.c_str());
//   //CCUserDefault::sharedUserDefault()->setStringForKey("control_data", buf);
// }
