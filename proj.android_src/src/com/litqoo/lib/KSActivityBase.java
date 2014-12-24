package com.litqoo.lib;

import java.util.concurrent.Semaphore;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.AlertDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Vibrator;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.webkit.WebView;
import android.widget.FrameLayout;

import com.hangame.hsp.payment.HSPPayment;
import com.igaworks.IgawCommon;
//import com.litqoo.DGproto.R;


@SuppressWarnings("unused")
abstract class LQRunnable implements Runnable
{
	protected final String totalSource;
	protected final int delekey;
	LQRunnable(String str, int k)
	{
		totalSource = str;
		delekey = k;
	}
}

public class KSActivityBase extends Cocos2dxActivity{
	protected Cocos2dxGLSurfaceView mGLView;
	private final Semaphore semaphore = new Semaphore(1);
	private Handler handler = new Handler();
	private WebView browser = null;
	private WebView etcWebView = null;
	private static final int MARKET_GOOGLE = 1;
	private static final int MARKET_TSTORE = 2;
	private final int MARKET_TYPE = MARKET_GOOGLE; // /< default setting.
	private boolean dialogAble = true;
	private int dialogValue = 0;
	protected String deviceId;

	class Size {
		public Size(int w, int h) {
			width = w;
			height = h;
		}

		public int width;
		public int height;
	}
	
	public boolean isExistApp(final String packageName)
	{
		Log.i("cocos", "isExistApp In");
		PackageManager pm = getPackageManager();
		try{
			pm.getApplicationInfo(packageName, PackageManager.GET_META_DATA);
		}
		catch(Exception e){
			return false;
		}
		return true;
	}
	
	public void diaryAppExe(final String memberID, final String diaryCode, final int cardNumber)
	{
		ComponentName compName = new ComponentName("com.litqoo.DgDiary","com.litqoo.DgDiary.DgDiary"); 
	    Intent intent = new Intent(Intent.ACTION_MAIN);
	    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);//|Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
	    intent.addCategory(Intent.CATEGORY_LAUNCHER);
	    intent.setComponent(compName);
	    
	    intent.putExtra("memberID", memberID);
	    intent.putExtra("diaryCode", diaryCode);
	    intent.putExtra("cardNumber", cardNumber);
	    
	    startActivity(intent);
	}
	
	public void diaryAppExeNA(final String memberID, final String diaryCode, final int cardNumber)
	{
		ComponentName compName = new ComponentName("com.litqoo.DgDiary.NA","com.litqoo.DgDiary.NA.DgDiaryNA"); 
	    Intent intent = new Intent(Intent.ACTION_MAIN);
	    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);//|Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
	    intent.addCategory(Intent.CATEGORY_LAUNCHER);
	    intent.setComponent(compName);
	    
	    intent.putExtra("memberID", memberID);
	    intent.putExtra("diaryCode", diaryCode);
	    intent.putExtra("cardNumber", cardNumber);
	    
	    startActivity(intent);
	}
	
	private int getLeftMargin() {
		Size b = getFitScreen();
		DisplayMetrics displayMetrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
		int deviceWidth = displayMetrics.widthPixels;

		return (deviceWidth - b.width) / 2;
	}

	private int getBottomMargin() {
		Size b = getFitScreen();
		DisplayMetrics displayMetrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
		int deviceHeight = displayMetrics.heightPixels;

		return (deviceHeight - b.height) / 2;
	}

	private Size getFitScreen() {
//		DisplayMetrics displayMetrics = new DisplayMetrics();
//		getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
//		int deviceWidth = displayMetrics.widthPixels;
//		int deviceHeight = displayMetrics.heightPixels;
//		double C1 = deviceWidth / 320.;
//		int screenFitWidth, screenFitHeight;
//		if (480 * C1 > deviceHeight) {
//			double C2 = deviceHeight / 480.;
//			screenFitWidth = (int) (320 * C2);
//			screenFitHeight = deviceHeight;
//		} else if (480 * C1 == deviceHeight) {
//			screenFitHeight = deviceHeight;
//			screenFitWidth = deviceWidth;
//		} else {
//			screenFitWidth = deviceWidth;
//			screenFitHeight = (int) (480 * C1);
//		}
//		return new Size(screenFitWidth,screenFitHeight);
		DisplayMetrics displayMetrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
		int deviceWidth = displayMetrics.widthPixels;
		int deviceHeight = displayMetrics.heightPixels;
		double C1 = deviceWidth / 480.;
		int screenFitWidth, screenFitHeight;
		if (320 * C1 > deviceHeight) {
			double C2 = deviceHeight / 320.;
			screenFitWidth = (int) (480 * C2);
			screenFitHeight = deviceHeight;
		} else if (320 * C1 == deviceHeight) {
			screenFitHeight = deviceHeight;
			screenFitWidth = deviceWidth;
		} else {
			screenFitWidth = deviceWidth;
			screenFitHeight = (int) (480 * C1);
		}
		return new Size(screenFitWidth, screenFitHeight);
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stubd
		super.onCreate(savedInstanceState);
		SnowFilelog.initialize(this);
		activity = this;
		
//		final TelephonyManager tm = (TelephonyManager) getBaseContext().getSystemService(Context.TELEPHONY_SERVICE);
//        final String tmDevice, tmSerial, androidId;
//        tmDevice = "" + tm.getDeviceId();
//        tmSerial = "" + tm.getSimSerialNumber();
//        androidId = "" + android.provider.Settings.Secure.getString(getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
//        UUID deviceUuid = new UUID(androidId.hashCode(), ((long)tmDevice.hashCode() << 32) | tmSerial.hashCode());
//        deviceId = deviceUuid.toString();
		
	}
	
	protected String getUDID()
	{
		return deviceId;
	}
	
	protected String getDeviceInfo()
	{
		String dinfo = android.os.Build.VERSION.RELEASE + '/';
		dinfo += android.os.Build.MODEL;
		return dinfo;
	}

	private static Context activity;

	// //////////////////////////////////////////////////////
	public static Object getActivity() {
//		 Log.i("cppCall", "return activity~~~~!!!");
		return activity;
	}
	
	private void showWebView() {
		// Log.i("JAVA", "showWebView");
		getHandler().post(new Runnable() {

			public void run() {
				// TODO Auto-generated method stub

				try {
					semaphore.acquire();
					if (browser != null)
						browser.setVisibility(View.VISIBLE);
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}

	private void moveWebView(final int x, final int y) {
		getHandler().post(new Runnable() {

			public void run() {
				// TODO Auto-generated method stub
				try {
					semaphore.acquire();
					if (browser != null) {
						Size mySize = getFitScreen();
						int left_margin = getLeftMargin();
						int bottom_margin = getBottomMargin();

						double xRatio = mySize.width / 480.;
						double yRatio = mySize.height / 320.;
						// browser.getLayoutParams().width
						// FrameLayout.LayoutParams params = new
						// FrameLayout.LayoutParams( browser.getLayoutParams());
						FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
								browser.getLayoutParams().width, browser
										.getLayoutParams().height, Gravity.TOP);
						params.setMargins((int) (x * xRatio) + left_margin,
								(int) (y * yRatio) + bottom_margin, 0, 0);

						browser.setLayoutParams(params);
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}

	private void showTest()
	{
		
		getHandler().post(new Runnable() {
			public void run() 
			{
				try {
					semaphore.acquire();
				
					//SnowFilelog.write(KSActivityBase.this, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//			        Intent intent = new Intent((Context)KSActivityBase.getActivity(), Dungeons.class);
//			        ((Context)KSActivityBase.getActivity()).startActivity(intent);
			        //startActivity(intent);  
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
			}
		}
		});
	}
	
	private void openApp(final String app_name)
	{
		getHandler().post(new Runnable() {
			public void run() {
				try {
					semaphore.acquire();
					if (MARKET_TYPE == MARKET_GOOGLE) {
						Intent market = new Intent(
								Intent.ACTION_VIEW,
								Uri.parse("market://details?id=" + app_name));
						startActivity(market);
					} else if (MARKET_TYPE == MARKET_TSTORE) {
						Intent intent = new Intent();
						intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP); // mandatory
																			// flag
						intent.setClassName("com.skt.skaf.A000Z00040",
								"com.skt.skaf.A000Z00040.A000Z00040");
						intent.setAction("COLLAB_ACTION"); // action
						intent.putExtra("com.skt.skaf.COL.URI",
								"PRODUCT_VIEW/0000297141/0".getBytes()); // user
																			// data
						intent.putExtra("com.skt.skaf.COL.REQUESTER",
								"A000Z00040"); // my App ID
						startActivity(intent);
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}
	
	private void showReview() {
		getHandler().post(new Runnable() {
			public void run() {
				try {
					semaphore.acquire();
					if (MARKET_TYPE == MARKET_GOOGLE) {
						Intent market = new Intent(
								Intent.ACTION_VIEW,
								Uri.parse("market://details?id=com.litqoo.DrawingJack"));
						startActivity(market);
					} else if (MARKET_TYPE == MARKET_TSTORE) {
						Intent intent = new Intent();
						intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP); // mandatory
																			// flag
						intent.setClassName("com.skt.skaf.A000Z00040",
								"com.skt.skaf.A000Z00040.A000Z00040");
						intent.setAction("COLLAB_ACTION"); // action
						intent.putExtra("com.skt.skaf.COL.URI",
								"PRODUCT_VIEW/0000297141/0".getBytes()); // user
																			// data
						intent.putExtra("com.skt.skaf.COL.REQUESTER",
								"A000Z00040"); // my App ID
						startActivity(intent);
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}

	public void vib() {
		getHandler().post(new Runnable() {

			public void run() {
				// TODO Auto-generated method stub
				try {
					semaphore.acquire();
					Vibrator vibe = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
					vibe.vibrate(500);
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}
			}
		});
	}

	private void showMoreApps() {
		getHandler().post(new Runnable() {
			public void run() {
				try {
					semaphore.acquire();
					if (MARKET_TYPE == MARKET_GOOGLE) {
						Intent goToMarket;
						goToMarket = new Intent(Intent.ACTION_VIEW, Uri
								.parse("market://search?q=pub:LitQoo"));
						startActivity(goToMarket);
					} else if (MARKET_TYPE == MARKET_TSTORE) {
						Intent intent = new Intent();
						intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
						intent.setClassName("com.skt.skaf.A000Z00040",
								"com.skt.skaf.A000Z00040.A000Z00040");
						intent.setAction("COLLAB_ACTION");
						intent.putExtra("com.skt.skaf.COL.URI",
								"SEARCH_ACTION/0/LitQoo".getBytes());
						intent.putExtra("com.skt.skaf.COL.REQUESTER",
								"A000Z00040");
						startActivity(intent);
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}

	private String getUTF8String(String _strKey) {
		int resourceID = getResources().getIdentifier(_strKey, "string",
				"com.litqoo.DrawingJack");
		// Log.i("cocos2d-x", "" + resourceID);
		if (resourceID != 0)
			return getResources().getString(resourceID);
		else {
			return _strKey;
		}
	}

	public Handler getHandler() {
		return handler;
	}

	public void setHandler(Handler handler) {
		this.handler = handler;
	}
	
	public native void alertAction(int t_alertNumber, int t_buttonNumber);
	
	public void addDoubleDialog(final String t_title, final String t_message, final String t_leftButtonText, final String t_rightButtonText, final int t_tag)
	{
		IgawCommon.endSession();
//		ADBrixManager.endSession();
		handler.post(new Runnable() {
			public void run() {
				try {
					semaphore.acquire();
					dialogValue = t_tag;
					AlertDialog.Builder myAlert = new AlertDialog.Builder(KSActivityBase.this);
					myAlert.setTitle(t_title);
					myAlert.setMessage(t_message);
					myAlert.setPositiveButton(t_leftButtonText, new DialogInterface.OnClickListener() {
						
						public void onClick(DialogInterface dialog, int which) {
							// TODO Auto-generated method stub
							mGLView.queueEvent(new Runnable() {
				                public void run() {
				                	alertAction(dialogValue, 0);
				                }
				            });
						}
					});
					
					myAlert.setNegativeButton(t_rightButtonText, new DialogInterface.OnClickListener() {
						
						public void onClick(DialogInterface dialog, int which) {
							// TODO Auto-generated method stub
							mGLView.queueEvent(new Runnable() {
				                public void run() {
				                	alertAction(dialogValue, 1);
				                	IgawCommon.startSession(KSActivityBase.this);
//				                	ADBrixManager.startSession(KSActivityBase.this); // getApplicationContext()에서 activity를 넣어주는 것으로 변경 되었습니다. - 2013-11-13
//				                	DBrixManager.startSession(this);
				                }
				            });
						}
					});
					
					Log.i("seo","double dialog");
					myAlert.show();
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}
			}
		});
	}
	
	public void addSingleDialog(final String t_title, final String t_message, final String t_checkButtonText, final int t_tag)
	{
		handler.post(new Runnable() {
			public void run() {
				try {
					semaphore.acquire();
					dialogValue = t_tag;
					AlertDialog.Builder myAlert = new AlertDialog.Builder(KSActivityBase.this);
					myAlert.setTitle(t_title);
					myAlert.setMessage(t_message);
					myAlert.setPositiveButton(t_checkButtonText, new DialogInterface.OnClickListener() {
					
						public void onClick(DialogInterface dialog, int which) {
							// TODO Auto-generated method stub
							mGLView.queueEvent(new Runnable() {
				                public void run() {
				                	alertAction(dialogValue, 0);
				                }
				            });
						}
					});
				
					myAlert.show();
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}
			}
		});
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		HSPPayment.closePaymentService();
		super.onDestroy();
	}
}
