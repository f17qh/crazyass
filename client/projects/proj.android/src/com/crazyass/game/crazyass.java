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

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.telephony.TelephonyManager;
import android.util.Log;

import com.umeng.fb.FeedbackAgent;
import com.qfighting.hipmania.googleplay;
import com.qfighting.hipmania.tapjoy;

public class crazyass extends Cocos2dxActivity {
    static FeedbackAgent agent;	
    static boolean TapjoyInit = false;

    static int TapjoyPoint = 0;
    static Activity SaveThis;
    static String DeviceID;

    static googleplay gpay;
    static tapjoy tap;

    protected void onCreate(Bundle savedInstanceState){
	super.onCreate(savedInstanceState);
	SaveThis = this;

	gpay = new googleplay();
	gpay.setActivity(this);

	TelephonyManager tm = (TelephonyManager) this.getSystemService(TELEPHONY_SERVICE);
	DeviceID = tm.getDeviceId();

	tap = new tapjoy();
	tap.setActivity(this);
	tap.startConnect();
    }

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// crazyass should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static public void startFeedBack() {
	agent = new FeedbackAgent(getContext());
	//agent = new FeedbackAgent(this.getActivity());
	agent.startFeedbackActivity();
    }

    static public void startTAPOffers() {
	tap.startTAPOffers();
    }

    public native void CAAddTapjoyPoint(int amount);

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
	gpay.pay(userid, itemid, cost);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	gpay.onActivityResult(requestCode, resultCode, data);
	super.onActivityResult(requestCode, resultCode, data);
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }     
}
