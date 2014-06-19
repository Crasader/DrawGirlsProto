package com.litqoo.lib;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.Locale;
import java.util.Map;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.res.AssetManager;
import android.net.Uri;
import android.os.Build;
import android.os.Handler;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPMessage;
import com.hangame.hsp.HSPOAuthProvider;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.cgp.HSPCGP;
import com.hangame.hsp.itemdelivery.HSPItemDelivery;
import com.hangame.hsp.payment.HSPPayment;

//import com.kakao.api.Kakao;
//import com.kakao.api.KakaoResponseHandler;
//import com.kakao.api.Logger;
//import com.kakao.api.StringKeySet;
//import com.kakao.api.Kakao.KakaoTokenListener;
//import com.kakao.api.Kakao.LogLevel;
//import com.kakao.api.KakaoLeaderboard;


abstract class KRunnable implements Runnable
{
	protected final String totalSource;
	protected final int delekey;
	KRunnable(final int key, String str)
	{
		totalSource = str;
		delekey = key;
	}
}

public class hspConnector{
	//public static KakaoLeaderboard kakaoLeaderboard;
	//public static Kakao kakao;
	public static Cocos2dxGLSurfaceView mGLView;
	public static Context AppContext;
	public static Handler handler = new Handler();
	public static String CLIENT_ID = "89862538362910992";
	public static String CLIENT_SECRET = "u8P9b6Le4rUi14q9dfdVhiwJMlyrNO/CElX96drdLPwXWFrUVEbcw4Ke45Ug47krM7yCMIQuogza3sJqvnzlkQ==";
	public static String CLIENT_REDIRECT_URI = "kakao" + CLIENT_ID + "://exec";
	public static final String PREF_KEY = "test.kakao.auth.pref.key";	

	//	private static KakaoResponseHandler loginResponseHandler;

	// ===========================================================
	// Constants
	// ===========================================================
	private static final String PREFS_NAME = "Cocos2dxPrefsFile";

	// ===========================================================
	// Fields
	// ===========================================================

	private static AssetManager sAssetManager;
	private static boolean sAccelerometerEnabled;
	private static String sPackageName;
	private static String sFileDirectory;
	private static Context sContext = null;

	// ===========================================================
	// Constructors
	// ===========================================================

	public static void kInit(final Context pContext,Cocos2dxGLSurfaceView _mGLView,Context aContext) {
		final ApplicationInfo applicationInfo = pContext.getApplicationInfo();
		hspConnector.mGLView = _mGLView;
		hspConnector.sContext = pContext;
		hspConnector.AppContext=aContext;
		hspConnector.sPackageName = applicationInfo.packageName;
		hspConnector.sFileDirectory = pContext.getFilesDir().getAbsolutePath();
		hspConnector.sAssetManager = pContext.getAssets();

	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================
	private static native void ResultLogin(int _key,String datas,boolean isFinish);
	private static native void SendResultNative(int _key,String datas,boolean isFinish);
	public static native void SetupOnAndroid(int gameno,String gameid,String gameVersion);
	public static boolean checkCGP(final int _key)
	{
		hspConnector.handler.post(
				new Runnable(){
					public void run() {
						HSPCGP.checkPromotion(hspConnector.sContext, new CheckPromotionMapCBImpl(_key, mGLView, hspConnector.sContext));
					}
				}
				);
		return true;
	}
	public static boolean purchaseProduct(final int _key, final String productId)
	{
		hspConnector.handler.post(
				new Runnable(){
					public void run() {
						HSPPayment.purchase((Activity) hspConnector.sContext, productId, new PurchaseCBImpl(_key, mGLView));
//						HSPCGP.checkPromotion(hspConnector.sContext, new CheckPromotionCBImpl(_key, mGLView));
					}
				}
				);
		return true;
	}
	public static void completePromotion(){
		HSPCGP.completePromotion(sContext);
	}
	public static void completeInstallPromotion(){
		HSPCGP.completeInstallPromotion(sContext);
	}
	public static void launchPromotion(){
		
		hspConnector.handler.post(
				new Runnable(){
					public void run() {
						HSPCGP.launchPromotion((Activity) sContext);	
					}
				}
				);
	}
	public static void openUrl(final String url)
	{
		
		
		hspConnector.handler.post(
				new Runnable(){
					public void run() {
						Activity activity=(Activity)hspConnector.sContext;
						Intent market = new Intent(
								Intent.ACTION_VIEW,
								Uri.parse(url)
								);
						activity.startActivity(market);
						
						
					}
				}
				);
		
	}
	
	public static void openKakaoMsg(){

			/**
			 * @param activity
			 * @param url
			 * @param message
			 * @param appId
			 * @param appVer
			 * @param appName
			 * @param encoding
			 */

			
			hspConnector.handler.post(
					new Runnable(){
						public void run() {
							KakaoLink kakaoLink = KakaoLink.getLink((Activity)hspConnector.sContext);

							// check, intent is available.
							if (!kakaoLink.isAvailableIntent())
							  return;
							kakaoLink.openKakaoLink((Activity)hspConnector.sContext, 
							        "http://naver.com", 
							        "남자들을 흥분의 도가니로 몰아 넣은 그게임!!\n땅따먹기 섬란카구라 뉴웨이브", 
							        "com.nhnent.SKDRAWGIRLSA", 
							        "1.0",
							        "땅따먹기 섬란카구라", 
							        "UTF-8");
						
							
						}
					}
					);
	}
	public static void finishItemDelivery(final int _key, final String datas)
	{
		try {
			//Log.d("finishItemDelivery", datas);
			JSONObject json = new JSONObject(datas);
			JSONArray itemSeqArray = json.getJSONArray("itemsequence");
			ArrayList<Long> itemList = new ArrayList<Long>();
			for(int i=0; i<itemSeqArray.length(); i++)
			{
				itemList.add(itemSeqArray.getLong(i));
			}
			boolean result = HSPItemDelivery.finishItemDelivery(json.getLong("transactionid"), itemList);
			
			JSONObject sendMessage = new JSONObject();
			sendMessage.put("issuccess", result == true ? 1 : 0);
			mGLView.queueEvent(new KRunnable(_key, sendMessage.toString()) {
				public void run() {
					hspConnector.SendResult(this.delekey,this.totalSource);
				}
			});
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public static void SendResult(int _key,String datas){
		int size = datas.length();
		String source = datas;
		boolean isFinish = true;
		if(size<200){
			hspConnector.SendResultNative(_key,source,isFinish);
		}else{
			int index=0;
			isFinish=false;
			int sublen = 200;
			while(isFinish==false){
				if(size<index+sublen){sublen = size-index; isFinish=true;}
				hspConnector.SendResultNative(_key,source.substring(index, index+sublen),isFinish);
				index+=sublen;
			}
		}


	}
	// ===========================================================
	// Methodskakao.hasTokens()
	// ===========================================================

	//	public static boolean kIsLogin(){
	//		Log.i("litqoo","isLogin");
	//		Log.i("litqoo","go to handler.post");
	//		if(kakao.hasTokens()){
	//			return true;
	//		}else{
	//			return false;
	//		}
	//	}

	public static long getHSPMemberNo(){
		return HSPCore.getInstance().getMemberNo();
	}
	public static void login(final int _key, final boolean manualLogin){

		hspConnector.handler.post(
				new Runnable(){
					public void run() {

						Activity activity=(Activity)hspConnector.sContext;
						HSPCore core = HSPCore.getInstance();

						if(core!=null){
							//Log.i("com.litqoo.dgproto", "hspcore create ok2");



							core.login(activity,HSPOAuthProvider.GUEST,new HSPCore.HSPLoginCB() {

								public void onLogin(final HSPResult result, boolean isPlayable) {
									//Log.d("litqoo", "BEGIN - HSPLoginCB");
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

									try {
										r.put("playable", isPlayable);
										error.put("code", result.getCode());
										error.put("isSuccess", result.isSuccess());
										error.put("localizedDescription", result.getDetail());
										r.put("error", error);
									} catch (JSONException e) {

									}



									mGLView.queueEvent(new KRunnable(_key,r.toString()) {
										public void run() {

											//Log.d("litqoo", "login id:"+HSPCore.getInstance().getMemberID()+"/no:"+HSPCore.getInstance().getMemberNo());
											//Log.d("litqoo", "SendResult"+this.totalSource);
											hspConnector.SendResult(this.delekey,this.totalSource);
										}
									});



								}
							});
						}else{
							//Log.d("litqoo","!!!!!!!! need setup !!!!!!");
						}


					}
				}
				);
	}

	public static void mappingToAccount(final int _key){

		hspConnector.handler.post(
				new Runnable(){
					public void run() {
						Activity activity=(Activity)hspConnector.sContext;
						HSPCore core = HSPCore.getInstance();

						if(core!=null){
							core.requestMappingToAccount(new HSPCore.HSPRequestMappingToAccountCB() {
								
								@Override
								public void onIdpIDMap(HSPResult result) {
									// TODO Auto-generated method stub
									JSONObject r= new JSONObject();
									JSONObject error = new JSONObject();
									try {
										error.put("code", result.getCode());
										error.put("isSuccess", result.isSuccess());
										error.put("localizedDescription", result.getDetail());
										r.put("error", error);
									} catch (JSONException e) {

									}


									mGLView.queueEvent(new KRunnable(_key,r.toString()) {
										public void run() {
											hspConnector.SendResult(this.delekey,this.totalSource);
										}
									});
								}
							});
							
						}
					
					}
				}
				);
	}
	
	public static String getCountryCode(){
		String r = Locale.getDefault().getCountry();
		return r;
	}
	
	public static void logout(final int _key){
		hspConnector.handler.post(
				new Runnable(){
					public void run() {

						Activity activity=(Activity)hspConnector.sContext;
						HSPCore core = HSPCore.getInstance();

						if(core!=null){
							//Log.i("com.litqoo.dgproto", "hspcore create ok2");

							core.logout(new HSPCore.HSPLogoutCB() {
								@Override
								public void onLogout(HSPResult result) {
									// TODO Auto-generated method stub
									//Log.d("litqoo", "BEGIN - HSPLogoutCB");
									JSONObject r= new JSONObject();
									JSONObject error = new JSONObject();
									try {
										error.put("code", result.getCode());
										error.put("isSuccess", result.isSuccess());
										error.put("localizedDescription", result.getDetail());
										r.put("error", error);
									} catch (JSONException e) {

									}


									mGLView.queueEvent(new KRunnable(_key,r.toString()) {
										public void run() {
											hspConnector.SendResult(this.delekey,this.totalSource);
										}
									});

								}

							});

						}

					}
				});
	}

	public static boolean setup(int gameno,String gameid,String gameVersion){
		if(HSPCore.getInstance()==null){
			hspConnector.SetupOnAndroid(gameno, gameid, gameVersion);

			boolean isCreate =  HSPCore.createInstance(hspConnector.sContext, gameno, gameid, gameVersion);
			if(!isCreate)return false;
		}


		HSPCore.HSPBeforeLogoutListener beforLogoutListener = new HSPCore.HSPBeforeLogoutListener() {

			@Override
			public void onBeforeLogout() {
				// TODO Auto-generated method stub
				//Log.i("litqoo","~~~~~~HSPBeforeLogoutListener");

			}
		};

		HSPCore.HSPBeforeResetAccountListener beforeResetAccountListener = new HSPCore.HSPBeforeResetAccountListener() {

			@Override
			public void onBeforeResetAccount() {
				// TODO Auto-generated method stub
				//Log.i("litqoo","~~~~~~HSPBeforeResetAccountListener");

			}
		};

		HSPCore.getInstance().addBeforeLogoutListener(beforLogoutListener);
		HSPCore.getInstance().addBeforeResetAccountListener(beforeResetAccountListener);

		HSPCore.HSPAfterLoginListener afterLoginListener = new HSPCore.HSPAfterLoginListener() {

			@Override
			public void onAfterLogin() {
				// TODO Auto-generated method stub
				//Log.i("litqoo","~~~~~~HSPAfterLoginListener");

			}
		};

		HSPCore.HSPBeforeLoginListener beforeLoginListener = new HSPCore.HSPBeforeLoginListener() {

			@Override
			public void onBeforeLogin() {
				// TODO Auto-generated method stub
				//Log.i("litqoo","~~~~~~HSPBeforeLoginListener");

			}
		};

		HSPCore.HSPAfterLogoutListener afterLogoutListener = new HSPCore.HSPAfterLogoutListener() {

			@Override
			public void onAfterLogout() {
				// TODO Auto-generated method stub
				//Log.i("litqoo","~~~~~~HSPAfterLogoutListener");

			}
		};

		HSPCore.HSPAfterResetAccountListener afterAccountListener = new HSPCore.HSPAfterResetAccountListener() {

			@Override
			public void onAfterResetAccount() {
				// TODO Auto-generated method stub
				//Log.i("litqoo","~~~~~~HSPAfterResetAccountListener");

			}
		};

		HSPCore.getInstance().addAfterLoginListener(afterLoginListener);
		HSPCore.getInstance().addBeforeLoginListener(beforeLoginListener);
		HSPCore.getInstance().addAfterLogoutListener(afterLogoutListener);
		HSPCore.getInstance().addAfterResetAccountListener(afterAccountListener);
		return true;
	}

	private static String mMessage = "Hello~ Message Test";
	private static long mReceiver = 4105000000131917L;
	private static HSPMessage.HSPReceiveMessageListener mReceiveMessageListener;

	private static HSPMessage.HSPReceivePacketListener mReceivePacketListener;
	private static HSPMessage.HSPReceivePushNotificationListener mReceivePushNotificationListener;

	public static void testSendPacket() {
		// Send packet to user with memberNo(4105000000131917)
		byte[] data = mMessage.getBytes();

		HSPMessage.sendPacket(mReceiver, 0, data, new HSPMessage.HSPSendPacketCB() {
			@Override
			public void onPacketSend(HSPResult result) {
				if (result.isSuccess()) {
					//Log.i("litqoo","Send Packet has been successful.");
				} else {
					//Log.i("litqoo","Failed to send packet: " + result);
				}
			}
		});
	}

	public static void sendPushMessage(){
		//Log.i("litqoo","java sendPushMessage, go test");
		testSendPushNotification();
	}
	public static void testSendPushNotification() {
		// Send push message to user with memberNo(4105000000131917)
		//03-29 09:31:58.183: D/cocos2d-x debug info(15545):    "hspMemberNo" : 88899626759605474,

		mReceiver = 88899626759605474L;
		Map<String, String> mapData = new LinkedHashMap<String, String>();
		mapData.put("content", mMessage);
		mapData.put("url", "http://www.hangame.com");
		mapData.put("extraData", "疫꿸��占쏙옙占쏙옙怨�占쎈��占쏙옙");

		HSPMessage.sendPushNotification(mReceiver, "test ~!!", mapData, new HSPMessage.HSPSendPushNotificationCB() {
			@Override
			public void onPushNotificationSend(HSPResult result) {
				if (result.isSuccess()) {
					//Log.i("litqoo","Send Push message has been successful.");
				} else {
					//Log.i("litqoo","Failed to Send push message. ( " + result + " )");
				}
			}
		});
	}

	public static void testRegisterListener() {

		//Log.i("litqoo","regist message listener");
		// An event that is invoked when a message is received
		mReceiveMessageListener = new HSPMessage.HSPReceiveMessageListener() {
			@Override
			public void onMessageReceive(HSPMessage message) {
				// the actions required when handling incoming message
				//Log.i("litqoo","onMessageReceive");
			}
		};

		
		HSPMessage.addMessageReceiveListener(mReceiveMessageListener);

		// An event that is invoked when a packet is received
		mReceivePacketListener = new HSPMessage.HSPReceivePacketListener() {
			@Override
			public void onPacketReceive(int gameNo, int type, long senderMemberNo, byte[] data) {
				// the actions required when handling incoming packet
				//Log.i("litqoo","onPacketReceive");
			}
		};

		HSPMessage.addPacketReceiveListener(mReceivePacketListener);

		// An event that is invoked when a push message is received
		mReceivePushNotificationListener = new HSPMessage.HSPReceivePushNotificationListener() {
			@Override
			public void onPushNotificationReceive(Map<String, Object> pushData) {
				// the actions required when handling incoming push message
				// log("onPushNotificationReceive");
				//
				// log("extradata :: " + extraData.toString());

				//Log.d("nPush", "onPushNotificationReceive :: " + pushData.toString());
				//Log.d("nPush", "id " + pushData.get("id") + " :: badge " + pushData.get("badge") + " :: content "
				//		+ pushData.get("content"));
			}
		};

		HSPMessage.addPushNotificationReceiveListener(mReceivePushNotificationListener);
	}



	public static String getCocos2dxPackageName() {
		return hspConnector.sPackageName;
	}

	public static String getCocos2dxWritablePath() {
		return hspConnector.sFileDirectory;
	}

	public static String getCurrentLanguage() {
		return Locale.getDefault().getLanguage();
	}

	public static String getDeviceModel(){
		return Build.MODEL;
	}

	public static AssetManager getAssetManager() {
		return hspConnector.sAssetManager;
	}

	public static interface Cocos2dxHelperListener {
		public void showDialog(final String pTitle, final String pMessage);
		public void showEditTextDialog(final String pTitle, final String pMessage, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength);

		public void runOnGLThread(final Runnable pRunnable);
	}
}
