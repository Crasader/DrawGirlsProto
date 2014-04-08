/*
 * @(#)CGPResponseProcessor.java $version 2011. 10. 26.
 *
 * Copyright 2011 NHN Corp. All rights Reserved. 
 * NHN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

package com.hangame.hsp.sample.promotion;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.cgp.HSPCGP;
import com.hangame.hsp.cgp.constant.CGPType.ImageType;
import com.hangame.hsp.cgp.core.CGPService;
import com.hangame.hsp.cgp.model.PromotionInfo;
import com.hangame.hsp.sample.util.SampleUtil;
import com.hangame.hsp.ui.DialogManager;
import com.hangame.hsp.util.HttpsUtil;
import com.hangame.hsp.util.Log;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;

import java.util.HashMap;
import java.util.Map;

/**
 * The Class CGPResponseProcessor.
 * 
 * @author Eom Iksang
 */
public class CGPResponseProcessor {

    /** The TAG. */
    private final String TAG = "CGPResponseProcessor";

    /** The Constant SINGLE_SUCCESS. */
    public static final int SINGLE_SUCCESS = 2;

    /** The Constant MULTI_SUCCESS. */
    public static final int MULTI_SUCCESS = 1;

    /** The Constant FAIL. */
    public static final int FAIL = -1;

    /** The instance. */
    private static CGPResponseProcessor instance = null;

    /**
     * Instantiates a new cGP response processor.
     */
    private CGPResponseProcessor() {
    }

    /**
     * Gets the single instance of CGPResponseProcessor.
     * 
     * @return single instance of CGPResponseProcessor
     */
    public static CGPResponseProcessor getInstance() {
        if (instance == null) {
            instance = new CGPResponseProcessor();
        }
        return instance;
    }

    /** The banner color. */
    private int bannerColor;

    /**
     * Gets the banner color.
     * 
     * @return the banner color
     */
    public int getBannerColor() {
        return bannerColor;
    }

    /**
     * Sets the banner color.
     * 
     * @param bannerColor the new banner color
     */
    public void setBannerColor(int bannerColor) {
        this.bannerColor = bannerColor;
    }

    public void showImage(CGPBaseActivity activity) {
        showImage(activity, false);
    }
    
    /**
     * Show promotion.
     * 
     * @param activity the activity
     */
    public void showImage(CGPBaseActivity activity, boolean installRewardCheck) {

        PromotionInfo promotionInfo = HSPCGP.getPromotionInfo();
        
        if (promotionInfo.getTypeCode() == ImageType.BANNER.getValue()) {
            SampleUi.drawPromotionBanner(activity, bannerColor);
        } else if (promotionInfo.getTypeCode() == ImageType.POPUP.getValue()) {
            SampleUi.drawPopup(activity, bannerColor);
        } else if (promotionInfo.getTypeCode() == ImageType.BUTTON.getValue()) {
            /*
             * ================================================================ 이미지 버튼은 직접 구현해주세요!!!!!!!!!!
             * ================================================================
             */
            if (!CGPService.getInstance().getCgpResponse().isRewardCheck()
                    && !CGPService.getInstance().getCgpResponse().isPromotionCheck()
                    && !CGPService.getInstance().getCgpResponse().isRewardCheckByInstall()) {
                Log.d(TAG, "Not expected to call this API.");
                return;
            } else {
                SampleUi.drawButton(activity, installRewardCheck);
            }
        }
    }

    /**
     * Process reward.
     * 
     * @param activity the activity
     * @return the int
     */
    public int processReward(Activity activity, boolean isSingleGame) {
        PromotionInfo promotionInfo = HSPCGP.getPromotionInfo();
        if (promotionInfo != null) {
            if (isSingleGame) {
                SampleUtil.toastMsg(activity, "rewardCode : " + promotionInfo.getRewardCode() + "\nrewardValue : "
                        + promotionInfo.getRewardValue());
                SampleUtil.toastMsg(activity, "rewardCode_install : " + promotionInfo.getRewardCode_install() + "\nrewardValue_install : "
                        + promotionInfo.getRewardValue_install());
                return SINGLE_SUCCESS;
            } else {
                // 보상지급 요청 (실제 게임에서 처리) - 지급 OK 확인 후
                
                int resultReward = requestTestGameServer(activity,"http://183.110.213.7:11111/TestGameServer/ReqReward");
                int resultRewardInstall = requestTestGameServer(activity,"http://183.110.213.7:11111/TestGameServer/ReqRewardByInstall");
                
                if ( resultReward == FAIL && resultRewardInstall == FAIL) {
                    return FAIL;
                }
            }

        } else {
            SampleUtil.toastMsg(activity, "프로모션 정보를 얻어오는데 실패하였습니다. 다시 시도 해주세요");
        }
        return FAIL;

    }

    private int requestTestGameServer(Activity activity, String url) {
        Map<String, Object> resultMap = requestGameServer(url);
        if (resultMap == null) {
            SampleUtil.toastMsg(activity, "게임서버 접속 오류");
            return FAIL;
        }
        Log.d(TAG, "resultMap : " + resultMap.toString());
        Long result = (Long)resultMap.get("result");
        if (result == 0) { // result : 0 지급 성공, 1 지급 실패
            String rewardCode = (String)resultMap.get("rewardCode");
            Long rewardValue = (Long)resultMap.get("rewardValue");
            SampleUtil.toastMsg(activity, "rewardCode : " + rewardCode + "\nrewardValue : " + rewardValue);
            return MULTI_SUCCESS;
        } else {
            SampleUtil.toastMsg(activity, "보상 지급 실패 유효한 보상요청이 아님!!!!!!!");
            return FAIL;
        }
    }

    /**
     * Request game server.
     * 
     * @return the map
     */
    public Map<String, Object> requestGameServer(String url) {
        PromotionInfo promotionInfo = HSPCGP.getPromotionInfo();
        if (promotionInfo == null) {
            return null;
        }
        
        Map<String, String> paramMap = new HashMap<String, String>();
        paramMap.put("gameNo", String.valueOf(HSPCore.getInstance().getGameNo()));
        paramMap.put("memberNo", String.valueOf(HSPCore.getInstance().getMemberNo()));

        paramMap.put("promotionId", String.valueOf(promotionInfo.getPromotionId()));
        paramMap.put("ticket", HSPCore.getInstance().getTicket());
        Map<String, Object> resultMap = null;
        try {
            resultMap = HttpsUtil.execute(url, paramMap);
        } catch (Exception e) {
            Log.e(TAG, "requestGameServer Error", e);
        }
        return resultMap;
    }
}
