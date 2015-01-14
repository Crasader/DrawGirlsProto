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
package com.nhnent.SKSUMRAN;
import io.fiverocks.android.FiveRocks;

import java.io.IOException;
import java.io.InputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONObject;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.os.Build;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View.OnSystemUiVisibilityChangeListener;
import android.view.WindowManager;
import android.widget.Toast;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPMessage;
import com.hangame.hsp.HSPMessage.HSPReceivePushNotificationListener;
import com.hangame.hsp.HSPOAuthProvider;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.HSPState;
import com.igaworks.IgawCommon;
//import com.igaworks.IgawCommon;
//import com.kamcord.android.Kamcord;
//import com.kamcord.android.Kamcord;
import com.litqoo.lib.KSActivityBase;
import com.litqoo.lib.hspConnector;
//import com.litqoo.lib.KRunnable;
import com.toast.android.analytics.GameAnalytics;

@SuppressLint("NewApi")
public class DGproto extends KSActivityBase{//Cocos2dxActivity{
	public static final String FiveRocks_AppId = "53c4918600821d86a400000e";
	public static final String FiveRocks_AppKey = "bSYyKMPM-oSSEMEvOPMW";
	public static final int ANDROID_BUILD_GINGERBREAD = 9;
	public static final int SCREEN_ORIENTATION_SENSOR_LANDSCAPE = 6;
	private static native int getUserState();
	private static LuaGLSurfaceView glSurfaceView;
	
	protected String getHSPConfProp(String prop)
	{
		//***XML문서를 Document라는 객체로 인식.***
        //1.문서를 읽기위한 공장을 만들어야 한다.
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        //2.빌더를 생성
        DocumentBuilder builder = null;
		try {
			builder = factory.newDocumentBuilder();
		} catch (ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        //3.생성된 빌더를 통해서 xml문서를 Document객체로 파싱해서 가져온다.
        //Document org에서 가져온다!!!

        Document doc = null;
		try {
			InputStream in = getAssets().open("HSPConfiguration.xml");
//			in.
			doc = builder.parse(in);
		} catch (SAXException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        Element root = doc.getDocumentElement();
        //configuration 이라는 이름을 가진 요소들을 모두 가려내는 작업.
        NodeList n_list = root.getElementsByTagName("configuration");
        for(int i=0; i<n_list.getLength(); i++)
        {
        	Node s_node = n_list.item(i);
        	NamedNodeMap Attrs = s_node.getAttributes();
        	String key = null, value = null;
            for (int j = 0; j < Attrs.getLength(); j++){
               Node attr = Attrs.item(j);
               if(attr.getNodeName().equals("key"))
               {
            	   key = attr.getNodeValue();
               }
               if(attr.getNodeName().equals("value"))
               {
            	   value = attr.getNodeValue();
               } 
            } 
            
            if(key.equals(prop))
            {
            	return value;
            }
        } 
		return "";
	}
	protected String getBuildType()
	{
		String isDevelop = getHSPConfProp("LIT_DEVELOP_SERVER");
		if(isDevelop.equals("true"))
			return "dev";
		
		String launching_zone = getHSPConfProp("HSP_LAUNCHING_ZONE");
		if(launching_zone.equals("ALPHA-KR"))
			return "alpha";
		else
			return "real";
	}	
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);

		String versionName = null;
		try {
			PackageInfo info = getPackageManager().getPackageInfo(
					"com.nhnent.SKSUMRAN", 
					PackageManager.GET_SIGNATURES);
			
			versionName = info.versionName;		
		} catch (NameNotFoundException e) {

		}
		
		int result = GameAnalytics.initializeSdk(getApplicationContext(), "QA8LmqYa", 
				"tk8on6Xe", versionName, true);
		
		if(result != GameAnalytics.S_SUCCESS)
		{

			Log.d("toast", "initialize error " + GameAnalytics.getResultMessage(result));

		}	
		SharedPreferences pref = getSharedPreferences("pref", MODE_PRIVATE);
        pref.getString("check", "");
        if(pref.getString("check", "").isEmpty()){
        	Intent shortcutIntent = new Intent(Intent.ACTION_MAIN);
        	shortcutIntent.addCategory(Intent.CATEGORY_LAUNCHER);
        	shortcutIntent.setClassName(this, getClass().getName());
        	shortcutIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK| 
        			Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
        	Intent intent = new Intent();

        	intent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, shortcutIntent);
        	intent.putExtra(Intent.EXTRA_SHORTCUT_NAME, getResources().getString(R.string.app_name));
        	intent.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE,
        			Intent.ShortcutIconResource.fromContext(this, R.drawable.icon));
        	intent.putExtra("duplicate", false);
        	intent.setAction("com.android.launcher.action.INSTALL_SHORTCUT");

        	
        	sendBroadcast(intent);
        }
        SharedPreferences.Editor editor = pref.edit();
        editor.putString("check", "exist");
        editor.commit();
        

		//aaaaa
		Log.i("com.litqoo.dgproto", "init1 kamcord");
		//Kamcord.initKeyAndSecret("VlEoFwFydvNVhMhMCPIlPTuwO79AATr3eMuixaF4YFO",
		//		"Ecl3mH6WxvG8T3lsrqbtAAOBrRq1AE664D7VYpMgZ7b",
		//		"drawgirls");

		//Kamcord.initActivity(this);

		if (Build.VERSION.SDK_INT >= ANDROID_BUILD_GINGERBREAD){
			setRequestedOrientation(SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
		}

		if(getBuildType().equals("real"))
		{
			FiveRocks.init(this, FiveRocks_AppId, FiveRocks_AppKey);
			FiveRocks.setGLSurfaceView(Cocos2dxGLSurfaceView.getInstance());
		}
		
		//FiveRocks.setDebugEnabled(true);

		if(com.litqoo.lib.hspConnector.setup(10331, "SKSUMRAN", versionName)){
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
		//		Kamcord.initKeyAndSecret(
		//		        "Ir45fxMERyaSCWYFnaxz8Km0I6IhtiGWtMBqFuaz7N0",
		//		        "UqS6SftTrZNWSuzP5WryaeFQK5gJ1oYFQTlMOHmctBK",
		//		        "175998");
		//		Kamcord.initActivity(this);

		//IgawCommon.startApplication(DGproto.this);

		// 'isupdate'占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙���占� 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占� 2.4 ������������占쏙옙占쏙옙������������占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙���占� 癲ワ옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙��������������뱄옙占썲����뀐옙占쏙옙占쏙옙占쏙옙占썹��占썲����뀐옙占쏙옙占쏙옙占쏙옙占�. 'loglevel' 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙��겸��占쏙옙占쏙옙占쎈��占썩�쇽옙占썲����뀐옙占쏙옙占쏙옙占쏙옙占� 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占� 占쏙옙棺占쎌��占쏙옙占쏙옙占썼キ占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙��겸��������������占쏙옙占쏙옙占쏙옙���占� 占쏙옙占쏙옙占쏙옙占쏙옙占썹��占썲����뀐옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙���占�. �����������ⓦ�쇽옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占� 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙���占� 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占썲�������������뀐옙占썲��占� '0(占쏙옙占쏙옙占쏙옙占쏙옙占썲��������占쏙옙占쏙옙占쏙옙占쏙옙���占� 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙���占�)'占쏙옙占쏙옙占쏙옙占쏙옙占썲����������ⓦ�쇽옙占쏙옙占쏙옙���占� 占쏙옙占쏙옙占쏙옙占쏙옙占썹��占썲����뀐옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占�.
//		AdXConnect.getAdXConnectInstance(getApplicationContext(), false, 1);
//
//		AdXConnect.getAdXConnectEventInstance(getApplicationContext(),"Launch","","");

		IgawCommon.startApplication(DGproto.this);
		
		
		//占쏙옙占썲����뀐옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占� 占쏙옙占쏙옙占쏙옙占쏙옙占썹뭐占썲����뀐옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙��������������뀐옙占쏙옙占쏙옙占쏙옙占� 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占썩�⑥��占쏙옙占쏙옙占쏙옙占쏙옙占� Push Listener 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占�: 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占썲��占� 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占� ���������占쏙옙占썲����뀐옙占쏙옙占쏙옙占쏙옙占�-占쏙옙占썲����뀐옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙���占� 占쏙옙占쏙옙占쏙옙占쏙옙占썲��������占쏙옙占쏙옙占쏙옙占쏙옙占썲��占� 濚�占썸��占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占썲��占� 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占�
		HSPMessage
		    .addPushNotificationReceiveListener(new HSPReceivePushNotificationListener() {
		       @Override
		       public void onPushNotificationReceive(Map<String, Object> pushData) {
		           Toast.makeText(DGproto.this, (CharSequence) pushData.get("content"), Toast.LENGTH_SHORT).show();
		       }
		});
		
	}
	void hideSystemUI()
	{
		// Set the IMMERSIVE flag.
		// Set the content to appear under the system bars so that the content
		// doesn't resize when the system bars hide and show.

		if (Build.VERSION.SDK_INT >= 11){
			int hideOption = Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_STABLE;

			if (Build.VERSION.SDK_INT >= 19){
				hideOption = hideOption | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
						| Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
						| Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
						| Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
						| Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_FULLSCREEN; // hide status bar
			}


			glSurfaceView.setSystemUiVisibility(hideOption);


		}

	}



	public void onWindowFocusChanged(boolean hasFocus)
	{
		super.onWindowFocusChanged(hasFocus);
		if (hasFocus)
		{
			this.hideSystemUI();
		}
	}

	public Cocos2dxGLSurfaceView onCreateGLSurfaceView() {
		return new LuaGLSurfaceView(this);
	}

	static { 
		//FiveRocks.loadSharedLibrary();
		//System.loadLibrary("kamcord");
//		System.load("/data/data/com.nhnent.SKSUMRAN/lib/libcocos2dlua.so");
		System.loadLibrary("cocos2dlua");
	}

	public Cocos2dxGLSurfaceView onCreateView() {
		glSurfaceView = new LuaGLSurfaceView(this);
		// hspConnector should create stencil buffer
		mGLView = glSurfaceView;
		//    	setEGLConfigChooser
		String product = Build.PRODUCT;
		boolean isEmulator = false;
		if (product != null) {
			isEmulator = product.equals("sdk") || product.contains("_sdk") || product.contains("sdk_") || 
					product.contains ("vbox");
		}
		if(isEmulator)
		{
			glSurfaceView.setEGLConfigChooser(8 , 8, 8, 8, 16, 0);
		}
		else
		{
			glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8); 
		}
		com.litqoo.lib.hspConnector.kInit(this,glSurfaceView,getApplicationContext());
		if (Build.VERSION.SDK_INT >= 19){
			glSurfaceView.setOnSystemUiVisibilityChangeListener(new OnSystemUiVisibilityChangeListener() {
	            @Override
	            public void onSystemUiVisibilityChange(int visibility) {
	            	hideSystemUI(); // Needed to avoid exiting immersive_sticky when keyboard is displayed
	            }
	          });
		}
		
		hideSystemUI();
		return glSurfaceView;
	}
	@Override
	protected void onResume()
	{
		super.onResume();     
		GameAnalytics.traceActivation(this);
		
		hideSystemUI();
		IgawCommon.startSession(DGproto.this);
		//IgawCommon.startSession(DGproto.this);
		//         ADBrixManager.startSession(this);
		HSPCore core = HSPCore.getInstance();
		//         if (HSPCore.getInstance().getState() != HSPState.HSP_STATE_INIT
		//         		&& HSPCore.getInstance().getState() != HSPState.HSP_STATE_ONLINE) 
		if (HSPCore.getInstance().getState() != HSPState.HSP_STATE_INIT)
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

											// ??????????????????????????? ??????????????????????????? ���������������������������������������������������������������������������������?????????????????���������������������������������������������������������������������������������??��������������뀐옙占썲��占�? ????????????????????????���������������������������������������占�?????????????????????��������������뀐옙占썲��占�?.
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
		IgawCommon.endSession();
		GameAnalytics.traceDeactivation(this);
		suspend();
	}
	@Override
	public void onDestroy() {
		super.onDestroy();
		android.os.Process.killProcess(android.os.Process.myPid());
	}
	protected void onActivityResult(int requestCode, int resultCode, Intent data){
		super.onActivityResult(requestCode, resultCode, data);
		hideSystemUI();
		//com.litqoo.lib.hspConnector.onActivityResult(requestCode, resultCode, data, this);
	}
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
	@Override
	public boolean dispatchGenericMotionEvent(MotionEvent ev) {
		// TODO Auto-generated method stub
		return super.dispatchGenericMotionEvent(ev);
	}
	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {
		// TODO Auto-generated method stub
		return super.dispatchKeyEvent(event);
	}


}



