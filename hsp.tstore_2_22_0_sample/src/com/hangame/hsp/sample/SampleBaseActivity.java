/*
 * @(#)CGPBaseActivity.java $version 2012. 1. 11.
 *
 * Copyright 2012 NHN Corp. All rights Reserved. 
 * NHN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

package com.hangame.hsp.sample;

import com.hangame.hsp.HSPConfiguration;
import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPCore.HSPResetAccountCB;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.HSPState;
import com.hangame.hsp.sample.util.SampleDataConstants;
import com.hangame.hsp.sample.util.SampleUtil;
import com.hangame.hsp.util.Log;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Process;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

/**
 * The Class SampleBaseActivity.
 */
public abstract class SampleBaseActivity extends Activity {

    /** The m game no. */
    protected int mGameNo = 101;

    protected String mGameVersion = "1.0.0";

    /** The m game id. */
    protected String mGameId = "SK101";

    /** The m launching svr url. */
    protected String mLaunchingSvrURL = "";

    /** The m launching Zone. */
    protected String mLaunchingZone = "";

    /** The m market type. */
    protected String mMarketType = "";

    /** The m login btn. */
    protected Button mLoginBtn = null;

    /** The is back pressed. */
    protected boolean isBackPressed = false;

    /** The m activity. */
    protected SampleBaseActivity mActivity;

    /** The TAG. */
    private static final String TAG = "SampleBaseActivity";

    /** The m progress dialog. */
    public static ProgressDialog mProgressDialog = null;

    /**
     * On create.
     * 
     * @param savedInstanceState the saved instance state
     * @see com.hangame.hsp.cgp.sample.api110.activity.CGPSampleActivity110#onCreate(android.os.Bundle)
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        try {
            super.onCreate(savedInstanceState);
            mActivity = this;

            setGameData();

            if (HSPCore.getInstance() == null) {
                if (HSPCore.createInstance(mActivity, mGameNo, mGameId, mGameVersion)) {
                    Toast.makeText(this, "HSPCore.initialize Success!", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(this, "HSPCore.initialize Failed!", Toast.LENGTH_SHORT).show();
                    finish();
                }
            }
        } catch (Exception e) {
            Log.e(TAG, "onCreate Error", e);
        }
    }

    private void setGameData() {
        boolean mIsTest = (getIntent().getAction() == null);

        if (mIsTest) {
            Intent settingIntent = getIntent();
            mGameNo = settingIntent.getExtras().getInt(SampleDataConstants.HSP_INTENT_PARAM_GAME_NO);
            mGameId = settingIntent.getExtras().getString(SampleDataConstants.HSP_INTENT_PARAM_GAME_ID);
            mGameVersion = settingIntent.getExtras().getString(SampleDataConstants.HSP_INTENT_PARAM_GAME_VERSION);
            mLaunchingSvrURL = settingIntent.getExtras().getString(SampleDataConstants.HSP_INTENT_PARAM_LNC_ADDRESS);
            mLaunchingZone = settingIntent.getExtras().getString(SampleDataConstants.HSP_INTENT_PARAM_LNC_ZONE);
            mMarketType = settingIntent.getExtras().getString(SampleDataConstants.HSP_INTENT_PARAM_MARKET_TYPE);

            Log.w(TAG, "setGameData mGameNo " + mGameNo + " mGameId " + mGameId + " mGameVersion " + mGameVersion
                    + " mLaunchingSvrURL " + mLaunchingSvrURL + " mLaunchingZone " + mLaunchingZone + " mMarketType ");

            HSPConfiguration configuration = HSPConfiguration.getInstance(this);
            if (mLaunchingSvrURL != null && mLaunchingSvrURL.equals("") == false) {
                configuration.setAddressLaunching(mLaunchingSvrURL);
            }
            if (mLaunchingZone != null && mLaunchingZone.equals("") == false) {
                configuration.setLaunchingZone(mLaunchingZone);
            }
            if (mMarketType != null && mMarketType.equals("") == false) {
                configuration.setMarketCode(mMarketType);
            }
        }
    }

    /**
     * On resume.
     * 
     * @see android.app.Activity#onResume()
     */
    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "onResume");

        if (HSPCore.getInstance().getState() != HSPState.HSP_STATE_INIT) {
            login(false);
        }
    }

    @Override
    public void onPause() {
        super.onPause();
        Log.d(TAG, "onPause");

        suspend();
    }

    /**
     * return setting.
     * 
     * @param view the view
     */
    public void onExitClick(View view) {
        onDestroy();
    }

    /**
     * On login click.
     * 
     * @param view the view
     * @see com.hangame.hsp.cgp.sample.api110.activity.CGPSampleActivity110#onLoginClick(android.view.View)
     */
    public void onLoginClick(View view) {
        login(true);
    }

    private void login(boolean isManualLogin) {
        SampleUtil.showProgressDialog(mActivity);
        HSPCore.getInstance().login(mActivity, isManualLogin, new HSPCore.HSPLoginCB() {

            @Override
            public void onLogin(HSPResult result, boolean isPlayable) {
                SampleUtil.hideProgressDialog(mActivity);
                if (result.isSuccess()) {
                    afterLoginSuccess();
                } else {
                    afterLoginFail();
                    SampleUtil.toastMsg(mActivity, "onLogin : " + result.isSuccess() + " result : " + result.getCode());
                }
                afterLogin();
            }
        });
    }

    private void suspend() {
        // Call suspend() when onPause() is called
        if (HSPCore.getInstance().getState() == HSPState.HSP_STATE_ONLINE) {
            SampleUtil.showProgressDialog(mActivity);
            HSPCore.getInstance().suspend(new HSPCore.HSPSuspendCB() {

                @Override
                public void onSuspend(HSPResult result) {
                    SampleUtil.hideProgressDialog(mActivity);
                    if (result.isSuccess()) {
                        Log.d(TAG, "onSuspend success");
                    } else {
                        Log.d(TAG, "onSuspend fail: " + result);
                    }
                }
            });
        }
    }

    protected abstract void afterLoginSuccess();

    protected abstract void afterLoginFail();

    protected abstract void afterLogin();

    /**
     * On suspend click.
     * 
     * @param view the view
     */
    public void onSuspendClick(View view) {
        suspend();
    }

    /**
     * On reset.
     * 
     * @param view the view
     * @see com.hangame.hsp.PaymentSampleActivity.sample.CGPBaseActivity#onReset(android.view.View)
     */
    @SuppressWarnings("deprecation")
    public void onReset(View view) {
        try {
            HSPCore.getInstance().resetAccount(new HSPResetAccountCB() {

                @Override
                public void onAccountReset(HSPResult result) {
                    Log.d(TAG, "onReset : " + result.isSuccess() + " error : " + result.isSuccess());
                    SampleUtil.toastMsg(mActivity, "onReset onResult : " + result.isSuccess());
                    if (result.isSuccess()) {
                        // isLogined = false;
                    }
                }
            });
        } catch (Exception e) {
            Log.e(TAG, "onReset Error", e);
        }
    }

    /**
     * Check close activity.
     */
    protected void checkCloseActivity() {
        SampleUtil.toastMsg(mActivity, "Exit to press 'Back' button again.");
        TimerTask timerTask = new TimerTask() {
            @Override
            public void run() {
                isBackPressed = false;
                return;
            }
        };
        Timer timer = new Timer();
        timer.schedule(timerTask, 3000);
    }

    /**
     * On back pressed.
     * 
     * @see android.app.Activity#onBackPressed()
     */
    @Override
    public void onBackPressed() {
        if (isBackPressed) {
            super.onBackPressed();
            return;
        }
        isBackPressed = true;
        checkCloseActivity();
    }

    @Override
    protected void onDestroy() {
        Log.i(TAG, TAG + " onDestroy called.");
        Process.killProcess(Process.myPid()); // 분리된 프로세스 종료
        super.onDestroy();
    }

    protected void setLocale(Locale locale) {
        if (locale != null) {
            Locale.setDefault(locale);
            Configuration config = new Configuration();

            config.locale = locale;
            getResources().updateConfiguration(config, null);
        }
    }
}
