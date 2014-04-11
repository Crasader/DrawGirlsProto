
package com.hangame.hsp.sample.core;

import com.hangame.hsp.HSPConfiguration;
import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPState;
import com.hangame.hsp.sample.core.login.HSPLoginAPI;
import com.hangame.hsp.sample.util.SampleDataConstants;
import com.hangame.hsp.sample.util.SampleUtil;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.os.Bundle;
import android.os.Process;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.Toast;

import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

/**
 * Test Game Activity
 * 
 * @author NHN
 */
public class CoreTestActivity extends Activity {
    private static final String TAG = "GameActivity";

    private boolean mIsTest;

    private int mGameNo = 10289;

    private String mGameId = "SKDRAWGIRLSA";

    private String mGameVersion = "1.0.0.KS";

    private boolean isBackPressed = false;

    private HSPLoginAPI loginAPI = new HSPLoginAPI();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.d(TAG, "onCreate");

        initialize();

        if (HSPCore.getInstance() == null) {
            if (loginAPI.initialize(this, mGameNo, mGameId, mGameVersion)) {
                Toast.makeText(this, "HSPCore.initialize Success!", Toast.LENGTH_SHORT).show();
                afterInitialize();

            } else {
                Toast.makeText(this, "HSPCore.initialize Failed!", Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.d(TAG, "onResume");

        // Auto login
        if (HSPCore.getInstance().getState() != HSPState.HSP_STATE_INIT) {
            loginAPI.login(this, false);
        }
    }

    @Override
    public void onPause() {
        super.onPause();
        Log.d(TAG, "onPause");

        // Call suspend() when onPause() is called
        if (HSPCore.getInstance().getState() == HSPState.HSP_STATE_ONLINE) {
            loginAPI.suspend();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Process.killProcess(Process.myPid());
    }

    /**
     * Check close activity.
     */
    protected void checkCloseActivity() {
        SampleUtil.toastMsg(this, "Exit to press 'Back' button again.");
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

    private void initialize() {
        initIntentParam();
        UiController.init(this, mIsTest);
    }

    private void afterInitialize() {
        UiController.printHSPInfo();
        UiController.updateView();
    }

    private void initIntentParam() {
        mIsTest = (getIntent().getAction() == null);

        if (mIsTest) {
            // Set current game process as game process
            ApplicationInfo app = getApplicationInfo();
            if (!app.processName.contains(":TestGame")) {
                app.processName = app.processName + ":TestGame";
            }

            Intent settingIntent = CoreTestActivity.this.getIntent();

            mGameNo = settingIntent.getExtras().getInt(SampleDataConstants.HSP_INTENT_PARAM_GAME_NO);
            mGameId = settingIntent.getExtras().getString(SampleDataConstants.HSP_INTENT_PARAM_GAME_ID);
            mGameVersion = settingIntent.getExtras().getString(SampleDataConstants.HSP_INTENT_PARAM_GAME_VERSION);

            String lncZone = settingIntent.getExtras().getString(SampleDataConstants.HSP_INTENT_PARAM_LNC_ZONE);
            String marketCode = settingIntent.getExtras().getString(SampleDataConstants.HSP_INTENT_PARAM_MARKET_TYPE);
            String localeStr = settingIntent.getExtras().getString(SampleDataConstants.HSP_INTENT_PARAM_LOCALE);
            boolean isUsePush = settingIntent.getExtras().getBoolean(SampleDataConstants.HSP_INTENT_PARAM_USE_PUSH);
            String language = settingIntent.getExtras().getString(SampleDataConstants.HSP_INTENT_PARAM_LANGUAGE);
            String country = settingIntent.getExtras().getString(SampleDataConstants.HSP_INTENT_PARAM_COUNTRY);

            long timeoutTCP = settingIntent.getExtras().getLong(SampleDataConstants.HSP_INTENT_PARAM_TIMEOUT_TCP);
            long timeoutHTTP = settingIntent.getExtras().getLong(SampleDataConstants.HSP_INTENT_PARAM_TIMEOUT_HTTP);
            long heartbeatInterval = settingIntent.getExtras().getLong(
                    SampleDataConstants.HSP_INTENT_PARAM_HEARTBEAT_INTERVAL);
            long lncRefreshTime = settingIntent.getExtras().getLong(
                    SampleDataConstants.HSP_INTENT_PARAM_LNC_REFRESH_TIME);
            long suspendDelay = settingIntent.getExtras().getLong(
                    SampleDataConstants.HSP_INTENT_PARAM_SUSPEND_DELAY_TIME);
            boolean isResendData = settingIntent.getExtras().getBoolean(
                    SampleDataConstants.HSP_INTENT_PARAM_IS_RESEND_FAILED_DATA);
            boolean isShowWelcome = settingIntent.getExtras().getBoolean(
                    SampleDataConstants.HSP_INTENT_PARAM_IS_SHOW_WELCOME);
            boolean isShowVersion = settingIntent.getExtras().getBoolean(
                    SampleDataConstants.HSP_INTENT_PARAM_IS_SHOW_VERSION);
            boolean isShowMaintenance = settingIntent.getExtras().getBoolean(
                    SampleDataConstants.HSP_INTENT_PARAM_IS_SHOW_MAINTENANCE);
            boolean enforcedDeviceLogin = settingIntent.getExtras().getBoolean(
                    SampleDataConstants.HSP_INTENT_PARAM_ENFORCED_DEVICE_LOGIN);
            String enableEmailLogin = settingIntent.getExtras().getString(
                    SampleDataConstants.HSP_ENABLE_EMAIL_LOGIN_KEY);
            String litmusLogAgreementCheck = settingIntent.getExtras().getString(
                    SampleDataConstants.HSP_LITMUS_LOG_AGREEMENT_CHECK_KEY);

            // Set locale to the phone configuration
            if (language != null) {
                Log.d(TAG, "language: " + language);
                Locale locale = new Locale(language, country);
                SampleUtil.setLocale(getApplicationContext(), locale);
            }

            HSPConfiguration configuration = HSPConfiguration.getInstance(CoreTestActivity.this);

            if (configuration != null) {
                // Reset configuration data as currently applied lnc, market, locale information
                configuration.setLaunchingZone(lncZone);
                configuration.setMarketCode(marketCode);
                configuration.setLocale(localeStr);
                configuration.setIsUsePush(isUsePush);

                configuration.setTimeoutTCP(timeoutTCP);
                configuration.setTimeoutHTTP(timeoutHTTP);
                configuration.setHeartBeatTimeInterval(heartbeatInterval);
                configuration.setLncRefreshTimeInterval(lncRefreshTime);
                configuration.setSuspendDelayTime(suspendDelay);

                configuration.setResendFailedData(isResendData);
                configuration.setUiWelcomeMessage(isShowWelcome);
                configuration.setShowUiVersionCheck(isShowVersion);
                configuration.setShowUiMaintenance(isShowMaintenance);
                configuration.setEnforcedDeviceLogin(enforcedDeviceLogin);
                configuration.setConfigurationItem(SampleDataConstants.HSP_LITMUS_LOG_AGREEMENT_CHECK_KEY,
                        litmusLogAgreementCheck);
                configuration.setConfigurationItem(SampleDataConstants.HSP_ENABLE_EMAIL_LOGIN_KEY,
                        enableEmailLogin);
            }
        }
    }
}
