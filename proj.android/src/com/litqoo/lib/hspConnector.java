//package com.litqoo.lib;
//
//import java.util.ArrayList;
//import java.util.HashMap;
//import java.util.Locale;
//import java.util.Map;
//
//import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
//import org.json.JSONArray;
//import org.json.JSONException;
//import org.json.JSONObject;
//
//import android.app.Activity;
//import android.content.Context;
//import android.content.Intent;
//import android.content.SharedPreferences;
//import android.content.pm.ApplicationInfo;
//import android.content.res.AssetManager;
//import android.os.Build;
//import android.text.TextUtils;
//import android.util.Base64;
//import android.util.Log;
//
//import com.kakao.api.Kakao;
//import com.kakao.api.KakaoResponseHandler;
//import com.kakao.api.Logger;
//import com.kakao.api.StringKeySet;
//import com.kakao.api.Kakao.KakaoTokenListener;
//import com.kakao.api.Kakao.LogLevel;
////import com.kakao.api.KakaoLeaderboard;
//import android.os.Handler;
//
//import com.hangame.hsp.HSPCore;
//import com.hangame.hsp.HSPResult;
//import com.hangame.hsp.HSPMyProfile;
//import com.hangame.hsp.HSPServiceProperties;
//
//abstract class KRunnable implements Runnable
//{
//	protected final String totalSource;
//	protected final int delekey;
//	KRunnable(int key,String str)
//	{
//		totalSource = str;
//		delekey = key;
//	}
//}
//
//public class hspConnector{
//	//public static KakaoLeaderboard kakaoLeaderboard;
//	public static Kakao kakao;
//	public static Cocos2dxGLSurfaceView mGLView;
//	public static Context AppContext;
//	public static Handler handler = new Handler();
//	public static String CLIENT_ID = "89862538362910992";
//	public static String CLIENT_SECRET = "u8P9b6Le4rUi14q9dfdVhiwJMlyrNO/CElX96drdLPwXWFrUVEbcw4Ke45Ug47krM7yCMIQuogza3sJqvnzlkQ==";
//	public static String CLIENT_REDIRECT_URI = "kakao" + CLIENT_ID + "://exec";
//	public static final String PREF_KEY = "test.kakao.auth.pref.key";	
//	
////	private static KakaoResponseHandler loginResponseHandler;
//	
//	// ===========================================================
//	// Constants
//	// ===========================================================
//	private static final String PREFS_NAME = "Cocos2dxPrefsFile";
//
//	// ===========================================================
//	// Fields
//	// ===========================================================
//
//	private static AssetManager sAssetManager;
//	private static boolean sAccelerometerEnabled;
//	private static String sPackageName;
//	private static String sFileDirectory;
//	private static Context sContext = null;
//
//	// ===========================================================
//	// Constructors
//	// ===========================================================
//
//	public static void kInit(final Context pContext,Cocos2dxGLSurfaceView _mGLView,Context aContext) {
//		final ApplicationInfo applicationInfo = pContext.getApplicationInfo();
//		hspConnector.mGLView = _mGLView;
//		hspConnector.sContext = pContext;
//		hspConnector.AppContext=aContext;
//		hspConnector.sPackageName = applicationInfo.packageName;
//		hspConnector.sFileDirectory = pContext.getFilesDir().getAbsolutePath();
//		hspConnector.sAssetManager = pContext.getAssets();
//		hspConnector.kakao = hspConnector.getKakao(aContext);
//		
//		//kakaoLeaderboard = KakaoLeaderboard.getInstance();
//		
//	
//		//asdfasdfssdfsdfasdfssdfsfsdf
//		
//		
////		hspConnector.loginResponseHandler = new KakaoResponseHandler(pContext) {
////
////			@Override
////			public void onStart() {
////				super.onStart();
////				Log.i("litqoo","onstart");
////			}
////			
////			@Override
////			public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////
////				Log.i("litqoo","oncomplete" + result.toString());
////			}
////
////			@Override
////			public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////				Log.i("litqoo","onerror" + result.toString());
////			}
////
////		};
//	}
//
//	// ===========================================================
//	// Getter & Setter
//	// ===========================================================
//	public static Kakao getKakao(Context context) {
//		if (kakao != null) {
//			return kakao;
//		}
//		
//		try {
//			// using device id that was generated unique id
//			kakao = new Kakao(context, CLIENT_ID, CLIENT_SECRET, CLIENT_REDIRECT_URI);
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
//		
//		// 諛고��� Release濡�蹂�꼍.
//		kakao.setLogLevel(LogLevel.Debug);
//		
//		final SharedPreferences pref = context.getSharedPreferences(PREF_KEY, Context.MODE_PRIVATE);
//
//        KakaoTokenListener tokenListener = new KakaoTokenListener() {
//            public void onSetTokens(String accessToken, String refreshToken) {
//                // �ㅼ� ��������蹂댁���蹂댁���� 怨녹� ����댁� �⑸��� 
//                if(TextUtils.isEmpty(accessToken) || TextUtils.isEmpty(refreshToken)) { // accessToken��null�닿굅��鍮���몃�("")�대㈃ 濡�렇���
//                	// ����대��������吏����
//                    pref.edit().remove("access_token").remove("refresh_token").commit();
//
//                    // TODO kakaoStatus媛�STATUS_INVALID_GRANT �닿굅�� KakaoTokenListener��access token��null ��� 鍮�Ц���濡��ㅻ�寃쎌� logout | �댄� �ъ���| 醫��
//                } else {
//                    pref.edit().putString("access_token", accessToken).putString("refresh_token", refreshToken).commit();
//                }
//            }
//        };
//        kakao.setTokenListener(tokenListener);
//
//        String accessToken = pref.getString("access_token", null);
//        String refreshToken = pref.getString("refresh_token", null);
//        kakao.setTokens(accessToken, refreshToken);
//		
//		return kakao;
//	}
//	
//	
//	public static void onActivityResult(int requestCode, int resultCode, Intent data,Activity activity) {
//		KakaoResponseHandler loginResponseHandler = new KakaoResponseHandler(hspConnector.sContext){
//
//			@Override
//			protected void onComplete(int arg0, int arg1, JSONObject arg2) {
//				// TODO Auto-generated method stub
//				Log.i("litqoo","onactivity:"+arg2.toString());
//			}
//
//			@Override
//			protected void onError(int arg0, int arg1, JSONObject arg2) {
//				// TODO Auto-generated method stub
//				Log.i("litqoo","onactivity:"+arg2.toString());			
//			}
//			
//		};
//		
//		kakao.onActivityResult(requestCode, resultCode, data, activity, loginResponseHandler);
//	}
//	// ===========================================================
//	// Methods for/from SuperClass/Interfaces
//	// ===========================================================
//	private static native void ResultLogin(int _key,String datas,boolean isFinish);
//	private static native void SendResultNative(int _key,String datas,boolean isFinish);
//	private static native void SetupOnAndroid(int gameno,String gameid,String gameVersion);
//
//	private static void SendResult(int _key,String datas){
//		int size = datas.length();
//		String source = datas;
//		boolean isFinish = true;
//		if(size<200){
//			hspConnector.SendResultNative(_key,source,isFinish);
//		}else{
//			int index=0;
//			isFinish=false;
//			int sublen = 200;
//			while(isFinish==false){
//				if(size<index+sublen){sublen = size-index; isFinish=true;}
//				hspConnector.SendResultNative(_key,source.substring(index, index+sublen),isFinish);
//				index+=sublen;
//			}
//		}
//		
//		
//	}
//	// ===========================================================
//	// Methodskakao.hasTokens()
//	// ===========================================================
//	
//	public static boolean kIsLogin(){
//		Log.i("litqoo","isLogin");
//		Log.i("litqoo","go to handler.post");
//		if(kakao.hasTokens()){
//			return true;
//		}else{
//			return false;
//		}
//	}
//	
//	public static long getHSPMemberNo(){
//		return HSPCore.getInstance().getMemberNo();
//	}
//	public static void login(final int _key, final boolean manualLogin){
//		
////		hspConnector.handler.post(
////				new Runnable(){
////					public void run() {
////		
////		Activity activity=(Activity)hspConnector.sContext;
////		HSPCore core = HSPCore.getInstance();
////		if(core!=null){
////		Log.i("com.litqoo.dgproto", "hspcore create ok2");
////		
////		
////		core.login(activity,manualLogin,new HSPCore.HSPLoginCB() {
////			
////			public void onLogin(final HSPResult result, boolean isPlayable) {
////                Log.d("litqoo", "BEGIN - HSPLoginCB");
////                JSONObject r= new JSONObject();
////                JSONObject error = new JSONObject();
////                try {
////					r.put("playable", isPlayable);
////	                error.put("code", result.getCode());
////	                error.put("isSuccess", result.isSuccess());
////	                error.put("localizedDescription", result.getDetail());
////	                r.put("error", error);
////				} catch (JSONException e) {
////					
////				}
////
////                
////				mGLView.queueEvent(new KRunnable(_key,r.toString()) {
////	                public void run() {
////
////	                    Log.d("litqoo", "login id:"+HSPCore.getInstance().getMemberID()+"/no:"+HSPCore.getInstance().getMemberNo());
////	                    Log.d("litqoo", "SendResult"+this.totalSource);
////	                	hspConnector.SendResult(this.delekey,this.totalSource);
////	                }
////	            });
////
////        		
////				
////			}
////		});
////		}else{
////			Log.d("litqoo","!!!!!!!! need setup !!!!!!");
////		}
////		
////		
////					}
////				}
////				);
//	}
//	
//
//	public static boolean setup(int gameno,String gameid,String gameVersion){
//		
////		if(HSPCore.getInstance()==null){
////			hspConnector.SetupOnAndroid(gameno, gameid, gameVersion);
////			return HSPCore.createInstance(hspConnector.sContext, gameno, gameid, gameVersion);
////		}
////		return false;
//		return false;
//	}
//	
//	public static void kLogin(final int _key){
//		Log.i("litqoo","start login");
//		
//
//		Log.i("litqoo","go to handler.post");
//		
//		hspConnector.handler.post(
//				new Runnable(){
//					public void run() {
//						KakaoResponseHandler loginResponseHandler = new KakaoResponseHandler(hspConnector.sContext) {
//							@Override
//							public void onStart() {
//								super.onStart();
//								Log.i("litqoo","start ok");
//							}
//							
//							@Override
//							public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
//								Log.i("litqoo","oncomplete ok");
//								mGLView.queueEvent(new KRunnable(_key,result.toString()) {
//					                public void run() {
//					                	hspConnector.SendResult(this.delekey,this.totalSource);
//					                }
//					            });
//								Log.i("litqoo","oncomplete" + result.toString());
//							}
//
//							@Override
//							public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
//								Log.i("litqoo","onerror ok");
//								mGLView.queueEvent(new KRunnable(_key,result.toString()) {
//					                public void run() {
//					                	hspConnector.SendResult(this.delekey,this.totalSource);
//					                }
//					            });
//								Log.i("litqoo","onerror" + result.toString());
//							}
//
//						};
//						Log.i("litqoo","in runnable ok");
//						kakao.login((Activity)hspConnector.sContext, loginResponseHandler);
//					}
//		});
//		
//		
//	}
//	
//	public static void kLoadLocalUser(final int _key){
//		Log.i("litqoo","kLoadLocalUser jni");
//		hspConnector.handler.post(
//		new Runnable(){
//			public void run() {
//					kakao.localUser(new KakaoResponseHandler(hspConnector.AppContext) {
//
//					@Override
//					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
//						Log.i("litqoo","oncomplete ok");
//						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
//			                public void run() {
//			                	hspConnector.SendResult(this.delekey,this.totalSource);
//			                }
//			            });
//						Log.i("litqoo","oncomplete" + result.toString());
//					}
//
//					@Override
//					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
//						Log.i("litqoo","oncomplete ok");
//						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
//			                public void run() {
//			                	hspConnector.SendResult(this.delekey,this.totalSource);
//			                }
//			            });
//						Log.i("litqoo","oncomplete" + result.toString());
//					}
//
//				});
//			}
//		}
//		);
//	}
//	
//	public static void kLoadFriends(final int _key){
//		hspConnector.handler.post(
//		new Runnable(){
//			public void run() {
//				kakao.friends(new KakaoResponseHandler(hspConnector.sContext) {
//					@Override
//					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
//						
//						// 移댁뭅�ㅽ� 移�뎄 以��대� �깆� �ъ���� 移�뎄 紐⑸�
//						JSONArray appFriendArray = result.optJSONArray(StringKeySet.app_friends_info);
//						JSONArray friendArray = result.optJSONArray(StringKeySet.friends_info);
//
//						Logger.getInstance().i("friends_count: " + result.optString("friends_count"));
//
//						Logger.getInstance().i("-------- app friends");
//						for (int i = 0, n = appFriendArray.length(); i < n; i++) {
//							JSONObject friend = appFriendArray.optJSONObject(i);
//							if (friend != null) {
//								try {
//									friend.put("user_id",friend.optString("user_id"));
//								} catch (JSONException e) {
//									// TODO Auto-generated catch block
//									e.printStackTrace();
//								}
//							}
//						}
//						for (int i = 0, n = friendArray.length(); i < n; i++) {
//							JSONObject friend = friendArray.optJSONObject(i);
//							if (friend != null) {
//								try {
//									friend.put("user_id",friend.optString("user_id"));
//								} catch (JSONException e) {
//									// TODO Auto-generated catch block
//									e.printStackTrace();
//								}
//							}
//						}
//						
//						Log.i("litqoo","oncomplete" + result.toString());
//						Log.i("litqoo","oncomplete" + result.toString().length());
//						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
//			                public void run() {
//			                	hspConnector.SendResult(this.delekey,this.totalSource);
//			                }
//			            });
//						Log.i("litqoo","oncomplete" + result.toString());
//					}
//
//					@Override
//					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
//						Log.i("litqoo","oncomplete ok");
//						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
//			                public void run() {
//			                	hspConnector.SendResult(this.delekey,this.totalSource);
//			                }
//			            });
//						Log.i("litqoo","oncomplete" + result.toString());
//					}
//					
//				});
//			}
//		}
//		);
//	}
//
//	public static void kSendMessage(final int _key,final String paramString){
//		hspConnector.handler.post(
//		new Runnable(){
//			public void run() {
//				
//				
//				JSONObject param = null;
//				try {
//					param = new JSONObject(paramString);
//				} catch (JSONException e) {
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				}
//				
//				ArrayList<Map<String, String>> metaInfo = new ArrayList<Map<String, String>>();
//				
//				// excute url��kakao + client_id + :// 怨�媛�� ����쇰� 留���댁����.
//				// 移댁뭅�ㅽ���� ���깆� �ㅽ����湲������AndroidManifest.xml��custom scheme���ㅼ��댁����.
//				Map<String, String> metaInfoAndroid = new HashMap<String, String>();
//				metaInfoAndroid.put("os", "android");
//				metaInfoAndroid.put("executeurl", "");
//				metaInfo.add(metaInfoAndroid);
//
//				Map<String, String> metaInfoIos = new HashMap<String, String>();
//				metaInfoIos.put("os", "ios");
//				metaInfoIos.put("executeurl", "");
//				metaInfo.add(metaInfoIos);
//				
//					kakao.sendMessage(hspConnector.sContext, new KakaoResponseHandler(hspConnector.sContext) {
//
//						@Override
//						public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
//							Log.i("litqoo","oncomplete ok");
//							mGLView.queueEvent(new KRunnable(_key,result.toString()) {
//					            public void run() {
//					            	hspConnector.SendResult(this.delekey,this.totalSource);
//					            }
//					        });
//							Log.i("litqoo","oncomplete" + result.toString());
//						}
//
//						@Override
//						public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
//						
//							Log.i("litqoo","oncomplete ok");
//							mGLView.queueEvent(new KRunnable(_key,result.toString()) {
//					            public void run() {
//					            	hspConnector.SendResult(this.delekey,this.totalSource);
//					            }
//					        });
//							Log.i("litqoo","oncomplete" + result.toString());
//							
//						}
//
//					}, param.optString("receiver_id"), false, param.optString("message"), metaInfo);
//
//				
//			}
//		}
//		);
//	}
//	
//	public static void kLogout(final int _key){
//		hspConnector.handler.post(
//		new Runnable(){
//			public void run() {
//				kakao.logout(new KakaoResponseHandler(hspConnector.sContext) {
//
//					@Override
//					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
//						Log.i("litqoo","oncomplete ok");
//						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
//			                public void run() {
//			                	hspConnector.SendResult(this.delekey,this.totalSource);
//			                }
//			            });
//						Log.i("litqoo","oncomplete" + result.toString());
//					}
//
//					@Override
//					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
//						Log.i("litqoo","oncomplete ok");
//						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
//			                public void run() {
//			                	hspConnector.SendResult(this.delekey,this.totalSource);
//			                }
//			            });
//						Log.i("litqoo","oncomplete" + result.toString());
//					}
//					
//				});
//			}
//		}
//		);
//	}
//	public static void kUnregister(final int _key){
//		hspConnector.handler.post(
//		new Runnable(){
//			public void run() {
//				kakao.unregister(new KakaoResponseHandler(hspConnector.sContext) {
//					
//					@Override
//					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
//						Log.i("litqoo","oncomplete ok");
//						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
//			                public void run() {
//			                	hspConnector.SendResult(this.delekey,this.totalSource);
//			                }
//			            });
//						Log.i("litqoo","oncomplete" + result.toString());
//					}
//					
//					@Override
//					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
//						Log.i("litqoo","oncomplete ok");
//						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
//			                public void run() {
//			                	hspConnector.SendResult(this.delekey,this.totalSource);
//			                }
//			            });
//						Log.i("litqoo","oncomplete" + result.toString());
//					}
//					
//				});
//				
//			}
//		}
//		);
//	}
//
//	
//	public static void kGetOldUser(final int _key){
//		hspConnector.handler.post(
//		new Runnable(){
//			public void run() {
//				
//				
//			}
//		}
//		);
//	}
//	public static void kShowStoryView(final int _key){
//		hspConnector.handler.post(
//		new Runnable(){
//			public void run() {
//				
//				
//			}
//		}
//		);
//	}
//	
////	public static void loadGameInfo(final int _key){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				kakaoLeaderboard.loadGameInfo(new KakaoResponseHandler(hspConnector.sContext) {
////					@Override
////					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////						
////	
////					}
////					
////					@Override
////					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////						
////					}
////				});
////			}
////		}
////		);
////	}
////	
////	public static void loadGameMe(final int _key){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				
////				kakaoLeaderboard.loadGameMe(new KakaoResponseHandler(hspConnector.sContext) {
////
////					@Override
////					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////						
////						Logger.getInstance().i("-------- User Info");
////						Logger.getInstance().i("user_id: " + result.optString("user_id")); 
////						Logger.getInstance().i("nickname: " + result.optString("nickname"));
////						Logger.getInstance().i("profile_image_url: " +  result.optString("profile_image_url"));
////						Logger.getInstance().i("heart: " +  result.optString("heart"));
////						Logger.getInstance().i("exp: " + result.optString("exp"));
////						Logger.getInstance().i("heart_regen_starts_at: " + result.optString("heart_regen_starts_at"));
////
////						String publicData = result.optString("public_data");
////						if( publicData!=null ){
////							Logger.getInstance().i("public_data : after- " + publicData);
////							byte[] decoded = Base64.decode(publicData, 0);
////							publicData = new String(decoded);
////							Logger.getInstance().i("public_data: before-" + publicData);
////						}
////						
////						String privateData = result.optString("private_data");
////						if( privateData!=null ){
////							Logger.getInstance().i("private_data : after- " + privateData);
////							byte[] decoded = Base64.decode(privateData, 0);
////							privateData = new String(decoded);
////							Logger.getInstance().i("private_data: before-" + privateData);
////						}
////						
////						Logger.getInstance().i("message_count: " + result.optString("message_count"));
////
////						
////						JSONArray scores = result.optJSONArray("scores");
////						
////						Logger.getInstance().i("-------- Score Info");
////						try {
////							for (int i = 0; i < scores.length(); i++) {
////								JSONObject score = scores.getJSONObject(i);
////								Logger.getInstance().i("leaderboard_key: " + score.optString("leaderboard_key"));
////								Logger.getInstance().i("season_score: " + score.optString("season_score"));
////								Logger.getInstance().i("last_season_score: " + score.optString("last_season_score"));
////								Logger.getInstance().i("last_score: " + score.optString("last_score"));
////								Logger.getInstance().i("best_score: " + score.optString("best_score"));
////							}
////						} catch (Exception e) {
////							e.printStackTrace();
////						}
////						
////								
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////						
////
////					}
////
////					@Override
////					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////
////				});
////				
////			}
////		}
////		);
////	}
////	
////	public static void loadGameFriends(final int _key){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				 
////				kakaoLeaderboard.loadGamefriends(new KakaoResponseHandler(hspConnector.sContext) {
////					@Override
////					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////
////								
////						// 移댁뭅�ㅽ� 移�뎄 以��대� �깆� �ъ���� 移�뎄 紐⑸�
////						JSONArray appFriendArray = result.optJSONArray(StringKeySet.app_friends);
////						JSONArray friendArray = result.optJSONArray(StringKeySet.friends);
////
////						Logger.getInstance().i("friends_count: " + result.optString("friends_count"));
////
////						Logger.getInstance().i("-------- app friends");
////						for (int i = 0, n = appFriendArray.length(); i < n; i++) {
////							JSONObject friend = appFriendArray.optJSONObject(i);
////							if (friend != null) {
////								try {
////									friend.put("user_id",friend.optString("user_id"));
////								} catch (JSONException e) {
////									// TODO Auto-generated catch block
////									e.printStackTrace();
////								}
////								
////								Logger.getInstance().i("-------- app_friends[" + i +"]");
////								Logger.getInstance().i("rank: " + friend.optString("rank"));
////								Logger.getInstance().i("user_id: " + friend.optString("user_id"));
////								Logger.getInstance().i("nickname: " + friend.optString("nickname"));
////								Logger.getInstance().i("profile_image_url: " + friend.optString("profile_image_url"));
////								Logger.getInstance().i("friend_nickname: " + friend.optString("friend_nickname")); 
////								Logger.getInstance().i("message_blocked: " + friend.optString("message_blocked"));
////								Logger.getInstance().i("last_message_sent_at: " + friend.optString("last_message_sent_at"));
////								Logger.getInstance().i("season_score: " + friend.optString("season_score"));
////								Logger.getInstance().i("last_season_score: " + friend.optString("last_season_score"));
////								Logger.getInstance().i("last_score: " + friend.optString("last_score"));
////								Logger.getInstance().i("best_score: " + friend.optString("best_score"));
////								Logger.getInstance().i("exp: " + friend.optString("exp"));
////								
////								String publicData = friend.optString("public_data");
////								if( publicData!=null ) {
////									Logger.getInstance().i("public_data : after- " + publicData);
////									byte[] decoded = Base64.decode(publicData, 0);
////									publicData = new String(decoded);
////									Logger.getInstance().i("public_data: before-" + publicData);
////								}
////							}
////						}
////
////						Logger.getInstance().i("-------- friends");
////						for (int i = 0, n = friendArray.length(); i < n; i++) {
////							JSONObject friend = friendArray.optJSONObject(i);
////							if (friend != null) {
////								
////								Logger.getInstance().i("-------- friends[" + i +"]");
////								Logger.getInstance().i("user_id: " + friend.optString("user_id"));
////								Logger.getInstance().i("profile_image_url: " + friend.optString("profile_image_url"));
////								Logger.getInstance().i("nickname: " + friend.optString("nickname"));
////								Logger.getInstance().i("friend_nickname: " + friend.optString("friend_nickname"));
////								Logger.getInstance().i("message_blocked: " + friend.optString("message_blocked"));
////								Logger.getInstance().i("last_message_sent_at: " + friend.optString("last_message_sent_at"));
////								Logger.getInstance().i("supported_device: " + friend.optString("supported_device"));
////							}
////						}
////						
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////
////					}
////
////					@Override
////					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////					
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////						
////					}
////					
////				});
////				
////			}
////		}
////		);
////	}
////	
////	public static void loadLeaderBoard(final int _key,final String paramString){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				
////				JSONObject param = null;
////				try {
////					param = new JSONObject(paramString);
////				} catch (JSONException e) {
////					// TODO Auto-generated catch block
////					e.printStackTrace();
////				}
////				
////				kakaoLeaderboard.loadLeaderboard(new KakaoResponseHandler(hspConnector.sContext) {
////					
////					@Override
////					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////						
////						
////						Logger.getInstance().i("friends_count: " + result.optString("friends_count"));
////						
////						JSONArray appFriendArray = result.optJSONArray(StringKeySet.app_friends);
////						
////						Logger.getInstance().i("-------- app friends");
////						for (int i = 0, n = appFriendArray.length(); i < n; i++) {
////							JSONObject friend = appFriendArray.optJSONObject(i);
////							if (friend != null) {
////								try {
////									friend.put("user_id",friend.optString("user_id"));
////								} catch (JSONException e) {
////									// TODO Auto-generated catch block
////									e.printStackTrace();
////								}
////								Logger.getInstance().i("-------- app_friends[" + i +"]");
////								Logger.getInstance().i("user_id: " + friend.optString("user_id"));
////								Logger.getInstance().i("season_score: " + friend.optString("season_score"));
////								Logger.getInstance().i("last_season_score: " + friend.optString("last_season_score"));
////								Logger.getInstance().i("last_score: " + friend.optString("last_score"));
////								Logger.getInstance().i("best_score: " + friend.optString("best_score"));
////							}
////						}
////						
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////					
////					@Override
////					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////				}, param.optString("leaderboard_key"));
////				
////			}
////		}
////		);
////	}
////	
////	public static void updateResult(final int _key,final String paramString){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				
////				JSONObject param = null;
////				try {
////					param = new JSONObject(paramString);
////				} catch (JSONException e) {
////					// TODO Auto-generated catch block
////					e.printStackTrace();
////				}
////				
////				String leaderboardKey=param.optString("leaderboard_key");
////				Integer score=param.optInt("score");
////				Integer exp=param.optInt("exp");
////				byte[] publicData=null;
////				byte[] privateData=null;
////				if(param.optString("public_data").length()>0)publicData=param.optString("public_data").getBytes();
////				if(param.optString("private_data").length()>0)privateData=param.optString("private_data").getBytes();
////				
////				kakaoLeaderboard.updateResult(new KakaoResponseHandler(hspConnector.AppContext) {
////					
////					@Override
////					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////					
////					@Override
////					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////				}, leaderboardKey, score, exp, publicData, privateData);
////				
////			}
////		}
////		);
////	}
////	
////	public static void updateMe(final int _key,final String paramString){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				
////				JSONObject param = null;
////				try {
////					param = new JSONObject(paramString);
////				} catch (JSONException e) {
////					// TODO Auto-generated catch block
////					e.printStackTrace();
////				}
////				
////				Integer currentHeart=param.optInt("currentHeart");
////				Integer heart=param.optInt("heart");
////				byte[] publicData=null;
////				byte[] privateData=null;
////				if(param.optString("public_data").length()>0)publicData=param.optString("public_data").getBytes();
////				if(param.optString("private_data").length()>0)privateData=param.optString("private_data").getBytes();
////				
////				
////				
////				
////				kakaoLeaderboard.updateMe(new KakaoResponseHandler(hspConnector.sContext) {
////					
////					@Override
////					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////					
////					@Override
////					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////				}, heart, currentHeart, publicData, privateData);
////			}
////		}
////		);
////	}
////	
////	public static void sendGameMessage(final int _key,final String paramString){
////		Log.i("litqoo","sendGameMessage");
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				JSONObject param = null;
////				try {
////					param = new JSONObject(paramString);
////				} catch (JSONException e) {
////					// TODO Auto-generated catch block
////					e.printStackTrace();
////				}
////				
////				String userId = param.optString("receiver_id");
////				String msg = param.optString("msg");
////				String game_msg = param.optString("game_msg");
////				Integer heart = param.optInt("heart");
////				byte[] data = null;
////				//if(param.optString("receiver_id").length()>0)data=param.optString("receiver_id").getBytes();
////				Log.i("litqoo","sendgamemessage"+userId+"msg:"+msg+"game_msg:"+game_msg+"heart:"+heart);
////				//Log.i("litqoo","data:"+data.toString());
////				ArrayList<Map<String, String>> metaInfo = new ArrayList<Map<String,String>>();
////				
////				// excute url��kakao + client_id + :// 怨�媛�� ����쇰� 留���댁����.
////				// 移댁뭅�ㅽ���� ���깆� �ㅽ����湲������AndroidManifest.xml��custom scheme���ㅼ��댁����.
////				Map<String, String> metaInfoAndroid = new HashMap<String, String>();
////				metaInfoAndroid.put("os", "android");
////				metaInfoAndroid.put("executeurl", "");
////				metaInfo.add(metaInfoAndroid);
////
////				Map<String, String> metaInfoIos = new HashMap<String, String>();
////				metaInfoIos.put("os", "ios");
////				metaInfoIos.put("executeurl", "");
////				metaInfo.add(metaInfoIos);
////				
////				if(hspConnector.sContext==null){
////					Log.i("litqoo","sContext is null");
////				}else{
////					Log.i("litqoo","sContext is ok");
////				}
////				Log.i("litqoo","sContext:"+hspConnector.sContext.toString());
////				Log.i("litqoo","aContext:"+hspConnector.AppContext.toString());
////				kakaoLeaderboard.sendGameMessage(hspConnector.AppContext, new KakaoResponseHandler(hspConnector.sContext) {
////					
////					@Override
////					protected void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////						// TODO Auto-generated method stub
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////					
////					@Override
////					protected void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////						// TODO Auto-generated method stub
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////				}, userId, false, msg, game_msg, heart, data, metaInfo);
////				
////			}
////		}
////		);
////	}
////	public static void loadGameMessages(final int _key){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				kakaoLeaderboard.loadMessages(new KakaoResponseHandler(hspConnector.sContext) {
////
////					@Override
////					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////
////					@Override
////					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////					
////				});
////				
////				
////			}
////			
////		}
////		);
////
////	}
////	public static void acceptMessage(final int _key,final String paramString){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				JSONObject param = null;
////				try {
////					param = new JSONObject(paramString);
////				} catch (JSONException e) {
////					// TODO Auto-generated catch block
////					e.printStackTrace();
////				}
////
////				Log.i("litqoo","acceptMessage1 :" + paramString);
////				Log.i("litqoo","acceptMessage2 :" + param.optString("ids[]"));
////				kakaoLeaderboard.acceptMessage(new KakaoResponseHandler(hspConnector.sContext) {
////					
////					@Override
////					protected void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////					
////					@Override
////					protected void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////				}, param.optString("ids[]"));
////				
////			}
////		}
////		);
////	}
////	
////	public static void acceptAllMessages(final int _key){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				
////				kakaoLeaderboard.acceptAllMessages(new KakaoResponseHandler(hspConnector.sContext) {
////					
////					@Override
////					protected void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////					
////					@Override
////					protected void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////				});
////				
////			}
////		}
////		);
////	}
////	
////	public static void useHeart(final int _key,final String paramString){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				JSONObject param = null;
////				try {
////					param = new JSONObject(paramString);
////				} catch (JSONException e) {
////					// TODO Auto-generated catch block
////					e.printStackTrace();
////				}
////				
////				kakaoLeaderboard.useHeart(new KakaoResponseHandler(hspConnector.sContext) {
////					
////					@Override
////					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////					
////					@Override
////					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////				}, param.optInt("heart_count"));
////				
////			}
////		}
////		);
////	}
////	
////	public static void messageBlock(final int _key,final String paramString){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				JSONObject param = null;
////				try {
////					param = new JSONObject(paramString);
////				} catch (JSONException e) {
////					// TODO Auto-generated catch block
////					e.printStackTrace();
////				}
////				
////				boolean blocked = false;
////				if(param.optInt("block")==0)blocked=false;
////				else blocked=true;
////				
////				
////				kakaoLeaderboard.blockMessage(new KakaoResponseHandler(hspConnector.sContext) {
////
////					@Override
////					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////
////					@Override
////					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////
////				}, blocked);
////				
////			}
////		}
////		);
////	}
////	
////	public static void sendInviteMessage(final int _key,final String paramString){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				JSONObject param = null;
////				try {
////					param = new JSONObject(paramString);
////				} catch (JSONException e) {
////					// TODO Auto-generated catch block
////					e.printStackTrace();
////				}
////				String userId = param.optString("receiver_id");
////				String message = param.optString("msg");
////				ArrayList<Map<String, String>> metaInfo = new ArrayList<Map<String,String>>();
////				
////				// excute url��kakao + client_id + :// 怨�媛�� ����쇰� 留���댁����.
////				// 移댁뭅�ㅽ���� ���깆� �ㅽ����湲������AndroidManifest.xml��custom scheme���ㅼ��댁����.
////				Map<String, String> metaInfoAndroid = new HashMap<String, String>();
////				metaInfoAndroid.put("os", "android");
////				metaInfoAndroid.put("executeurl", "");
////				metaInfo.add(metaInfoAndroid);
////
////				Map<String, String> metaInfoIos = new HashMap<String, String>();
////				metaInfoIos.put("os", "ios");
////				metaInfoIos.put("executeurl", "");
////				metaInfo.add(metaInfoIos);
////				kakaoLeaderboard.sendInviteMessage(hspConnector.sContext, new KakaoResponseHandler(hspConnector.sContext) {
////					
////					@Override
////					protected void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////						// TODO Auto-generated method stub
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////					
////					@Override
////					protected void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////						// TODO Auto-generated method stub
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////				}, userId, false, message, metaInfo);
////				
////			}
////		}
////		);
////	}
////	
////	public static void deleteMe(final int _key){
////		hspConnector.handler.post(
////		new Runnable(){
////			public void run() {
////				
////				kakaoLeaderboard.deleteMe(new KakaoResponseHandler(hspConnector.sContext) {
////					@Override
////					public void onComplete(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////
////					@Override
////					public void onError(int httpStatus, int kakaoStatus, JSONObject result) {
////						Log.i("litqoo","oncomplete ok");
////						mGLView.queueEvent(new KRunnable(_key,result.toString()) {
////			                public void run() {
////			                	hspConnector.SendResult(this.delekey,this.totalSource);
////			                }
////			            });
////						Log.i("litqoo","oncomplete" + result.toString());
////					}
////				});
////				
////			}
////		}
////		);
////	}
////	
//	
//	public static String getCocos2dxPackageName() {
//		return hspConnector.sPackageName;
//	}
//
//	public static String getCocos2dxWritablePath() {
//		return hspConnector.sFileDirectory;
//	}
//
//	public static String getCurrentLanguage() {
//		return Locale.getDefault().getLanguage();
//	}
//	
//	public static String getDeviceModel(){
//		return Build.MODEL;
//    }
//
//	public static AssetManager getAssetManager() {
//		return hspConnector.sAssetManager;
//	}
//
//
//	public static interface Cocos2dxHelperListener {
//		public void showDialog(final String pTitle, final String pMessage);
//		public void showEditTextDialog(final String pTitle, final String pMessage, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength);
//
//		public void runOnGLThread(final Runnable pRunnable);
//	}
//}
