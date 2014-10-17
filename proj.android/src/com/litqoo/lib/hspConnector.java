package com.litqoo.lib;

import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.TimeZone;
import java.util.UUID;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetManager;
import android.net.Uri;
import android.os.Build;
import android.os.Handler;
import android.telephony.TelephonyManager;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Toast;


//import com.AdX.tag.AdXConnect;
import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPCore.HSPLoginType;
import com.hangame.hsp.HSPCore.HSPWithdrawAccountCB;
import com.hangame.hsp.HSPMappingType;
import com.hangame.hsp.HSPMessage;
import com.hangame.hsp.HSPOAuthProvider;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.HSPResult.HSPResultCode;
import com.hangame.hsp.HSPServiceProperties;
import com.hangame.hsp.HSPServiceProperties.HSPServerName;
import com.hangame.hsp.HSPUtil;
import com.hangame.hsp.HSPUtil.HSPAlertViewWithToastTermsCB;
import com.hangame.hsp.cgp.HSPCGP;
import com.hangame.hsp.cgp.model.PromotionItem;
import com.hangame.hsp.itemdelivery.HSPItemDelivery;
import com.hangame.hsp.payment.HSPPayment;
import com.hangame.hsp.payment.HSPPayment.ProductInfosCB;
import com.hangame.hsp.payment.HSPPaymentProductInfo;
import com.hangame.hsp.ui.HSPUiFactory;
import com.hangame.hsp.ui.HSPUiLauncher;
import com.hangame.hsp.ui.HSPUiUri;
import com.hangame.hsp.ui.HSPUiUri.HSPUiUriParameterKey;
import com.hangame.hsp.ui.HSPUiUri.HSPUiUriParameterValue;
import com.igaworks.adbrix.IgawAdbrix;
import com.nhnent.SKSUMRAN.LuaGLSurfaceView;

//import com.kakao.api.Kakao;
//import com.kakao.api.KakaoResponseHandler;
//import com.kakao.api.Logger;
//import com.kakao.api.StringKeySet;
//import com.kakao.api.Kakao.KakaoTokenListener;
//import com.kakao.api.Kakao.LogLevel;
//import com.kakao.api.KakaoLeaderboard;



public class hspConnector {
	// public static KakaoLeaderboard kakaoLeaderboard;
	// public static Kakao kakao;
	public static String uniqId;
	public static LuaGLSurfaceView mGLView;
	public static Context AppContext;
	public static Handler handler = new Handler();
	public static String CLIENT_ID = "89862538362910992";
	public static String CLIENT_SECRET = "u8P9b6Le4rUi14q9dfdVhiwJMlyrNO/CElX96drdLPwXWFrUVEbcw4Ke45Ug47krM7yCMIQuogza3sJqvnzlkQ==";
	public static String CLIENT_REDIRECT_URI = "kakao" + CLIENT_ID + "://exec";
	public static final String PREF_KEY = "test.kakao.auth.pref.key";

	// private static KakaoResponseHandler loginResponseHandler;

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
	private static Context sContext = null; // DGProto Activity 임.

	// ===========================================================
	// Constructors
	// ===========================================================

	public static void kInit(final Context pContext,
			LuaGLSurfaceView _mGLView, Context aContext) {
		final ApplicationInfo applicationInfo = pContext.getApplicationInfo();
		hspConnector.mGLView = _mGLView;
		hspConnector.sContext = pContext;
		hspConnector.AppContext = aContext;
		hspConnector.sPackageName = applicationInfo.packageName;
		hspConnector.sFileDirectory = pContext.getFilesDir().getAbsolutePath();
		hspConnector.sAssetManager = pContext.getAssets();

		initUniqId();
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================
	private static native void ResultLogin(int _key, String datas,
			boolean isFinish);

	private static native void requestItemDeliveryNative();
	private static native void SendResultNative(int _key, String datas,
			boolean isFinish);

	private static native void SendReactionNative(int _key, String datas,
			boolean isFinish);
	// private static native int getUserState();
	public static native void SetupOnAndroid(int gameno, String gameid,
			String gameVersion);

	public static boolean checkCGP(final int _key) {
		hspConnector.handler.post(new Runnable() {
			public void run() {
				HSPCGP.checkPromotion(hspConnector.sContext,
						new CheckPromotionMapCBImpl(_key, mGLView,
								hspConnector.sContext));
			}
		});
		return true;
	}

	public static boolean purchaseProduct(final int _key, final String productId) {

		hspConnector.handler.post(new Runnable() {
			public void run() {
				HSPPayment.purchase((Activity) hspConnector.sContext,
						productId, new PurchaseCBImpl(_key, mGLView));
				// HSPCGP.checkPromotion(hspConnector.sContext, new
				// CheckPromotionCBImpl(_key, mGLView));
			}
		});
		return true;
	}

	public static void completePromotion() {
		HSPCGP.completePromotion(sContext);
	}

	public static void completeInstallPromotion() {
		HSPCGP.completeInstallPromotion(sContext);
	}
	public static PromotionItem mPromoItem;
	public static PromotionItem getmPromoItem() {
		return mPromoItem;
	}

	public static void setmPromoItem(PromotionItem mPromoItem) {
		hspConnector.mPromoItem = mPromoItem;
	}

	public static void launchPromotion() 
	{
		PromotionItem pi = hspConnector.mPromoItem;
		Log.d("", "dsf");
		HSPCGP.launchPromotion((Activity) sContext, hspConnector.mPromoItem);
		//		HSPCGP.launchPromotion((Activity) sContext);
		//		hspConnector.handler.post(new Runnable() {
		//			public void run() {
		//				HSPCGP.launchPromotion((Activity) sContext, hspConnector.mPromoItem);
		//				mGLView.queueEvent(new Runnable() {
		//					public void run() {
		//					}
		//				});
		//			}
		//		});
	}

	public static void openUrl(final String url) {

		hspConnector.handler.post(new Runnable() {
			public void run() {
				Activity activity = (Activity) hspConnector.sContext;
				Intent market = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
				activity.startActivity(market);

			}
		});

	}

	public static void hspMappingToAccount(final int _key, final int mt,
			final boolean force) {
		// hspConnector.handler.post(
		// new Runnable(){
		// public void run() {

		HSPCore core = HSPCore.getInstance();
		Log.d("before cur State", core.getState().toString());
		if (core != null) {
			// Boolean isOverWriteMapping = true; // true 이면 이미 매핑한 sno를 강제로
			// 매핑시킨다는 의미이다.
			Boolean isOverWriteMapping = Boolean.valueOf(force); // false 이면 이미
			// 매핑한 sno는
			// 건들지 않고
			// 얼럿으로
			// 알려주기만 한다.

			HSPMappingType mt2 = HSPMappingType.values()[mt];
			HSPCore.getInstance().requestMappingToAccount(mt2,
					isOverWriteMapping,
					new HSPCore.HSPReMappingAndMemberNoCB() {

				@Override
				public void onIdpIDMap(HSPResult result,
						long prevMemberNo) {
					HSPCore core = HSPCore.getInstance();
					Log.d("after cur State", core.getState().toString());
					Log.d("mapping",
							"@@@@@@ HSPCore.login callback => "
									+ result);
					JSONObject r = new JSONObject();
					JSONObject error = new JSONObject();
					// 매핑을 성공한 케이스.
					if (result.isSuccess() == true) {
						Toast.makeText(hspConnector.sContext,
								"로그인 성공", Toast.LENGTH_LONG)
								.show();

					} else { // 매핑을 실패한 케이스
						Log.d("mapping",
								"HSP Remapping Failed - error = "
										+ result);
						if (result.getCode() == HSPResultCode.HSP_RESULT_CODE_ALREADY_MAPPED_ACCOUNT_TO_SNO) {

							// 만약 이미 매핑되어 있는 id라고 하면 매핑되어 있는 memberNo를
							// 얻을 수 있다.

							Log.d("TAG", "previous member number : "
									+ prevMemberNo);

						}
						// 상세 에러코드 확인
						Log.d("mapping", "Detail Error code = "
								+ result.getCode() + ", domain = "
								+ result.getDomain() + ", detail = "
								+ result.getDetail());

						//								Toast.makeText(hspConnector.sContext,
						//										" Remapping Failed: " + result,
						//										Toast.LENGTH_LONG).show();
					}
					Log.d("mapping",
							"END - HSPRequestMappingToAccountCB ");

					if (result.isSuccess() == false) {
						// Log.i("litqoo", "HSP Login Error = " +
						// result);

						// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 ���占쏙옙占쏙옙���占�
						// 占쏙옙占쏙옙占쎈��占쏙옙占쏙옙占�.
						int errorCode = result.getCode();
						String errorDescription = result.getDetail();

						// Log.i("litqoo", "code = " + errorCode +
						// ", message = " + errorDescription);
					} else {
						// Log.i("litqoo", "success");
					}

					try {
						r.put("account", mt);
						r.put("prevMemberNo", prevMemberNo);
						error.put("code", result.getCode());
						error.put("isSuccess", result.isSuccess());
						error.put("localizedDescription",
								result.getDetail());
						r.put("error", error);
					} catch (JSONException e) {

					}
					mGLView.queueEvent(new KRunnable(_key, r.toString()) {
						public void run() {
							hspConnector.SendResult(this.delekey,
									this.totalSource);
						}
					});
				}
			});

		} else {
			// 초기화가 되지 않은 경우
			Log.d("mapping", "HSPCore.getInstance() is NULL");
		}
		// }
		// });

	}

	public static void openHSPNotice() {
		HSPUiUri uri = HSPUiFactory
				.getUiUri(HSPUiUri.HSPUiUriAction.SUPPORTS_NOTICE);
		HSPUiLauncher.getInstance().launch(uri);
		// URI에 파라미터를 추가하는 경우 아래와 같이 추가할 수 있습니다.
		uri.setParameter(HSPUiUriParameterKey.TOPBAR_SHOW,
				HSPUiUriParameterValue.TRUE);
		uri.setParameter(HSPUiUriParameterKey.GNB_SHOW,
				HSPUiUriParameterValue.TRUE);
	}

	public static void openHSPUrl(final String url) {
		// 내 정보 URI Action(WEBVIEW)으로 HSPUiUri 인스턴스를 생성한다 HSPUri*
		HSPUiUri uriWebview = HSPUiFactory
				.getUiUri(HSPUiUri.HSPUiUriAction.WEBVIEW);

		// 3. 접속하고자 하는 Web Page Url을 파라미터로 설정한다.

		uriWebview.setParameter(HSPUiUri.HSPUiUriParameterKey.WEB_URL, url);

		// 4. 생성된 HSPUiUri 인스턴스를 이용하여 HSPUiLauncher에 launch 요청을 한다.

		// 게임 위에 해당 HSPUiUri 인스턴스에 해당하는 HSP화면을 출력한다.
		HSPUiLauncher.getInstance().launch(uriWebview);

	}

	public static void openCSCenter(final String url) {
		// 내 정보 URI Action(WEBVIEW)으로 HSPUiUri 인스턴스를 생성한다 HSPUri*
		HSPUiUri uriWebview = HSPUiFactory
				.getUiUri(HSPUiUri.HSPUiUriAction.SUPPORTS_CSCENTER);

		// 3. 접속하고자 하는 Web Page Url을 파라미터로 설정한다.

		// uriWebview.setParameter(HSPUiUri.HSPUiUriParameterKey.WEB_URL, url);

		// 4. 생성된 HSPUiUri 인스턴스를 이용하여 HSPUiLauncher에 launch 요청을 한다.

		// 게임 위에 해당 HSPUiUri 인스턴스에 해당하는 HSP화면을 출력한다.
		HSPUiLauncher.getInstance().launch(uriWebview);

	}

	public static String getServerAddress() {
		HSPServiceProperties properties = HSPCore.getInstance()
				.getServiceProperties();
		String gameServerAddress = properties
				.getServerAddress(HSPServerName.HSP_SERVERNAME_GAMESVR);
		return gameServerAddress;
	}

	public static int openKakaoMsg() {

		/**
		 * @param activity
		 * @param url
		 * @param message
		 * @param appId
		 * @param appVer
		 * @param appName
		 * @param encoding
		 */

		KakaoLink kakaoLink = KakaoLink
				.getLink((Activity) hspConnector.sContext);

		// check, intent is available.
		if (!kakaoLink.isAvailableIntent()) {
			return 0;
		} else {
			hspConnector.handler.post(new Runnable() {
				public void run() {
					KakaoLink kakaoLink = KakaoLink
							.getLink((Activity) hspConnector.sContext);
					//
					// // check, intent is available.
					// if (!kakaoLink.isAvailableIntent())
					// return;
					kakaoLink.openKakaoLink((Activity) hspConnector.sContext,
							"http://hgurl.me/am7",
							"돌아온 오락실의 제왕!!\n땅따먹기 리턴즈 with 섬란카구라 뉴웨이브",
							"com.nhnent.SKSUMRAN", "1.0", "땅따먹기 리턴즈 with 섬란카구라 뉴웨이브",
							"UTF-8");

				}
			});
			return 1;
		}
	}

	public static int sendKakaoMsg(final String title,final String msg,final String url) {

		/**
		 * @param activity
		 * @param url
		 * @param message
		 * @param appId
		 * @param appVer
		 * @param appName
		 * @param encoding
		 */

		KakaoLink kakaoLink = KakaoLink
				.getLink((Activity) hspConnector.sContext);

		// check, intent is available.
		if (!kakaoLink.isAvailableIntent()) {
			return 0;
		} else {
			hspConnector.handler.post(new Runnable() {
				public void run() {
					KakaoLink kakaoLink = KakaoLink
							.getLink((Activity) hspConnector.sContext);
					//
					// // check, intent is available.
					// if (!kakaoLink.isAvailableIntent())
					// return;
					kakaoLink.openKakaoLink((Activity) hspConnector.sContext,
							url,
							msg,
							"com.nhnent.SKSUMRAN", "1.0", title,
							"UTF-8");

				}
			});
			return 1;
		}
	}



	public static void requestProductInfos(final int _key){
		HSPPayment.requestProductInfos(new ProductInfosCB() {

			@Override
			public void onProductInfos(List<HSPPaymentProductInfo> productInfos, HSPResult result) { 
				JSONObject r = new JSONObject();
				JSONArray jArray = new JSONArray();
				try {
//					r.put("issuccess", true);
					if (result.isSuccess()) {
						r.put("issuccess", true);
						for (HSPPaymentProductInfo productInfo : productInfos) {
							JSONObject arrayElement = new JSONObject();
							arrayElement.put("productid", productInfo.getProductID());
							arrayElement.put("productname", productInfo.getProductName());
							arrayElement.put("currency", productInfo.getCurrency());
							arrayElement.put("price", productInfo.getPrice());
							jArray.put(arrayElement);
						}
						r.put("info", jArray);
					} else {
						r.put("issuccess", false);
					}
					mGLView.queueEvent(new KRunnable(_key, r.toString()) {
						public void run() {
							hspConnector.SendResult(this.delekey, this.totalSource);
						}
					});
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} 
			}
		});
	}
	public static void finishItemDelivery(final int _key, final String datas) {
		try {
			// Log.d("finishItemDelivery", datas);
			JSONObject json = new JSONObject(datas);
			JSONArray itemSeqArray = json.getJSONArray("itemsequence");
			ArrayList<Long> itemList = new ArrayList<Long>();
			for (int i = 0; i < itemSeqArray.length(); i++) {
				itemList.add(itemSeqArray.getLong(i));
			}
			boolean result = HSPItemDelivery.finishItemDelivery(
					json.getLong("transactionid"), itemList);

			JSONObject sendMessage = new JSONObject();
			sendMessage.put("issuccess", result == true ? 1 : 0);
			mGLView.queueEvent(new KRunnable(_key, sendMessage.toString()) {
				public void run() {
					hspConnector.SendResult(this.delekey, this.totalSource);
				}
			});
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public static void SendReaction(int _key, String datas) {
		int size = datas.length();
		String source = datas;
		boolean isFinish = true;
		if (size < 200) {
			hspConnector.SendReactionNative(_key, source, isFinish);
		} else {
			int index = 0;
			isFinish = false;
			int sublen = 200;
			while (isFinish == false) {
				if (size < index + sublen) {
					sublen = size - index;
					isFinish = true;
				}
				hspConnector.SendReactionNative(_key,
						source.substring(index, index + sublen), isFinish);
				index += sublen;
			}
		}

	}
	public static void SendResult(int _key, String datas) {
		int size = datas.length();
		String source = datas;
		boolean isFinish = true;
		if (size < 200) {
			hspConnector.SendResultNative(_key, source, isFinish);
		} else {
			int index = 0;
			isFinish = false;
			int sublen = 200;
			while (isFinish == false) {
				if (size < index + sublen) {
					sublen = size - index;
					isFinish = true;
				}
				hspConnector.SendResultNative(_key,
						source.substring(index, index + sublen), isFinish);
				index += sublen;
			}
		}

	}

	// ===========================================================
	// Methodskakao.hasTokens()
	// ===========================================================

	// public static boolean kIsLogin(){
	// Log.i("litqoo","isLogin");
	// Log.i("litqoo","go to handler.post");
	// if(kakao.hasTokens()){
	// return true;
	// }else{
	// return false;
	// }
	// }

	public static long getHSPMemberNo() {
		return HSPCore.getInstance().getMemberNo();
	}

	public static double getScreenRealWidth() {
		DisplayMetrics dm = new DisplayMetrics();
		((WindowManager)AppContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getMetrics(dm);

		if(dm.xdpi>0){
			double x = dm.widthPixels/dm.xdpi;
			return x;
		}else{
			return 4.2f;
		}
	}

	public static double getScreenRealHeight() {
		DisplayMetrics dm = new DisplayMetrics();
		((WindowManager)AppContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getMetrics(dm);

		if(dm.ydpi>0){
			double y = dm.heightPixels/dm.ydpi;
			return y;
		}else{
			return 2.47f;
		}
	}
	public static void AdxLogin()
	{ 
//		AdXConnect.getAdXConnectEventInstance(sContext.getApplicationContext(), "Signup", "", "");
	}
	public static void getAdXConnectEventInstance(String event, String data, String currency)
	{
//		 AdXConnect.getAdXConnectEventInstance(getApplicationContext(), "Sale", "2.50", "USD");
//		AdXConnect.getAdXConnectEventInstance(sContext.getApplicationContext(), event, data, currency);
	}
	public static void IgawAdbrixBuy(String productName)
	{
		IgawAdbrix.buy(productName);
		// AdXConnect.getAdXConnectEventInstance(getApplicationContext(), "Sale", "2.50", "USD");
//		AdXConnect.getAdXConnectEventInstance(sContext.getApplicationContext(), event, data, currency);
	}
	public static void IgawAdbrixRetention(String msg)
	{
		IgawAdbrix.retention(msg);
	}	
	public static void IgawAdbrixFirstTimeExperience(String msg)
	{
		IgawAdbrix.firstTimeExperience(msg);
		// AdXConnect.getAdXConnectEventInstance(getApplicationContext(), "Sale", "2.50", "USD");
//		AdXConnect.getAdXConnectEventInstance(sContext.getApplicationContext(), event, data, currency);
	}
	public static void registerGamePadCallback(final int _key)
	{ 
		mGLView.setCallbackKey(_key);
		//		hspConnector. 
		//		sContext.
	}
	public static void unregisterGamePadCallback(int registeredKey)
	{ 
		mGLView.removeCallbackKey(registeredKey);
		//		hspConnector. 
		//		sContext.
	}
	public static void login(final int _key, final boolean manualLogin,
			final int loginType) {
		Log.d("", "login function");
		hspConnector.handler.post(new Runnable() {
			public void run() {

				Activity activity = (Activity) hspConnector.sContext;
				HSPCore core = HSPCore.getInstance();

				if (core != null) {
					// Log.i("com.litqoo.dgproto", "hspcore create ok2");


					HSPOAuthProvider lType = HSPOAuthProvider.values()[loginType];
					core.login(activity, lType, new HSPCore.HSPLoginCB() {

						public void onLogin(final HSPResult result,
								boolean isPlayable) {
							// Log.d("litqoo", "BEGIN - HSPLoginCB");

							HSPCore core = HSPCore.getInstance();
							JSONObject r = new JSONObject();
							JSONObject error = new JSONObject();

							if (result.isSuccess() == false) {
								// Log.i("litqoo", "HSP Login Error = " +
								// result);

								// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 ���占쏙옙占쏙옙���占�
								// 占쏙옙占쏙옙占쎈��占쏙옙占쏙옙占�.
								int errorCode = result.getCode();
								String errorDescription = result.getDetail();

								// Log.i("litqoo", "code = " + errorCode +
								// ", message = " + errorDescription);
							} else {
								// Log.i("litqoo", "success");
							}

							try {
								r.put("playable", isPlayable);
								error.put("code", result.getCode());
								error.put("isSuccess", result.isSuccess());
								error.put("localizedDescription",
										result.getDetail());
								HSPServiceProperties properties = HSPCore.getInstance().getServiceProperties();
								r.put("serviceprop", properties.getLaunchingState());
								r.put("error", error);
							} catch (JSONException e) {

							}


							mGLView.queueEvent(new KRunnable(_key, r.toString()) {
								public void run() {

									// Log.d("litqoo",
									// "login id:"+HSPCore.getInstance().getMemberID()+"/no:"+HSPCore.getInstance().getMemberNo());
									// Log.d("litqoo",
									// "SendResult"+this.totalSource);
									hspConnector.SendResult(this.delekey,
											this.totalSource);
								}
							});

						}
					});
				} else {
					// Log.d("litqoo","!!!!!!!! need setup !!!!!!");
				}

			}
		});
	}

	public static void mappingToAccount(final int _key) {

		hspConnector.handler.post(new Runnable() {
			public void run() {
				Activity activity = (Activity) hspConnector.sContext;
				HSPCore core = HSPCore.getInstance();

				if (core != null) {
					core.requestMappingToAccount(new HSPCore.HSPRequestMappingToAccountCB() {

						@Override
						public void onIdpIDMap(HSPResult result) {
							// TODO Auto-generated method stub
							JSONObject r = new JSONObject();
							JSONObject error = new JSONObject();
							try {
								error.put("code", result.getCode());
								error.put("isSuccess", result.isSuccess());
								error.put("localizedDescription",
										result.getDetail());
								r.put("error", error);
							} catch (JSONException e) {

							}

							mGLView.queueEvent(new KRunnable(_key, r.toString()) {
								public void run() {
									hspConnector.SendResult(this.delekey,
											this.totalSource);
								}
							});
						}
					});

				}

			}
		});
	}

	public static String getCountryCode() {
		String r = Locale.getDefault().getCountry();
		if(r=="")r="kr";
		return r;
	}

	public static String getTimeZone(){
		TimeZone tz = TimeZone.getDefault();
		if(tz.getID()=="")return"Asia/seoul";
		return tz.getID();
	}

	public static void getIsUsimKorean(final int _key){
		
		
		
		HSPUtil.alertViewWithToastTerms((Activity)hspConnector.sContext, new HSPAlertViewWithToastTermsCB() {

			@Override
			public void onCheckResult(Boolean agreeTrueFalse) {
				// TODO Auto-generated method stub
				//                Log.d("Result : " + agreeTrueFalse);
				JSONObject r= new JSONObject();
				//                JSONObject error = new JSONObject();
				try {
					r.put("korean", !agreeTrueFalse);
					r.put("isSuccess", 1);
					//                	error.put("localizedDescription", result.getDetail());
					//                	r.put("error", error);
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
	public static int getIsEnablePushNotification(){
		return HSPMessage.isEnablePushNotification() == false ? 0 : 1; // : true;		
	}
	public static void setIsEnablePushNotification(int truefalse){
		HSPMessage.setPushNotification(truefalse == 0 ? false : true);	
	}
	public static int getLoginType()
	{
		for(HSPLoginType t: HSPLoginType.values())
		{
			Log.d("enum", t.name() + t.ordinal());
		}
		Log.d("enum", "AA");
		for (HSPMappingType t : HSPMappingType.values()) {
			Log.d("enum", t.name() + t.ordinal());
		}
		HSPLoginType t = HSPCore.getInstance().loginType();
		return t.ordinal();
	}
	public static void withdrawAccount(final int _key)
	{
		hspConnector.handler.post(new Runnable() {
			public void run() { 
				Activity activity = (Activity) hspConnector.sContext;
				HSPCore core = HSPCore.getInstance();

				if (core != null) {
					// Log.i("com.litqoo.dgproto", "hspcore create ok2"); 
					core.withdrawAccount(new HSPWithdrawAccountCB() { 
						@Override
						public void onAccountWithdraw(HSPResult result) {
							// TODO Auto-generated method stub
							JSONObject r = new JSONObject();
							JSONObject error = new JSONObject();
							try {
								error.put("code", result.getCode());
								error.put("isSuccess", result.isSuccess());
								error.put("localizedDescription",
										result.getDetail());
								r.put("error", error);
							} catch (JSONException e) {

							}

							mGLView.queueEvent(new KRunnable(_key, r.toString()) {
								public void run() {
									hspConnector.SendResult(this.delekey,
											this.totalSource);
								}
							}); 
						}
					}); 
				} 
			}
		});	
	}
	public static void logout(final int _key) {
		hspConnector.handler.post(new Runnable() {
			public void run() {

				Activity activity = (Activity) hspConnector.sContext;
				HSPCore core = HSPCore.getInstance();

				if (core != null) {
					// Log.i("com.litqoo.dgproto", "hspcore create ok2"); 
					core.logout(new HSPCore.HSPLogoutCB() {
						@Override
						public void onLogout(HSPResult result) {
							// TODO Auto-generated method stub
							// Log.d("litqoo", "BEGIN - HSPLogoutCB");
							JSONObject r = new JSONObject();
							JSONObject error = new JSONObject();
							try {
								error.put("code", result.getCode());
								error.put("isSuccess", result.isSuccess());
								error.put("localizedDescription",
										result.getDetail());
								r.put("error", error);
							} catch (JSONException e) {

							}

							mGLView.queueEvent(new KRunnable(_key, r.toString()) {
								public void run() {
									hspConnector.SendResult(this.delekey,
											this.totalSource);
								}
							}); 
						} 
					}); 
				} 
			}
		});
	}

	public static boolean setup(int gameno, String gameid, String gameVersion) {
		if (HSPCore.getInstance() == null) {
			hspConnector.SetupOnAndroid(gameno, gameid, gameVersion);

			boolean isCreate = HSPCore.createInstance(hspConnector.sContext,
					gameno, gameid, gameVersion);
			if (!isCreate)
				return false;
		}

		HSPCore.HSPBeforeLogoutListener beforLogoutListener = new HSPCore.HSPBeforeLogoutListener() {

			@Override
			public void onBeforeLogout() {
				// TODO Auto-generated method stub
				// Log.i("litqoo","~~~~~~HSPBeforeLogoutListener");

			}
		};

		HSPCore.HSPBeforeResetAccountListener beforeResetAccountListener = new HSPCore.HSPBeforeResetAccountListener() {

			@Override
			public void onBeforeResetAccount() {
				// TODO Auto-generated method stub
				// Log.i("litqoo","~~~~~~HSPBeforeResetAccountListener");

			}
		};

		HSPCore.getInstance().addBeforeLogoutListener(beforLogoutListener);
		HSPCore.getInstance().addBeforeResetAccountListener(
				beforeResetAccountListener);

		HSPCore.HSPAfterLoginListener afterLoginListener = new HSPCore.HSPAfterLoginListener() {

			@Override
			public void onAfterLogin() {

				//				requestItemDeliveryNative();

				//RequestItemDelivery(); 호출하여 미 배송 아이템 존재 유무 체크
				//아이템 존재 한다면 지급 프로세스 및  FinishItemDelivery();
				// TODO Auto-generated method stub
				// Log.i("litqoo","~~~~~~HSPAfterLoginListener");

			}
		};

		HSPCore.HSPBeforeLoginListener beforeLoginListener = new HSPCore.HSPBeforeLoginListener() {

			@Override
			public void onBeforeLogin() {
				// TODO Auto-generated method stub
				// Log.i("litqoo","~~~~~~HSPBeforeLoginListener");

			}
		};

		HSPCore.HSPAfterLogoutListener afterLogoutListener = new HSPCore.HSPAfterLogoutListener() {

			@Override
			public void onAfterLogout() {
				// TODO Auto-generated method stub
				// Log.i("litqoo","~~~~~~HSPAfterLogoutListener");

			}
		};

		HSPCore.HSPAfterResetAccountListener afterAccountListener = new HSPCore.HSPAfterResetAccountListener() {

			@Override
			public void onAfterResetAccount() {
				// TODO Auto-generated method stub
				// Log.i("litqoo","~~~~~~HSPAfterResetAccountListener");

			}
		};

		HSPCore.getInstance().addAfterLoginListener(afterLoginListener);
		HSPCore.getInstance().addBeforeLoginListener(beforeLoginListener);
		HSPCore.getInstance().addAfterLogoutListener(afterLogoutListener);
		HSPCore.getInstance()
		.addAfterResetAccountListener(afterAccountListener);
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

		HSPMessage.sendPacket(mReceiver, 0, data,
				new HSPMessage.HSPSendPacketCB() {
			@Override
			public void onPacketSend(HSPResult result) {
				if (result.isSuccess()) {
					// Log.i("litqoo","Send Packet has been successful.");
				} else {
					// Log.i("litqoo","Failed to send packet: " +
					// result);
				}
			}
		});
	}

	public static void sendPushMessage() {
		// Log.i("litqoo","java sendPushMessage, go test");
		testSendPushNotification();
	}

	public static void testSendPushNotification() {
		// Send push message to user with memberNo(4105000000131917)
		// 03-29 09:31:58.183: D/cocos2d-x debug info(15545): "hspMemberNo" :
		// 88899626759605474,

		mReceiver = 88899626759605474L;
		Map<String, String> mapData = new LinkedHashMap<String, String>();
		mapData.put("content", mMessage);
		mapData.put("url", "http://www.hangame.com");
		mapData.put("extraData", "疫꿸��占쏙옙占쏙옙怨�占쎈��占쏙옙");

		HSPMessage.sendPushNotification(mReceiver, "test ~!!", mapData,
				new HSPMessage.HSPSendPushNotificationCB() {
			@Override
			public void onPushNotificationSend(HSPResult result) {
				if (result.isSuccess()) {
					// Log.i("litqoo","Send Push message has been successful.");
				} else {
					// Log.i("litqoo","Failed to Send push message. ( "
					// + result + " )");
				}
			}
		});
	}

	public static void testRegisterListener() {

		// Log.i("litqoo","regist message listener");
		// An event that is invoked when a message is received
		mReceiveMessageListener = new HSPMessage.HSPReceiveMessageListener() {
			@Override
			public void onMessageReceive(HSPMessage message) {
				// the actions required when handling incoming message
				// Log.i("litqoo","onMessageReceive");
			}
		};

		HSPMessage.addMessageReceiveListener(mReceiveMessageListener);

		// An event that is invoked when a packet is received
		mReceivePacketListener = new HSPMessage.HSPReceivePacketListener() {
			@Override
			public void onPacketReceive(int gameNo, int type,
					long senderMemberNo, byte[] data) {
				// the actions required when handling incoming packet
				// Log.i("litqoo","onPacketReceive");
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

				// Log.d("nPush", "onPushNotificationReceive :: " +
				// pushData.toString());
				// Log.d("nPush", "id " + pushData.get("id") + " :: badge " +
				// pushData.get("badge") + " :: content "
				// + pushData.get("content"));
			}
		};

		HSPMessage
		.addPushNotificationReceiveListener(mReceivePushNotificationListener);
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

	public static String getDeviceModel() {
		return Build.MODEL;
	}

	public static String getUniqId() {
		return uniqId;
	}

	public static void initUniqId() {
		final TelephonyManager tm = (TelephonyManager) ((Activity) sContext)
				.getBaseContext().getSystemService(Context.TELEPHONY_SERVICE);
		final String tmDevice, tmSerial, androidId;
		tmDevice = "" + tm.getDeviceId();
		tmSerial = "" + tm.getSimSerialNumber();
		androidId = ""
				+ android.provider.Settings.Secure.getString(
						sContext.getContentResolver(),
						android.provider.Settings.Secure.ANDROID_ID);
		UUID deviceUuid = new UUID(androidId.hashCode(),
				((long) tmDevice.hashCode() << 32) | tmSerial.hashCode());
		uniqId = deviceUuid.toString();
	}
	
	public static int getVersionCode()
	{
		PackageInfo pi;
		try {
			pi = AppContext.getPackageManager().getPackageInfo(AppContext.getPackageName(), 0);
		} catch (NameNotFoundException e) {
			return -1;
		}
		
		return pi.versionCode; 
	}

	public static AssetManager getAssetManager() {
		return hspConnector.sAssetManager;
	}

	public static interface Cocos2dxHelperListener {
		public void showDialog(final String pTitle, final String pMessage);

		public void showEditTextDialog(final String pTitle,
				final String pMessage, final int pInputMode,
				final int pInputFlag, final int pReturnType,
				final int pMaxLength);

		public void runOnGLThread(final Runnable pRunnable);
	}
}
