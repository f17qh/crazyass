/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.crazyass.game;

import java.util.Hashtable;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.os.Bundle;
import android.util.Log;

import com.tapjoy.TapjoyConnect;
import com.tapjoy.TapjoyConnectFlag;
import com.tapjoy.TapjoyConnectNotifier;
import com.tapjoy.TapjoyOffersNotifier;
import com.tapjoy.TapjoyEarnedPointsNotifier;
import com.tapjoy.TapjoyViewNotifier;
import com.tapjoy.TapjoyViewType;
import com.tapjoy.TapjoyNotifier;
import com.tapjoy.TapjoySpendPointsNotifier;
import com.umeng.fb.FeedbackAgent;
//import com.umeng.ui.BaseSinglePaneActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.telephony.TelephonyManager;
import com.qufan.activity.APayLoad;
import com.qufan.paysdk.payutils.Common;

public class crazyass extends Cocos2dxActivity implements TapjoyNotifier{
    static FeedbackAgent agent;	
    static boolean TapjoyInit = false;

    static int TapjoyPoint = 0;
    static Activity SaveThis;
    static String DeviceID;

    static String googlePayKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAnVSGzND0Nim5mlufzgC3zg0E8MqyIITKPbk0Aulus82XWRKIJFsczZbmrFDSdD8ugh06ZkJs9gDsnXarkxOZaSqpjo4Hsu4ubpuO+iEp5bkozKvQ6SLKroDzBLQPNgPRS2o2cr3lyce9n4LP1NysfYdzrQhm/LJYT29Z0LfvEe/TgiU1fOQozViGQKcf4B78mBFWxRuwfxpkVow+0EEwPALy/Ld0LDkNL7WNY/nLrb/teweSVg38fs6b4gyl9Vyu1zRMRVLXB59ZeeNdXasj/m1RI3XWzhp96tbqSnLtd8xoyXa0ngsaif5wxkt0alEvvj2LHZjpkvQoA0qagoZ4rwIDAQAB";

    protected void onCreate(Bundle savedInstanceState){
	super.onCreate(savedInstanceState);
	SaveThis = this;

	TelephonyManager tm = (TelephonyManager) this.getSystemService(TELEPHONY_SERVICE);
	DeviceID = tm.getDeviceId();

	// OPTIONAL: For custom startup flags.
	Hashtable<String, Object> connectFlags = new Hashtable<String, Object>();
	connectFlags.put(TapjoyConnectFlag.ENABLE_LOGGING, "true");
		
	// If you are not using Tapjoy Managed currency, you would set your own user ID here.
	//	connectFlags.put(TapjoyConnectFlag.USER_ID, "A_UNIQUE_USER_ID");
	   
	// You can also set your event segmentation parameters here.
	//  Hashtable<String, String> segmentationParams = new Hashtable<String, String>();
	//  segmentationParams.put("iap", "true");
	//  connectFlags.put(TapjoyConnectFlag.SEGMENTATION_PARAMS, segmentationParams);
		
	// Connect with the Tapjoy server.  Call this when the application first starts.
	// REPLACE THE APP ID WITH YOUR TAPJOY APP ID.
	String tapjoyAppID = "74934c9e-e596-42b4-aa0a-9d76cdc9a861";
	// REPLACE THE SECRET KEY WITH YOUR SECRET KEY.
	String tapjoySecretKey = "Z3k2pGIRTJiVxJizCjN1";
		
	// NOTE: This is the only step required if you're an advertiser.
	TapjoyConnect.requestTapjoyConnect(getApplicationContext(), tapjoyAppID, tapjoySecretKey, connectFlags, new TapjoyConnectNotifier()
	    {
		@Override
		public void connectSuccess() {
		    onConnectSuccess();
		}

		@Override
		public void connectFail() {
		    onConnectFail();
		}
	    });
    }

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// crazyass should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    public void onEarnedPoints(int amount) {
	Log.i("crazyass",String.format( "Tapjoy earned %d peaches", amount));
    }

    public void onConnectSuccess() {
	// Toast.makeText(getContext(), "tapjoy Success", Toast.LENGTH_LONG).show();
	Log.i("crazyass", "Tapjoy Success");
	TapjoyInit = true;

	// Get notifications whenever Tapjoy currency is earned.
	TapjoyConnect.getTapjoyConnectInstance().setEarnedPointsNotifier(new TapjoyEarnedPointsNotifier() {
		@Override
		public void earnedTapPoints(int amount) {
		    Log.i("crazyass", "XXXX");
		    onEarnedPoints(amount);
		}
	    });

	// Get notifications when Tapjoy views open or close.
	TapjoyConnect.getTapjoyConnectInstance().setTapjoyViewNotifier(new TapjoyViewNotifier() {
		@Override
		public void viewWillOpen(int viewType) {
		    Log.i("crazyass", "viewWillOepn");
		}
			
		@Override
		public void viewWillClose(int viewType) {
		    Log.i("crazyass", "viewWillClose");
		}
			
		@Override
		public void viewDidOpen(int viewType) {
		    Log.i("crazyass", "viewDidOpen");
		}
	       	
		@Override
		public void viewDidClose(int viewType) {
		    Log.i("crazyass", "viewDidClose");
		    // Best Practice: We recommend calling getTapPoints as often as possible so the user瑾� balance is always up-to-date.
		    TapjoyConnect.getTapjoyConnectInstance().getTapPoints(crazyass.this);
		}
	    });
    }

    public void onConnectFail() {
	// Toast.makeText(this, "tapjoy Fail", Toast.LENGTH_LONG).show();
	Log.i("crazyass", "Tapjoy Fail");
    }

    //================================================================================
    // TapjoyNotifier Methods
    //================================================================================	
    @Override
    public void getUpdatePointsFailed(String error) {
	Log.i("crazyass", "getUpdatePointsFailed");
    }
	
    @Override
    public void getUpdatePoints(String currencyName, int pointTotal) {
	Log.i("crazyass", "currencyName: " + currencyName);
	Log.i("crazyass", "pointTotal: " + pointTotal);

	TapjoyPoint = pointTotal;
	CAAddTapjoyPoint(TapjoyPoint);

	// start spend
	TapjoyConnect.getTapjoyConnectInstance().spendTapPoints(pointTotal, new TapjoySpendPointsNotifier() {
		@Override
		public void getSpendPointsResponseFailed(String error) {
		    Log.i("crazyass", "Spendpoint faild");
		}
		@Override
		public void getSpendPointsResponse(String currencyName, int pointTotal) {
		}
	    });
    }

    static public void startFeedBack() {
	agent = new FeedbackAgent(getContext());
	//agent = new FeedbackAgent(this.getActivity());
	agent.startFeedbackActivity();
    }

    static public void startTAPOffers() {
	if (TapjoyInit == false) {
	    Log.i("crazyass", "Tapjoy init failed");
	    return;
	}

	TapjoyConnect.getTapjoyConnectInstance().showOffers(new TapjoyOffersNotifier() {
		@Override
		public void getOffersResponse() {
		    Log.i("crazyass", "Tapjoy offer Success");
		}
						
		@Override
		public void getOffersResponseFailed(String error) {
		    Log.i("crazyass", "Tapjoy offer Success");
		}
	    });
    }

    public native void CAAddTapjoyPoint(int amount);

    static public int GetTapjoyPoint() {
	Log.i("crazyass", String.format("GetTapjoyPoint %d", TapjoyPoint));
	// return TapjoyPoint;
	return 5;
    }

    static public void SetTapjoyPoint(int amount) {
	TapjoyPoint = amount;
    }

    static public Activity getJavaActivity() {
	return SaveThis;
    }

    static public String getDeviceID() {
	Log.i("crazyass", String.format("DeviceID %s", DeviceID));
	return DeviceID;
    }

    static public void payGooglePlay(String userid, String itemid, float cost){
	// Log.i("crazyass", String.format("payGoogle %s %s %f", userid, itemid, cost));
	// Log.i("crazyass",
	//       String.format("cost %f item_id %s app_key %s",
	// 		    cost, itemid, googlePayKey));
	Intent intent = new Intent(SaveThis, APayLoad.class);
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
	SaveThis.startActivityForResult(intent, Common.RESULT_CODE);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	if (Common.RESULT_CODE == requestCode){
	    boolean isPaySuccess = data.getBooleanExtra("is_success", false);
	    String resultMsg = data.getStringExtra("pay_meaage");
	    String itemId = data.getStringExtra("item_id");
	    String billID = data.getStringExtra("bill_id");
	    Log.i("crazyass", String.format("pay res %s %s %d", itemId, resultMsg, resultCode));

	    if (isPaySuccess) {
		if (itemId == "com.qfkj.crazyass.peach5")
		    CAAddTapjoyPoint(5);
		else if (itemId == "com.qfkj.crazyass.peach30")
		    CAAddTapjoyPoint(30);
		else if (itemId == "com.qfkj.crazyass.peach60")
		    CAAddTapjoyPoint(60);
		else if (itemId == "com.qfkj.crazyass.peach200")
		    CAAddTapjoyPoint(200);
		else if (itemId == "com.qfkj.crazyass.peach700")
		    CAAddTapjoyPoint(700);
	    }
	}
	super.onActivityResult(requestCode, resultCode, data);
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }     
}
