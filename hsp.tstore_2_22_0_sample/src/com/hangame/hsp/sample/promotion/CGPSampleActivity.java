/*
 * @(#)CGP13SampleActivity.java $version 2012. 1. 9.
 *
 * Copyright 2012 NHN Corp. All rights Reserved. 
 * NHN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

package com.hangame.hsp.sample.promotion;

import com.hangame.hsp.HSPResult;
import com.hangame.hsp.cgp.HSPCGP;
import com.hangame.hsp.cgp.HSPCGP.CheckPromotionCB;
import com.hangame.hsp.cgp.constant.CGPType.ImageType;
import com.hangame.hsp.cgp.model.PromotionInfo;
import com.hangame.hsp.cgp.model.PromotionState;
import com.hangame.hsp.sample.util.SampleUtil;
import com.hangame.hsp.util.Log;

/**
 * The Class CGP13SampleActivity.
 * 
 * @author Eom Iksang
 */
public class CGPSampleActivity extends CGPBaseActivity {

    /** The TAG. */
    private final String TAG = "CGPSampleActivity130";

    /**
     * Inits the cgp.
     * 
     * @see com.hangame.hsp.cgp.sample.CGPBaseActivity#initCGP()
     */
    @Override
    public void initCGP() {
        // 없음
    }

    /**
     * Check promotion.
     * 
     * @see com.hangame.hsp.cgp.sample.CGPBaseActivity#checkPromotion()
     */
    @Override
    public void checkPromotion() {
        CheckPromotionCB callback = new SampleCheckPromotionCallback();
        HSPCGP.checkPromotion(mActivity, callback);
    }

    /**
     * Gets the promotion info.
     * 
     * @return the promotion info
     * @see com.hangame.hsp.cgp.sample.CGPBaseActivity#getPromotionInfo()
     */
    @Override
    public PromotionInfo getPromotionInfo() {
        return HSPCGP.getPromotionInfo();
    }

    /**
     * Request single game reward completed.
     * 
     * @see com.hangame.hsp.cgp.sample.CGPBaseActivity#requestSingleGameRewardCompleted()
     */
    @Override
    public void completePromotion() {
        HSPCGP.completePromotion(mActivity);
    }
    
    /**
     * Request single game reward completed.
     * 
     * @see com.hangame.hsp.cgp.sample.CGPBaseActivity#requestSingleGameRewardCompleted()
     */
    @Override
    public void completeInstallPromotion() {
        HSPCGP.completeInstallPromotion(mActivity);
    }
    
    /**
     * Request Delete promotion
     */
    @Override
    public void deletePromotion() {
        HSPCGP.deletePromotion(mActivity);
    }
    
    /**
     * The Class SampleCheckPromotionCallback.
     * 
     * @author Eom Iksang
     */
    class SampleCheckPromotionCallback implements CheckPromotionCB {

        /**
         * On check promotion.
         * 
         * @param hspResult the hsp result
         * @param context the context
         * @param promotionState the promotion state
         * @see com.hangame.hsp.cgp.HSPCGP.CheckPromotionCallback#onCheckPromotion(boolean,
         *      com.hangame.hsp.core.HSPResult, java.lang.Object, com.hangame.hsp.cgp.model.PromotionState)
         */
        @Override
        public void onCheckPromotion(HSPResult hspResult, Object context, PromotionState promotionState) {
            try {
                SampleUtil.hideProgressDialog(mActivity);
                btnRequestPromotion.setEnabled(true);

                if (!hspResult.isSuccess()) {
                    cgpStep = 0;
                    SampleUtil.logText(
                            mActivity,
                            getLogTextView(),
                            "Fail to get promotion info [code : " + hspResult.getCode() + ", detail : "
                                    + hspResult.getDetail() + "]");
                    return;
                }

                switch (promotionState) {
                    case CGP_NONE: {
                        SampleUtil.logText(mActivity, getLogTextView(), "State : CGP_NONE");

                        cgpStep = 0;
                        break;
                    }
                    case CGP_PROMOTION_EXISTS: {
                        SampleUtil.logText(mActivity, getLogTextView(), "State : CGP_PROMOTION_EXISTS [ID: "
                                + HSPCGP.getPromotionInfo().getPromotionId() + "]");

                        cgpStep = 1;
                        if (isAutoTest) {
                            cgpProcessor.showImage(mActivity);

                            int typeCd = HSPCGP.getPromotionInfo().getTypeCode();
                            if(typeCd == ImageType.BUTTON.getValue()) {
                                SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show promotion button");
                            } else if (typeCd == ImageType.BANNER.getValue()) {
                                SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show promotion banner");
                            } else {
                                SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show promotion popup");
                            }
                        }
                        break;
                    }
                    case CGP_REWARD_REQUIRED: {
                        SampleUtil.logText(mActivity, getLogTextView(), "State : CGP_REWARD_REQUIRED [ID: "
                                + HSPCGP.getPromotionInfo().getPromotionId() + "]");

                        cgpStep = (isSingleGame) ? 3 : 2;

                        if (isAutoTest) {
                            int result = cgpProcessor.processReward(mActivity, isSingleGame);
                            if(result == CGPResponseProcessor.MULTI_SUCCESS || result == CGPResponseProcessor.SINGLE_SUCCESS) {
                                    if (result == CGPResponseProcessor.MULTI_SUCCESS) {
                                        SampleUtil.logText(mActivity, getLogTextView(), "Success to reward - Multi Game");
                                        cgpProcessor.showImage(mActivity);
                                    } else if (result == CGPResponseProcessor.SINGLE_SUCCESS) {
                                        SampleUtil.logText(mActivity, getLogTextView(), "Success to reward - Single Game");
                                        HSPCGP.completePromotion(mActivity);
                                        SampleUtil.logText(mActivity, getLogTextView(), "Report reward completion");
                                        cgpProcessor.showImage(mActivity);
                                    }
                                    
                                    int typeCd = HSPCGP.getPromotionInfo().getTypeCode();
                                    if(typeCd == ImageType.BUTTON.getValue()) {
                                        SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show reward button");
                                    } else if (typeCd == ImageType.BANNER.getValue()) {
                                        SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show reward banner");
                                    } else {
                                        SampleUtil.logText(mActivity, mActivity.getLogTextView(), "Show reward popup");
                                    }
                            } else {
                                SampleUtil.logText(mActivity, getLogTextView(), "Fail to process rewarding");
                            }
                        }
                        break;
                    }
                }

            } catch (Exception e) {
                Log.e(TAG, "onCheckPromotion Error", e);
            }
        }

    }

    /**
     * Destroy.
     * 
     * @see com.hangame.hsp.cgp.sample.CGPBaseActivity#destroy()
     */
    @Override
    public void destroy() {

    }
}
