package com.qfighting.hipmania;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.telephony.TelephonyManager;
import android.util.Log;

import com.qufan.allpaysdk.APayLoad;
import com.qufan.allpaysdk.payutils.Common;

import com.qfighting.hipmania.ca;

public class googleplay {
    Activity currentAct;
    ca cpp;

    static String googlePayKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAnVSGzND0Nim5mlufzgC3zg0E8MqyIITKPbk0Aulus82XWRKIJFsczZbmrFDSdD8ugh06ZkJs9gDsnXarkxOZaSqpjo4Hsu4ubpuO+iEp5bkozKvQ6SLKroDzBLQPNgPRS2o2cr3lyce9n4LP1NysfYdzrQhm/LJYT29Z0LfvEe/TgiU1fOQozViGQKcf4B78mBFWxRuwfxpkVow+0EEwPALy/Ld0LDkNL7WNY/nLrb/teweSVg38fs6b4gyl9Vyu1zRMRVLXB59ZeeNdXasj/m1RI3XWzhp96tbqSnLtd8xoyXa0ngsaif5wxkt0alEvvj2LHZjpkvQoA0qagoZ4rwIDAQAB";

    public void setActivity(Activity act) {
	currentAct = act;
	cpp = new ca();
    }

    public void pay(String userid, String itemid, float cost) {
	Intent intent = new Intent(currentAct, APayLoad.class);
	Bundle bundle = new Bundle();
	bundle.putFloat("cost", cost);
	bundle.putString("item_id", itemid);
	bundle.putInt("bill_type", 0);
	bundle.putString("userid", userid);
	bundle.putString("google_app_key", googlePayKey);
	bundle.putString("app_key", "d9839%&nkfpy9&jxzs-*4&t2lj*_0j7@");
	bundle.putString("source", "ass");
	bundle.putString("pass_info", "passinfo");
	intent.putExtras(bundle);
	currentAct.startActivityForResult(intent, Common.RESULT_CODE);
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
	if (Common.RESULT_CODE == requestCode){
//		Log.e("data", data.toString());
	    boolean isPaySuccess = data.getBooleanExtra("is_success", false);
	    String resultMsg = data.getStringExtra("pay_meaage");
	    String itemId = data.getStringExtra("item_id");
	    String billID = data.getStringExtra("bill_id");
	    Log.i("crazyass", String.format("pay res %s %s %d", itemId, resultMsg, resultCode));

	    if (isPaySuccess) {
		if (itemId == "com.qfkj.crazyass.peach5")
		    cpp.CAAddTapjoyPoint(5);
		else if (itemId == "com.qfkj.crazyass.peach30")
		    cpp.CAAddTapjoyPoint(30);
		else if (itemId == "com.qfkj.crazyass.peach60")
		    cpp.CAAddTapjoyPoint(60);
		else if (itemId == "com.qfkj.crazyass.peach200")
		    cpp.CAAddTapjoyPoint(200);
		else if (itemId == "com.qfkj.crazyass.peach700")
		    cpp.CAAddTapjoyPoint(700);
	    }
	}
    }
}
