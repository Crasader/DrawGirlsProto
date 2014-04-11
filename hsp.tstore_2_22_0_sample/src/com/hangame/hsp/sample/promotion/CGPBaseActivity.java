/*
 * @(#)CGPBaseActivity.java $version 2012. 1. 11.
 *
 * Copyright 2012 NHN Corp. All rights Reserved. 
 * NHN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

package com.hangame.hsp.sample.promotion;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.cgp.HSPCGP;
import com.hangame.hsp.cgp.HSPCGP.HSPSendCustomDataCB;
import com.hangame.hsp.cgp.constant.CGPConstant;
import com.hangame.hsp.cgp.constant.CGPType.ImageType;
import com.hangame.hsp.cgp.core.CGPService;
import com.hangame.hsp.cgp.db.CGPDBManager;
import com.hangame.hsp.cgp.db.CGPDBObject;
import com.hangame.hsp.cgp.model.PromotionInfo;
import com.hangame.hsp.sample.SampleBaseActivity;
import com.hangame.hsp.sample.util.SampleUtil;
import com.hangame.hsp.ui.HSPUiFactory;
import com.hangame.hsp.ui.HSPUiLauncher;
import com.hangame.hsp.ui.HSPUiUri;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.ToggleButton;

import java.util.List;

/**
 * The Class CGPBaseActivity.
 * 
 * @author Eom Iksang
 */
public abstract class CGPBaseActivity extends SampleBaseActivity {

    /** The TAG. */
    private final String TAG = "CGPBaseActivity";

    /** The is single game. */
    protected boolean isSingleGame = false;

    /** The is auto test. */
    protected boolean isAutoTest = false;

    /** The text member no. */
    protected TextView textMemberNo = null;

    /** The ticket. */
    protected String ticket = null;

    /** The m log text view. */
    protected TextView mLogTextView;

    /** The btn request promotion. */
    public Button btnRequestPromotion = null;
    
    /** The btn send custom data. */
    public Button btnSendCustomData = null;

    /** The custom key we will send. */
    public EditText sendCustomKey = null;
    
    /** The custom value we will send. */
    public EditText sendCustomValue = null;
    
    /** The btn reset. */
    public Button btnReset = null;

    /** The btn suspend. */
    public Button btnSuspend = null;

    /** The btn get promotion data. */
    protected Button btnGetPromotionData = null;

    /** The btn del promotion data. */
    protected Button btnDelPromotionData = null;
    
    /** The promotion info. */
    protected PromotionInfo promotionInfo = null;

    /** The btn promotion button. */
    protected ImageButton btnPromotionButton = null;

    /** The text game type. */
    protected TextView textGameType;

    /** The revision text. */
    // protected TextView revisionText;

    /** The text zone. */
    protected TextView textZone;

    /** The text game no. */
    protected TextView textGameNo;

    /** The toggle wifi. */
    protected ToggleButton toggleWifi;

    /** The manual test layout. */
    protected LinearLayout manualTestLayout;

    /** The auto test layout. */
    protected LinearLayout autoTestLayout;

    /** The db test layout. */
    protected LinearLayout dbTestLayout;

    /** The desc2. */
    protected LinearLayout desc2;

    /** The text market type. */
    protected TextView textMarketType;

    /** The sp market type. */
    protected Spinner spMarketType;

    /** The btn select db. */
    protected Button btnSelectDB;

    /** The btn delete db. */
    protected Button btnDeleteDB;

    /** The is cgp init. */
    protected boolean isCGPInit = false;

    /** The is complete. */
    protected boolean isComplete = false;

    /** The is promotion. */
    protected boolean isPromotion = false;

    /** The cgp step. */
    public int cgpStep = 0;
    public int rewardStep = 0;

    /** The cgp processor. */
    protected CGPResponseProcessor cgpProcessor = CGPResponseProcessor.getInstance();

    /** The m activity. */
    protected CGPBaseActivity mActivity = this;

    /** The m handler. */
    protected Handler mHandler = new Handler();

    protected boolean isFirstLaunch = true;

    protected View mMainLayout;

    /**
     * Inits the cgp.
     */
    public abstract void initCGP();

    /**
     * Destroy.
     */
    public abstract void destroy();

    /**
     * Check promotion.
     */
    public abstract void checkPromotion();

    /**
     * Gets the promotion info.
     * 
     * @return the promotion info
     */
    public abstract PromotionInfo getPromotionInfo();

    /**
     * Request single game reward completed.
     */
    public abstract void completePromotion();
    
    /**
     * Request single game reward completed.
     */
    public abstract void completeInstallPromotion();
    
    /**
     * Request delete promotion
     */
    public abstract void deletePromotion();

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

            int mainLayoutId = getResources().getIdentifier("sample_cgp", "layout", getPackageName());
            mMainLayout = getLayoutInflater().inflate(mainLayoutId, null);
            setContentView(mMainLayout);
            setGameData();
            setButtonId();

            initCGP();
        } catch (Exception e) {
            Log.e(TAG, "onCreate Error", e);
        }
    }

    private void setGameData() {
        boolean mIsTest = (getIntent().getAction() == null);

        StringBuffer sb = new StringBuffer();
        sb.append("GameNo : ").append(mGameNo);
        sb.append("\nGameVersion : ").append(mGameVersion);
        sb.append("\nGameId : ").append(mGameId);
        sb.append("\nLaunchingSvrURL : ").append(mLaunchingSvrURL);
        sb.append("\nMarketType : ").append(mMarketType);
        
        if (mIsTest) {
            Intent settingIntent = getIntent();

            isSingleGame = settingIntent.getExtras().getBoolean("SingleGame");
            isAutoTest = settingIntent.getExtras().getBoolean("AutoTest");
            sb.append("\nSingleGame : ").append(isSingleGame);
            sb.append("\nAutoTest : ").append(isAutoTest);
        }

        SampleUtil.toastMsg(this, sb.toString());
    }

    /**
     * On launch nomad ui.
     * 
     * @param view the view
     */
    public void onLaunchNomadUI(View view) {
        HSPUiLauncher.getInstance().launch(HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.PROFILE_MYPROFILE));
    }

    /**
     * On request promotion.
     * 
     * @param view the view
     */
    public void onRequestPromotion(View view) {
        try {
            cgpProcessor.setBannerColor(CGPConstant.BANNER_COLOR_ORANGE);
            btnRequestPromotion.setEnabled(false);
            SampleUtil.showProgressDialog(mActivity);
            checkPromotion();
        } catch (Exception e) {
            Log.e(TAG, "onRequestPromotion Error", e);
        }
    }

    /**
     * On get image info.
     * 
     * @param view the view
     */
    public void onGetImageInfo(View view) {
        try {
            String[] imageInfo = CGPService.getInstance().getImageInfo();
            StringBuffer sb = new StringBuffer();
            sb.append("버튼 이미지 상태 : ").append(imageInfo[0]).append("\n");
            sb.append("배너 세로 이미지 상태 : ").append(imageInfo[1]).append("\n");
            sb.append("배너 가로 이미지 상태 : ").append(imageInfo[2]).append("\n");
            SampleUtil.popDialog(this, "이미지 정보", sb.toString(), "OK");
        } catch (Exception e) {
            Log.e(TAG, "onGetImageInfo Error", e);
        }
    }

    /**
     * On get promotion info.
     * 
     * @param view the view
     */
    public void onGetPromotionInfo(View view) {
        try {
            PromotionInfo promotionInfo = getPromotionInfo();
            SampleUtil.popDialog(this, "프로모션 정보", promotionInfo.toString(), "OK");
        } catch (Exception e) {
            Log.e(TAG, "onGetPromotionInfo Error", e);
        }
    }

    /**
     * On complete reward.
     * 
     * @param view the view
     */
    public void onCompletePromotion(View view) {
        try {
            if (cgpStep == 4 || rewardStep == 1) {
                completePromotion();
                SampleUtil.logText(mActivity, getLogTextView(), "Report reward completion");
                cgpStep = 5;
                rewardStep = 1;
            } else {
                SampleUtil.toastMsg(mActivity, "현재 진행 Step이 아닙니다.");
            }
        } catch (Exception e) {
            Log.e(TAG, "onRequestSingleGameRewardCompleted Error", e);
        }
    }

    /**
     * On show reward.
     * 
     * @param view the view
     */
    public void onShowReward(View view) {
        try {
            boolean installRewardCheck = false;
            cgpProcessor.showImage(mActivity, installRewardCheck);
            int typeCd = HSPCGP.getPromotionInfo().getTypeCode();
            if(typeCd == ImageType.BUTTON.getValue()) {
                SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show reward button");
            } else if (typeCd == ImageType.BANNER.getValue()) {
                if(CGPService.getInstance().getCgpResponse().getRewardInfo() != null) {
                    SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show install reward banner" + CGPService.getInstance().getCgpResponse().getRewardInfo().getBannerPortUrl());
                } else {
                    SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show install reward banner");
                }
            } else {
                if(CGPService.getInstance().getCgpResponse().getRewardInfo() != null) {
                    SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show install reward popup" + CGPService.getInstance().getCgpResponse().getRewardInfo().getBannerPortUrl());
                } else {
                    SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show install reward popup");
                }
            }
        } catch (Exception e) {
            Log.e(TAG, "onShowReward Error", e);
        }
    }
    
    /**
     * On complete reward.
     * 
     * @param view the view
     */
    public void onCompleteInstallPromotion(View view) {
        try {
            if (cgpStep == 4 || rewardStep == 1) {
                completeInstallPromotion();
                SampleUtil.logText(mActivity, getLogTextView(), "Report Install reward completion");
                cgpStep = 5;
                rewardStep = 1;
            } else {
                SampleUtil.toastMsg(mActivity, "현재 진행 Step이 아닙니다.");
            }
        } catch (Exception e) {
            Log.e(TAG, "onRequestSingleGameRewardCompleted Error", e);
        }
    }

    /**
     * On show reward.
     * 
     * @param view the view
     */
    public void onShowInstallReward(View view) {
        try {
            boolean installRewardCheck = true;
            cgpProcessor.showImage(mActivity, installRewardCheck);
            int typeCd = HSPCGP.getPromotionInfo().getTypeCode();
            if(typeCd == ImageType.BUTTON.getValue()) {
                SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show reward button");
            } else if (typeCd == ImageType.BANNER.getValue()) {
                if(CGPService.getInstance().getCgpResponse().getRewardInfoByInstall() != null) {
                    SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show install reward banner" + CGPService.getInstance().getCgpResponse().getRewardInfoByInstall().getBannerPortUrl());
                } else {
                    SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show install reward banner");
                }
            } else {
                if(CGPService.getInstance().getCgpResponse().getRewardInfoByInstall() != null) {
                    SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show install reward popup" + CGPService.getInstance().getCgpResponse().getRewardInfoByInstall().getBannerPortUrl());
                } else {
                    SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show install reward popup");
                }
            }
        } catch (Exception e) {
            Log.e(TAG, "onShowReward Error", e);
        }
    }
    
    /**
     * On delete promotion.
     * 
     * @param view the view
     */
    public void onDeletePromotion(View view) {
        try {
            deletePromotion();
            SampleUtil.logText(mActivity, getLogTextView(), "Delete completion");
        } catch (Exception e) {
            Log.e(TAG, "onDeletePromotion Error", e);
        }
    }

    /**
     * Sets the wifi mode.
     * 
     * @param context the context
     * @param enabled the enabled
     */
    public void setWifiMode(Context context, boolean enabled) {
        WifiManager wManager = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
        wManager.setWifiEnabled(enabled);
    }

    /**
     * Checks if is wifi on.
     * 
     * @param context the context
     * @return true, if is wifi on
     */
    public boolean isWifiOn(Context context) {
        WifiManager wManager = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
        return wManager.isWifiEnabled();
    }

    /**
     * On delete db.
     * 
     * @param view the view
     */
    public void onDeleteDB(View view) {
        try {
            if (CGPDBManager.getInstance().deleteAll(this.getApplicationContext())) {
                SampleUtil.popDialog(this, "DB", "DB 삭제 성공", "OK");
            } else {
                throw new Exception();
            }
        } catch (Exception e) {
            Log.e(TAG, "onDeleteDB Error", e);
        }
    }

    /**
     * On select db.
     * 
     * @param view the view
     */
    public void onSelectDB(View view) {
        try {
            List<CGPDBObject> list = CGPDBManager.getInstance().selectAll(this.getApplicationContext());
            StringBuilder sb = new StringBuilder();
            for (CGPDBObject data : list) {
                sb.append("[Promo ID] ").append(data.getPromoId()).append("[memberNo] ").append(data.getMemberNo())
                        .append(", [Status] ").append(data.getStatus()).append("\n");
            }
            SampleUtil.popDialog(this, "CGP Result Data", sb.toString(), "OK");
        } catch (Exception e) {
            Log.e(TAG, "onSelectDB Error", e);
        }
    }

    /**
     * Gets the version name.
     * 
     * @param context the context
     * @return the version name
     */
    public String getVersionName(Context context) {
        try {
            PackageInfo pi = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
            return pi.versionName;
        } catch (NameNotFoundException e) {
            return null;
        }
    }

    /**
     * Sets the button id.
     */
    protected void setButtonId() {
        try {
            btnReset = (Button)mMainLayout.findViewWithTag("btnReset");
            mLoginBtn = (Button)mMainLayout.findViewWithTag("loginButton");
            btnSuspend = (Button)mMainLayout.findViewWithTag("btnSuspend");
            // revisionText = (TextView)mMainLayout.findViewWithTag("RevisionText");
            // revisionText.setText("rev. " + getVersionName(this));
            textMarketType = (TextView)mMainLayout.findViewWithTag("textMarketType");

            textGameType = (TextView)mMainLayout.findViewWithTag("textGameType");
            textZone = (TextView)mMainLayout.findViewWithTag("textZone");
            textGameNo = (TextView)mMainLayout.findViewWithTag("textGameNo");

            manualTestLayout = (LinearLayout)mMainLayout.findViewWithTag("manualTestLayout");
            autoTestLayout = (LinearLayout)mMainLayout.findViewWithTag("autoTestLayout");
            dbTestLayout = (LinearLayout)mMainLayout.findViewWithTag("dbTestLayout");
            desc2 = (LinearLayout)mMainLayout.findViewWithTag("desc2");

            btnRequestPromotion = (Button)mMainLayout.findViewWithTag("btnRequestPromotion");
            btnSendCustomData = (Button)mMainLayout.findViewWithTag("btnSendCustomData");
            sendCustomKey = (EditText)mMainLayout.findViewWithTag("sendCustomKey");
            sendCustomValue = (EditText)mMainLayout.findViewWithTag("sendCustomValue");
            
            textMemberNo = (TextView)mMainLayout.findViewWithTag("textMemberNo");
            btnSelectDB = (Button)mMainLayout.findViewWithTag("btnSelectDB");
            btnDeleteDB = (Button)mMainLayout.findViewWithTag("btnDeleteDB");
            btnGetPromotionData = (Button)mMainLayout.findViewWithTag("btnGetPromotionData");
            btnDelPromotionData = (Button)mMainLayout.findViewWithTag("btnDelPromotionData");

            toggleWifi = (ToggleButton)mMainLayout.findViewWithTag("toggleWifi");
            if (isWifiOn(mActivity)) {
                toggleWifi.setChecked(true);
            } else {
                toggleWifi.setChecked(false);
            }
            toggleWifi.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                    try {
                        if (toggleWifi.isChecked()) {
                            setWifiMode(mActivity, true);
                        } else {
                            setWifiMode(mActivity, false);
                        }
                    } catch (Exception e) {
                        SampleUtil.popDialog(mActivity, "에러", "toggleWifi 에러", "OK");
                    }
                }
            });

            // radioGroupBannerColor = (RadioGroup)findViewById(R.id.radioGroupBannerColor);

            mLogTextView = (TextView)mMainLayout.findViewWithTag("log");
        } catch (Exception e) {
            Log.e(TAG, "setButtonId error.", e);
        }
    }

    /**
     * On show promotion.
     * 
     * @param view the view
     */
    public void onShowPromotion(View view) {
        try {
            if (cgpStep == 1) {
                cgpProcessor.showImage(mActivity);
                int typeCd = HSPCGP.getPromotionInfo().getTypeCode();
                if(typeCd == ImageType.BUTTON.getValue()) {
                    SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show promotion button");
                } else if (typeCd == ImageType.BANNER.getValue()) {
                    SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show promotion banner");
                } else {
                    SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show promotion popup");
                }
            } else {
                SampleUtil.toastMsg(mActivity, "현재 진행 Step이 아닙니다.");
            }
        } catch (Exception e) {
            Log.e(TAG, "onShowPromotion Error", e);
        }
    }

    /**
     * On process reward.
     * 
     * @param view the view
     */
    public void onProcessReward(View view) {
        try {
            if (cgpStep == 2 || cgpStep == 3) {
                int result = cgpProcessor.processReward(mActivity, isSingleGame);
                if (result == CGPResponseProcessor.MULTI_SUCCESS) {
                    SampleUtil.logText(mActivity, getLogTextView(), "Success to reward - Multi Game");
                    cgpStep = 5;
                } else if (result == CGPResponseProcessor.SINGLE_SUCCESS) {
                    SampleUtil.logText(mActivity, getLogTextView(), "Success to reward - Single Game");
                    cgpStep = 4;
                }
            } else {
                SampleUtil.toastMsg(mActivity, "현재 진행 Step이 아닙니다. 현재 step : " + cgpStep);
            }
        } catch (Exception e) {
            Log.e(TAG, "onProcessReward Error", e);
        }
    }

    
    /**
     * Send custom data in single game.
     * 
     * @param view the view
     */
    public void onSendCustomData(View view) {
        String key = null;
        Long value = 0L;
        try {
            key = sendCustomKey.getText().toString();
            value = Long.parseLong(sendCustomValue.getText().toString());
            SampleUtil.toastMsg(mActivity, "data :: " + key + "  " + value);
        } catch (Exception e) {
            // TODO: handle exception
            key="LEVEL";
            value = 10L;
        }
        
        HSPCGP.sendCustomData(key, value, new HSPSendCustomDataCB() {
            @Override
            public void onSendCustomData(HSPResult hspResult) {
                // TODO Auto-generated method stub
                if (hspResult.isSuccess()) {
                    SampleUtil.logText(mActivity, getLogTextView(), "data를 보내었습니다.");
                } else {
                    SampleUtil.logText(mActivity, getLogTextView(), "data보내는 것을 실패하였습니다. :: " + hspResult.getDetail());
                }
            }
        });
    }
    
    
    /**
     * Gets the log text view.
     * 
     * @return the log text view
     */
    public TextView getLogTextView() {
        return mLogTextView;
    }

    public boolean isSingleGame() {
        return isSingleGame;
    }

    @Override
    protected void afterLoginSuccess() {
        try {
            btnSuspend.setVisibility(Button.VISIBLE);
            mLoginBtn.setVisibility(Button.GONE);
            btnReset.setVisibility(Button.VISIBLE);
            btnRequestPromotion.setEnabled(true);
            btnSendCustomData.setEnabled(true);
            btnGetPromotionData.setEnabled(true);
            btnDelPromotionData.setEnabled(true);
            btnSelectDB.setEnabled(true);
            btnDeleteDB.setEnabled(true);
            textMemberNo.setText(String.valueOf(HSPCore.getInstance().getMemberNo()));

            if (isAutoTest) {
                manualTestLayout.setVisibility(LinearLayout.GONE);
                autoTestLayout.setVisibility(LinearLayout.VISIBLE);
            } else {
                manualTestLayout.setVisibility(LinearLayout.VISIBLE);
                autoTestLayout.setVisibility(LinearLayout.GONE);
            }
            dbTestLayout.setVisibility(LinearLayout.VISIBLE);
            textGameNo.setText(HSPCore.getInstance().getGameNo() + "");
        } catch (Exception e) {
            Log.e(TAG, "afterLoginSuccess error", e);
        }
    }

    @Override
    protected void afterLoginFail() {
        try {
            btnSuspend.setVisibility(Button.GONE);
            mLoginBtn.setVisibility(Button.VISIBLE);
            btnReset.setVisibility(Button.GONE);
            btnRequestPromotion.setEnabled(false);
            btnSendCustomData.setEnabled(false);
            btnGetPromotionData.setEnabled(false);
            btnDelPromotionData.setEnabled(false);
            btnSelectDB.setEnabled(false);
            btnDeleteDB.setEnabled(false);
            textMemberNo.setText("");
            manualTestLayout.setVisibility(LinearLayout.GONE);
            autoTestLayout.setVisibility(LinearLayout.GONE);
            dbTestLayout.setVisibility(LinearLayout.GONE);
            textGameNo.setText(-1 + "");
        } catch (Exception e) {
            Log.e(TAG, "afterLoginFail error", e);
        }
    }

    @Override
    protected void afterLogin() {
        // TODO Auto-generated method stub
    }
}
