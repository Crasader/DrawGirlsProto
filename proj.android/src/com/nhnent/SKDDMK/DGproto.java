/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

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
package com.nhnent.SKDDMK;
import io.fiverocks.android.FiveRocks;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.os.Build;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.KeyEvent;
import android.view.WindowManager;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPOAuthProvider;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.HSPState;
import com.igaworks.adbrixtracersdk.interfaces.ADBrixManager;
//import com.litqoo.lib.KRunnable;
import com.litqoo.lib.KSActivityBase;
import com.litqoo.lib.hspConnector;

public class DGproto extends KSActivityBase{//Cocos2dxActivity{
	public static final String FiveRocks_AppId = "538c30c400821dfba2000001";
	public static final String FiveRocks_AppKey = "LxsEDRsKVnr8_Qd_Uffj";
	public static final int ANDROID_BUILD_GINGERBREAD = 9;
	public static final int SCREEN_ORIENTATION_SENSOR_LANDSCAPE = 6;
	private static native int getUserState();
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		if (Build.VERSION.SDK_INT >= ANDROID_BUILD_GINGERBREAD){
	    	setRequestedOrientation(SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
	    }
		
		FiveRocks.init(this, FiveRocks_AppId, FiveRocks_AppKey);
		FiveRocks.setGLSurfaceView(Cocos2dxGLSurfaceView.getInstance());
		

		if(com.litqoo.lib.hspConnector.setup(10316, "SKDDMK", "1.0.0.KG")){
			Log.i("com.litqoo.dgproto", "hspcore create ok");
			com.litqoo.lib.hspConnector.testRegisterListener();
		}else{
			Log.i("com.litqoo.dgproto","hspcore create fail");
		}
		
	    // Add code to print out the key hash
	    try {
	        PackageInfo info = getPackageManager().getPackageInfo(
	                "com.facebook.samples.hellofacebook", 
	                PackageManager.GET_SIGNATURES);
	        for (Signature signature : info.signatures) {
	            MessageDigest md = MessageDigest.getInstance("SHA");
	            md.update(signature.toByteArray());
	            Log.d("litqoo KeyHash:", Base64.encodeToString(md.digest(), Base64.DEFAULT));
	            }
	    } catch (NameNotFoundException e) {

	    } catch (NoSuchAlgorithmException e) {

	    }
		
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
//		
//		setContentView(R.layout.game_demo);
//		mGLView = (Cocos2dxGLSurfaceView) findViewById(R.id.game_gl_surfaceview);
//		mGLView.setEGLContextClientVersion(2);
//		mGLView.setCocos2dxRenderer(new Cocos2dxRenderer());
//		mGLView.setCocos2dxEditText((Cocos2dxEditText)findViewById(R.id.textField));
	}
	
	public Cocos2dxGLSurfaceView onCreateGLSurfaceView() {
		return new LuaGLSurfaceView(this);
    }

    static { 
    	FiveRocks.loadSharedLibrary();
        System.loadLibrary("cocos2dlua");
    }
    
    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// hspConnector should create stencil buffer
    	mGLView = glSurfaceView;
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	com.litqoo.lib.hspConnector.kInit(this,glSurfaceView,getApplicationContext());
    	
    	return glSurfaceView;
    }
    @Override
    protected void onResume()
    {
         super.onResume();     
         ADBrixManager.startSession(this);
         HSPCore core = HSPCore.getInstance();
         if (HSPCore.getInstance().getState() != HSPState.HSP_STATE_INIT
         		&& HSPCore.getInstance().getState() != HSPState.HSP_STATE_ONLINE) 
         { 
        	 hspConnector.handler.post(
        			 new Runnable(){
        				 public void run() {
        					 Activity activity=(Activity)DGproto.this;
        					 HSPCore core = HSPCore.getInstance();
        					 if(core!=null){
        						 HSPOAuthProvider lType = HSPOAuthProvider.values()[getUserState()];
        						 core.login(activity, lType,new HSPCore.HSPLoginCB() {

        							 public void onLogin(final HSPResult result, boolean isPlayable) {
        								 //Log.d("litqoo", "BEGIN - HSPLoginCB");

        								 HSPCore core = HSPCore.getInstance();
        								 if(core.getState() == HSPState.HSP_STATE_ONLINE)
        								 {
        									 Log.d("hsp", "dfsgfsdg");
        								 }
        								 JSONObject r= new JSONObject();
        								 JSONObject error = new JSONObject();

        								 if (result.isSuccess() == false) {
        									 //Log.i("litqoo", "HSP Login Error = " + result);

        									 // 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 ���占쏙옙占쏙옙���占� 占쏙옙占쏙옙占쎈��占쏙옙占쏙옙占�.
        									 int errorCode = result.getCode();
        									 String errorDescription = result.getDetail();

        									 //Log.i("litqoo", "code = " + errorCode + ", message = " + errorDescription);
        								 }else{
        									 //Log.i("litqoo", "success");
        								 } 
        							 }
        						 });
        					 }else{
        						 //Log.d("litqoo","!!!!!!!! need setup !!!!!!");
        					 }


        				 }
        			 }
        			 );
         }
    }
    private void suspend() {
        // Call suspend() when onPause() is called
        if (HSPCore.getInstance().getState() == HSPState.HSP_STATE_ONLINE) {
            HSPCore.getInstance().suspend(new HSPCore.HSPSuspendCB() {

                @Override
                public void onSuspend(HSPResult result) {
                    if (result.isSuccess()) {
                        Log.d("hsp", "onSuspend success");
                    } else {
                        Log.d("hsp", "onSuspend fail: " + result);
                    }
                }
            });
        }
    }
    @Override
    protected void onPause()
    {
         super.onPause();
         ADBrixManager.endSession();
         suspend();
    }
    //    protected void onActivityResult(int requestCode, int resultCode, Intent data){
//    	com.litqoo.lib.hspConnector.onActivityResult(requestCode, resultCode, data, this);
//    }
    @Override
    protected void onStart() {
      super.onStart();
      FiveRocks.onActivityStart(this);
    }

    @Override
    protected void onStop() {
      FiveRocks.onActivityStop(this);
      super.onStop();
    }
}

class LuaGLSurfaceView extends Cocos2dxGLSurfaceView{
	
	public LuaGLSurfaceView(Context context){
		super(context);
	}
	
	public boolean onKeyDown(int keyCode, KeyEvent event) {
    	// exit program when key back is entered
    	if (keyCode == KeyEvent.KEYCODE_BACK) {
    		android.os.Process.killProcess(android.os.Process.myPid());
    	}
        return super.onKeyDown(keyCode, event);
    }
}


