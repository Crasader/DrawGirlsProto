/*
 * @(#)SampleUi.java $version 2012. 1. 17.
 *
 * Copyright 2012 NHN Corp. All rights Reserved. 
 * NHN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

package com.hangame.hsp.sample.promotion;

import com.hangame.hsp.cgp.core.CGPService;
import com.hangame.hsp.util.Log;

/**
 * The Class SampleUi.
 * 
 * @author Eom Iksang
 */
public class SampleUi {

    /** The Constant TAG. */
    private static final String TAG = "SampleUi";

    /** The is showing promotion banner. */
    private static boolean isShowingPromotionBanner;

    /**
     * Checks if is showing promotion banner.
     * 
     * @return true, if is showing promotion banner
     */
    public static boolean isShowingPromotionBanner() {
        return isShowingPromotionBanner;
    }

    /**
     * Sets the showing promotion banner.
     * 
     * @param isShowingPromotionBanner the new showing promotion banner
     */
    public static void setShowingPromotionBanner(boolean isShowingPromotionBanner) {
        SampleUi.isShowingPromotionBanner = isShowingPromotionBanner;
    }

    /**
     * Draw promotion banner.
     * 
     * @param activity the activity
     * @param color the color
     * @return true, if successful
     */
    public static boolean drawPromotionBanner(CGPBaseActivity activity, int color) {
        try {
            if (!CGPService.getInstance().getCgpResponse().isRewardCheck()
                    && !CGPService.getInstance().getCgpResponse().isPromotionCheck()) {
                Log.d(TAG, "Not expected to call this API.");
                return false;
            }
            isShowingPromotionBanner = false;
            CGPBannerBridgeView bView = new CGPBannerBridgeView(activity, color);
            return bView.show();
        } catch (Exception e) {
            Log.e(TAG, e.getLocalizedMessage(), e);
        }
        return false;
    }

    /**
     * Draw button.
     * 
     * @param activity the activity
     * @return true, if successful
     */
    public static boolean drawButton(final CGPBaseActivity activity, boolean installRewardCheck) {
        try {
            if (!CGPService.getInstance().getCgpResponse().isRewardCheck()
                    && !CGPService.getInstance().getCgpResponse().isPromotionCheck()
                    && !CGPService.getInstance().getCgpResponse().isRewardCheckByInstall()) {
                Log.d(TAG, "Not expected to call this API.");
                return false;
            }
            CGPButtonBridgeView.getInstance().drawButton(activity, installRewardCheck);
            return true;
        } catch (Exception e) {
            Log.e(TAG, e.getLocalizedMessage(), e);
        }
        return false;
    }
    
    /**
     * Draw popup.
     * 
     * @param activity the activity
     * @param color the color
     * @return true, if successful
     */
    public static boolean drawPopup(CGPBaseActivity activity, int color) {
        try {
            if (!CGPService.getInstance().getCgpResponse().isRewardCheck()
                    && !CGPService.getInstance().getCgpResponse().isPromotionCheck()
                    && !CGPService.getInstance().getCgpResponse().isRewardCheckByInstall()) {
                Log.d(TAG, "Not expected to call this API.");
                return false;
            }
            isShowingPromotionBanner = false;
            CGPPopupBridgeView bView = new CGPPopupBridgeView(activity, color);
            return bView.show();
        } catch (Exception e) {
            Log.e(TAG, e.getLocalizedMessage(), e);
        }
        return false;
    }
}
