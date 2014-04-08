
package com.hangame.hsp.sample.promotion;

import com.hangame.hsp.cgp.constant.CGPConstant;
import com.hangame.hsp.cgp.core.CGPService;
import com.hangame.hsp.cgp.model.PromotionInfo;
import com.hangame.hsp.cgp.processor.ScreenUtil;
import com.hangame.hsp.cgp.processor.ScreenUtil.ScreenOrientationType;
import com.hangame.hsp.sample.util.SampleUtil;
import com.hangame.hsp.util.BitmapUtil;
import com.hangame.hsp.util.Log;

import android.graphics.drawable.BitmapDrawable;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.TranslateAnimation;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

/**
 * 프로모션 배너 / 버튼 이미지 노출.
 * 
 * @author Eom Iksang
 */
public class CGPPopupBridgeView {

    /** The TAG. */
    private final String TAG = "CGPBannerBridgeView";

    /** The activity. */
    private CGPBaseActivity activity;

    /** The promotion info. */
    private PromotionInfo promotionInfo;

    /** The body layout. */
    private static RelativeLayout bodyLayout;

    /** The animation layout. */
    private RelativeLayout animationLayout;

    /** The banner linear layout. */
    private LinearLayout bannerLinearLayout;
    
    /** The popup relative layout. */
    private RelativeLayout popupRelativeLayout;

    /** The tab button by disable mode. */
    private TextView tabButtonByDisableMode;

    /** The is showing animation. */
    private boolean isShowingAnimation;

    // private boolean forcedDraw;
    /** The banner frame image color info. */
    private BannerFrameImageColorInfo bannerFrameImageColorInfo;

    /**
     * Instantiates a new cGP banner bridge view.
     * 
     * @param activity the activity
     * @param color the color
     */
    public CGPPopupBridgeView(CGPBaseActivity activity, int color) {
        this.activity = activity;

        promotionInfo = CGPService.getInstance().getPromotionInfo();
        SampleUtil.logText(activity, activity.getLogTextView(), "Show reward land bannerUrl" + promotionInfo.getPromotionBannerLandUrl());
        SampleUtil.logText(activity, activity.getLogTextView(), "Show install reward land bannerUrl" + promotionInfo.getPromotionBannerLandUrl_install());
        
        bannerFrameImageColorInfo = new BannerFrameImageColorInfo();
        if (color == CGPConstant.BANNER_COLOR_ORANGE) {
            bannerFrameImageColorInfo.setBannerSideLineVerical("cgp_banner_side_line_ver_orange");
            bannerFrameImageColorInfo.setBannerSideLineHorizontal("cgp_banner_side_line_hor_orange");
            bannerFrameImageColorInfo.setBannerTabClosed("cgp_banner_closed_button_orange");
        } else if (color == CGPConstant.BANNER_COLOR_GRAY) {
            bannerFrameImageColorInfo.setBannerSideLineVerical("cgp_banner_side_line_ver_gray");
            bannerFrameImageColorInfo.setBannerSideLineHorizontal("cgp_banner_side_line_hor_gray");
            bannerFrameImageColorInfo.setBannerTabClosed("cgp_banner_closed_button_gray");
        }
        
    }

    /**
     * Show.
     * 
     * @return true, if successful
     */
    public boolean show() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (bodyLayout != null) {
                    bodyLayout.removeAllViews();
                    bodyLayout.setVisibility(RelativeLayout.INVISIBLE);
                    SampleUi.setShowingPromotionBanner(false);
                }
            }
        });
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                showPromotion();
            }
        });
        return true;
    }

    /**
     * Show promotion.
     */
    private void showPromotion() {
        if (SampleUi.isShowingPromotionBanner()) {
            Log.d(TAG, "Promotion Banner is Showing.");
            return;
        }
        try {
                
            bodyLayout = (RelativeLayout)activity.getLayoutInflater().inflate(
                  activity.getResources().getIdentifier("sample_cgp_promotion_default_layout", "layout",
                  activity.getPackageName()), null);
            ScreenOrientationType screenOrientation = ScreenUtil.getScreenOrientation(activity);
            if (screenOrientation == ScreenOrientationType.LANDSCAPE) {
                setAnimationLayoutByScrn(false);
                setPopupLinearLayout(false);
            } else if (screenOrientation == ScreenOrientationType.PORTRAIT) {
                setAnimationLayoutByScrn(true);
                setPopupLinearLayout(true);
            }
                
            animationLayout.addView(popupRelativeLayout);
            bodyLayout.removeAllViewsInLayout();
            bodyLayout.addView(animationLayout);
            activity.addContentView(bodyLayout, new RelativeLayout.LayoutParams(LayoutParams.FILL_PARENT,
                    LayoutParams.FILL_PARENT));
            setLayoutAnimationSlideUpFromBottom(animationLayout);
            SampleUi.setShowingPromotionBanner(true);
        } catch (Exception e) {
            Log.e(TAG, e.getMessage(), e);
        }
    }
    
    /**
     * Sets the animation layout.
     * 
     * @throws Exception the exception
     */
    private void setAnimationLayoutByScrn(boolean isPortScrn) throws Exception {
        RelativeLayout.LayoutParams aniLayoutParam = new RelativeLayout.LayoutParams(0, 0);
        aniLayoutParam.addRule(RelativeLayout.CENTER_IN_PARENT);
        if(isPortScrn == true) {
            aniLayoutParam.width = BitmapUtil.dipToPx(PopupLayoutInfo.ANIMATION_PORT_LAYOUT_WIDTH);
            aniLayoutParam.height = BitmapUtil.dipToPx(PopupLayoutInfo.ANIMATION_PORT_LAYOUT_HEIGHT);    
        } else {
            aniLayoutParam.width = BitmapUtil.dipToPx(PopupLayoutInfo.ANIMATION_LAND_LAYOUT_WIDTH);
            aniLayoutParam.height = BitmapUtil.dipToPx(PopupLayoutInfo.ANIMATION_LAND_LAYOUT_HEIGHT);
        }
        
        aniLayoutParam.leftMargin = BitmapUtil.dipToPx(BannerLayoutInfo.ANIMAITION_LAYOUT_LEFT_MARGIN);
        aniLayoutParam.rightMargin = BitmapUtil.dipToPx(BannerLayoutInfo.ANIMATION_LAYOUT_RIGHT_MARGIN);
        animationLayout = new RelativeLayout(activity);
        animationLayout.setLayoutParams(aniLayoutParam);
    }

    /**
     * Sets the banner linear layout.
     * 
     * @throws Exception the exception
     */
    private void setPopupLinearLayout(boolean isPortScrn) throws Exception {
        
        if(isPortScrn == true) {
            popupRelativeLayout = (RelativeLayout)activity.getLayoutInflater().inflate(
                    activity.getResources().getIdentifier("sample_cgp_promotion_popup_layout_port", "layout",
                    activity.getPackageName()), null);
        } else {
            popupRelativeLayout = (RelativeLayout)activity.getLayoutInflater().inflate(
                    activity.getResources().getIdentifier("sample_cgp_promotion_popup_layout_land", "layout",
                    activity.getPackageName()), null);
        }
        
        tabButtonByDisableMode = (TextView)popupRelativeLayout.findViewWithTag("popupClose");
        tabButtonByDisableMode.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                bodyLayout.setVisibility(View.INVISIBLE);
                bodyLayout.removeAllViews();
                isShowingAnimation = false;
                SampleUi.setShowingPromotionBanner(false);
            }
            
        });
        
        popupRelativeLayout.findViewWithTag("popupImg").setBackground(new BitmapDrawable(promotionInfo.getPromotionBannerPortImg()));
        popupRelativeLayout.findViewWithTag("popupImg").setOnClickListener(new OnClickListener() {
            
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                bodyLayout.setVisibility(View.INVISIBLE);
                bodyLayout.removeAllViews();
                isShowingAnimation = false;
                SampleUi.setShowingPromotionBanner(false);
                SampleUtil.logText(activity, activity.getLogTextView(), "Popup clicked. : launchPromotion");
                CGPService.getInstance().launchPromotion(activity);
            }
        });
    }

    /**
     * Sets the layout animation slide up from bottom.
     * 
     * @param panel the new layout animation slide up from bottom
     * @throws Exception the exception
     */
    private void setLayoutAnimationSlideUpFromBottom(ViewGroup panel) throws Exception {
        AnimationSet set = new AnimationSet(true);
        Animation animation = new TranslateAnimation(Animation.RELATIVE_TO_SELF, 0.0f, Animation.RELATIVE_TO_SELF,
                0.0f, Animation.RELATIVE_TO_SELF, BannerLayoutInfo.ANIMATION_MOTION_RATIO, Animation.RELATIVE_TO_SELF,
                0.0f);
        animation.setDuration(BannerLayoutInfo.ANIMATION_DURATION);
        set.addAnimation(animation);
        panel.startAnimation(set);
    }

    /**
     * Sets the layout animation slide down.
     * 
     * @param panel the new layout animation slide down
     * @throws Exception the exception
     */
    private void setLayoutAnimationSlideDown(ViewGroup panel) throws Exception {
        AnimationSet set = new AnimationSet(true);
        Animation animation = new TranslateAnimation(Animation.RELATIVE_TO_SELF, 0.0f, Animation.RELATIVE_TO_SELF,
                0.0f, Animation.RELATIVE_TO_SELF, 0.0f, Animation.RELATIVE_TO_SELF,
                BannerLayoutInfo.ANIMATION_MOTION_RATIO);
        animation.setDuration(BannerLayoutInfo.ANIMATION_DURATION);
        set.addAnimation(animation);
        panel.startAnimation(set);
    }

}
