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

    static String googlePayKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgLOST32YvYFmFro6+6T3vbWyNa2Rjl5QaotHnS2CJRkChv0LhRVUk4zYsRcE3n+i44T6VOX06tvRy4F26/WLXXrBfcMrX+WseBQq5K68CR456F6a68IHlWMvNlQu4T9oMyo20k3/n0h+CAztoC6+IJC9e1iv9fgsVQ8a7mrlE84BGXe9JRKNMYFNJee6COhFDpvD9jokcnKaHcOm/Y7Gp827ZGpuJbV1BStTR0UYp2KfNMKogzRqNY+FRYdS7dM2CvrmXc/2my//4rc+6ywULUywW/yncx+/uwZGuehkCMg5U+xUmkRtE7Cp4yOkyatgsv4xQ1aXm4gnWRrABrnrNQIDAQAB";

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
