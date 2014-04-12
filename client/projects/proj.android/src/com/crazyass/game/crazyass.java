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
import com.umeng.fb.FeedbackAgent;
//import com.umeng.ui.BaseSinglePaneActivity;

public class crazyass extends Cocos2dxActivity{
    static FeedbackAgent agent;	
    static boolean TapjoyInit = false;
    protected void onCreate(Bundle savedInstanceState){
	super.onCreate(savedInstanceState);

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
	String tapjoyAppID = "bba49f11-b87f-4c0f-9632-21aa810dd6f1";
	// REPLACE THE SECRET KEY WITH YOUR SECRET KEY.
	String tapjoySecretKey = "yiQIURFEeKm0zbOggubu";
		
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

    public void onConnectSuccess() {
	// Toast.makeText(getContext(), "tapjoy Success", Toast.LENGTH_LONG).show();
	Log.i("crazyass", "Tapjoy Success");
	TapjoyInit = true;
    }

    public void onConnectFail() {
	// Toast.makeText(this, "tapjoy Fail", Toast.LENGTH_LONG).show();
	Log.i("crazyass", "Tapjoy Fail");
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

    static {
        System.loadLibrary("cocos2dcpp");
    }     
}
