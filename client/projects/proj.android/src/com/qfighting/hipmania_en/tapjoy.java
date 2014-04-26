package com.qfighting.hipmania_en;

import android.app.Activity;
import android.util.Log;

import java.util.Hashtable;

import com.tapjoy.TapjoyConnect;
import com.tapjoy.TapjoyConnectFlag;
import com.tapjoy.TapjoyConnectNotifier;
import com.tapjoy.TapjoyEarnedPointsNotifier;
import com.tapjoy.TapjoyNotifier;
import com.tapjoy.TapjoyOffersNotifier;
import com.tapjoy.TapjoySpendPointsNotifier;
import com.tapjoy.TapjoyViewNotifier;

import com.qfighting.hipmania_en.ca;

public class tapjoy implements TapjoyNotifier {
    static int TapjoyPoint = 0;
    static boolean TapjoyInit = false;
    Activity currentAct;
    ca cpp;

    public void setActivity(Activity act) {
	currentAct = act;
	cpp = new ca();
    }

    public void startConnect() {
	// OPTIONAL: For custom startup flags.
	Hashtable<String, Object> connectFlags = new Hashtable<String, Object>();
	connectFlags.put(TapjoyConnectFlag.ENABLE_LOGGING, "true");

	String tapjoyAppID = "74934c9e-e596-42b4-aa0a-9d76cdc9a861";
	String tapjoySecretKey = "Z3k2pGIRTJiVxJizCjN1";

	// NOTE: This is the only step required if you're an advertiser.
	TapjoyConnect.requestTapjoyConnect(currentAct.getApplicationContext(), tapjoyAppID, tapjoySecretKey, connectFlags, new TapjoyConnectNotifier()
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
		    // Best Practice: We recommend calling getTapPoints as often as possible so the user��撅拷 balance is always up-to-date.
		    TapjoyConnect.getTapjoyConnectInstance().getTapPoints(tapjoy.this);
		}
	    });
    }

    public void onConnectFail() {
	// Toast.makeText(this, "tapjoy Fail", Toast.LENGTH_LONG).show();
	Log.i("crazyass", "Tapjoy Fail");
    }

    public void onEarnedPoints(int amount) {
	Log.i("crazyass",String.format( "Tapjoy earned %d peaches", amount));
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
	cpp.CAAddTapjoyPoint(TapjoyPoint);

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
}
