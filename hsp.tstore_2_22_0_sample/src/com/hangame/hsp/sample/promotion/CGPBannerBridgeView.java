
package com.hangame.hsp.sample.promotion;

import com.hangame.hsp.cgp.constant.CGPConstant;
import com.hangame.hsp.cgp.constant.CGPType.ImageType;
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

/**
 * 프로모션 배너 / 버튼 이미지 노출.
 * 
 * @author Eom Iksang
 */
public class CGPBannerBridgeView {

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

    /** The banner outer layout by disable mode. */
    private RelativeLayout bannerOuterLayoutByDisableMode;

    /** The banner outer layout by enable mode. */
    private RelativeLayout bannerOuterLayoutByEnableMode;

    /** The tab button by enable mode. */
    private ImageButton tabButtonByEnableMode;

    /** The tab button by disable mode. */
    private ImageButton tabButtonByDisableMode;

    /** The banner button. */
    private ImageButton bannerButton;

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
    public CGPBannerBridgeView(CGPBaseActivity activity, int color) {
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

            if (promotionInfo.getTypeCode() == ImageType.BUTTON.getValue()) {
                Log.w(TAG, "Cannot use this API for button type.");
            } else if (promotionInfo.getTypeCode() == ImageType.BANNER.getValue() || promotionInfo.getTypeCode() == ImageType.POPUP.getValue()) {
                bodyLayout = (RelativeLayout)activity.getLayoutInflater().inflate(
                        activity.getResources().getIdentifier("sample_cgp_promotion_default_layout", "layout",
                                activity.getPackageName()), null);
                setAnimationLayout();
                setBannerLinearLayout();
                setTabButtonByEnableMode();
                setBannerButton();
            }
            bannerOuterLayoutByEnableMode.addView(bannerButton);
            bannerLinearLayout.addView(tabButtonByEnableMode);
            bannerLinearLayout.addView(bannerOuterLayoutByEnableMode);
            animationLayout.addView(bannerLinearLayout);
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
    private void setAnimationLayout() throws Exception {
        RelativeLayout.LayoutParams aniLayoutParam = new RelativeLayout.LayoutParams(0, 0);
        aniLayoutParam.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
        aniLayoutParam.addRule(RelativeLayout.CENTER_HORIZONTAL);
        aniLayoutParam.width = BitmapUtil.dipToPx(BannerLayoutInfo.ANIMATION_LAYOUT_WIDTH);
        aniLayoutParam.height = BitmapUtil.dipToPx(BannerLayoutInfo.ANIMATION_LAYOUT_HEIGHT);
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
    private void setBannerLinearLayout() throws Exception {
        bannerLinearLayout = new LinearLayout(activity);
        bannerLinearLayout.setOrientation(LinearLayout.VERTICAL);
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LayoutParams.FILL_PARENT,
                LayoutParams.FILL_PARENT);
        params.bottomMargin = BitmapUtil.dipToPx(BannerLayoutInfo.BANNER_LINEAR_LAYOUT_BOTTOM_MARGIN);
        bannerLinearLayout.setLayoutParams(params);
    }

    /**
     * Sets the banner button.
     * 
     * @throws Exception the exception
     */
    private void setBannerButton() throws Exception {
        try {
            bannerOuterLayoutByEnableMode = new RelativeLayout(activity);
            bannerButton = new ImageButton(activity);
            ScreenOrientationType screenOrientation = ScreenUtil.getScreenOrientation(activity);
            if (screenOrientation == ScreenOrientationType.LANDSCAPE) {
                setLandscapeBannerProperties();
            } else if (screenOrientation == ScreenOrientationType.PORTRAIT) {
                setPortraitBannerProperties();
            }
            bannerButton.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (!isShowingAnimation) {
                        bodyLayout.setVisibility(View.INVISIBLE);
                        bodyLayout.removeAllViews();
                        isShowingAnimation = false;
                        SampleUi.setShowingPromotionBanner(false);
                        SampleUtil.logText(activity, activity.getLogTextView(), "Banner clicked. : launchPromotion");
                        CGPService.getInstance().launchPromotion(activity);
                    }
                }
            });
        } catch (Exception e) {
            throw new Exception(e.getMessage());
        }
    }

    /**
     * Sets the portrait banner properties.
     * 
     * @throws Exception the exception
     */
    private void setPortraitBannerProperties() throws Exception {
        RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(0, 0);
        params.width = BitmapUtil.dipToPx(BannerLayoutInfo.PORTRAIT_BANNER_OUTER_LAYOUT_BY_ENABLE_MODE_WIDTH);
        params.height = BitmapUtil.dipToPx(BannerLayoutInfo.PORTRAIT_BANNER_OUTER_LAYOUT_BY_ENABLE_MODE_HEIGHT);

        bannerOuterLayoutByEnableMode.setLayoutParams(params);
        try {
            bannerOuterLayoutByEnableMode.setBackgroundResource(activity.getResources().getIdentifier(
                    bannerFrameImageColorInfo.getBannerSideLineVerical(), "drawable", activity.getPackageName()));
        } catch (Exception e) {
            throw new Exception("Cannot find background resource.");
        }

        RelativeLayout.LayoutParams btParams = new RelativeLayout.LayoutParams(0, 0);
        btParams.topMargin = BitmapUtil.dipToPx(BannerLayoutInfo.PORTRAIT_BANNER_BUTTON_TOP_MARGIN);
        btParams.bottomMargin = BitmapUtil.dipToPx(BannerLayoutInfo.PORTRAIT_BANNER_BUTTON_BOTTOM_MARGIN);
        btParams.leftMargin = BitmapUtil.dipToPx(BannerLayoutInfo.PORTRAIT_BANNER_BUTTON_LEFT_MARGIN);
        btParams.rightMargin = BitmapUtil.dipToPx(BannerLayoutInfo.PORTRAIT_BANNER_BUTTON_RIGHT_MARGIN);
        btParams.width = BitmapUtil.dipToPx(BannerLayoutInfo.PORTRAIT_BANNER_BUTTON_WIDTH);
        btParams.height = BitmapUtil.dipToPx(BannerLayoutInfo.PORTRAIT_BANNER_BUTTON_HEIGHT);

        bannerButton.setLayoutParams(btParams);
        try {
            bannerButton.setBackgroundDrawable(new BitmapDrawable(promotionInfo.getPromotionBannerPortImg()));
        } catch (Exception e) {
            throw new Exception("Cannot find banner image.");
        }
    }

    /**
     * Sets the landscape banner properties.
     * 
     * @throws Exception the exception
     */
    private void setLandscapeBannerProperties() throws Exception {
        RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(0, 0);
        params.width = BitmapUtil.dipToPx(BannerLayoutInfo.LANDSCAPE_BANNER_OUTER_LAYOUT_BY_ENABLE_MODE_WIDTH);
        params.height = BitmapUtil.dipToPx(BannerLayoutInfo.LANDSCAPE_BANNER_OUTER_LAYOUT_BY_ENABLE_MODE_HEIGHT);
        bannerOuterLayoutByEnableMode.setLayoutParams(params);
        try {
            bannerOuterLayoutByEnableMode.setBackgroundResource(activity.getResources().getIdentifier(
                    bannerFrameImageColorInfo.getBannerSideLineHorizontal(), "drawable", activity.getPackageName()));
        } catch (Exception e) {
            throw new Exception("Cannot find background resource.");
        }
        RelativeLayout.LayoutParams btParams = new RelativeLayout.LayoutParams(0, 0);
        btParams.topMargin = BitmapUtil.dipToPx(BannerLayoutInfo.LANDSCAPE_BANNER_BUTTON_TOP_MARGIN);
        btParams.bottomMargin = BitmapUtil.dipToPx(BannerLayoutInfo.LANDSCAPE_BANNER_BUTTON_BOTTOM_MARGIN);
        btParams.leftMargin = BitmapUtil.dipToPx(BannerLayoutInfo.LANDSCAPE_BANNER_BUTTON_LEFT_MARGIN);
        btParams.rightMargin = BitmapUtil.dipToPx(BannerLayoutInfo.LANDSCAPE_BANNER_BUTTON_RIGHT_MARGIN);
        btParams.width = BitmapUtil.dipToPx(BannerLayoutInfo.LANDSCAPE_BANNER_BUTTON_WIDTH);
        btParams.height = BitmapUtil.dipToPx(BannerLayoutInfo.LANDSCAPE_BANNER_BUTTON_HEIGHT);
        bannerButton.setLayoutParams(btParams);
        try {
            bannerButton.setBackgroundDrawable(new BitmapDrawable(promotionInfo.getPromotionBannerLandImg()));
        } catch (Exception e) {
            throw new Exception("Cannot find banner image.");
        }
    }

    /**
     * Sets the tab button by enable mode.
     * 
     * @throws Exception the exception
     */
    private void setTabButtonByEnableMode() throws Exception {
        RelativeLayout.LayoutParams btCloseParams = new RelativeLayout.LayoutParams(0, 0);
        btCloseParams.width = BitmapUtil.dipToPx(BannerLayoutInfo.TAB_BUTTON_WIDTH);
        btCloseParams.height = BitmapUtil.dipToPx(BannerLayoutInfo.TAB_BUTTON_HEIGHT);
        tabButtonByEnableMode = new ImageButton(activity);
        tabButtonByEnableMode.setLayoutParams(btCloseParams);
        tabButtonByEnableMode.setBackgroundResource(activity.getResources().getIdentifier(
                bannerFrameImageColorInfo.getBannerTabClosed(), "drawable", activity.getPackageName()));
        try {
            tabButtonByEnableMode.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View v) {
                    Log.d(TAG, "Show Button Clicked.");
                    if (!isShowingAnimation) {
                        isShowingAnimation = true;
                        try {
                            setLayoutAnimationSlideDown(animationLayout);
                        } catch (Exception e) {
                            Log.e(TAG, e.getMessage(), e);
                        }
                        activityHandler.sendEmptyMessageDelayed(0, BannerLayoutInfo.ANIMATION_DURATION);
                    }
                }

                Handler activityHandler = new Handler() {
                    @Override
                    public void handleMessage(Message msg) {
                        bodyLayout.setVisibility(View.GONE);
                        bodyLayout.removeAllViews();
                        bodyLayout = (RelativeLayout)activity.getLayoutInflater().inflate(
                                activity.getResources().getIdentifier("sample_cgp_promotion_default_layout", "layout",
                                        activity.getPackageName()), null);
                        try {
                            setTabButtonByDisableMode();
                            setBannerOuterLayoutByDisableMode();
                        } catch (Exception e) {
                            Log.e(TAG, e.getMessage(), e);
                        }
                        bannerOuterLayoutByDisableMode.addView(tabButtonByDisableMode);
                        bodyLayout.addView(bannerOuterLayoutByDisableMode);
                        activity.addContentView(bodyLayout, new RelativeLayout.LayoutParams(LayoutParams.FILL_PARENT,
                                LayoutParams.FILL_PARENT));
                        isShowingAnimation = false;
                    }
                };
            });
        } catch (Exception e) {
            throw new Exception("Close button message handler error.");
        }
    }

    /**
     * Sets the banner outer layout by disable mode.
     * 
     * @throws Exception the exception
     */
    private void setBannerOuterLayoutByDisableMode() throws Exception {
        RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(0, LayoutParams.WRAP_CONTENT);
        params.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
        params.addRule(RelativeLayout.CENTER_HORIZONTAL);
        params.width = BitmapUtil.dipToPx(BannerLayoutInfo.BANNER_OUTER_LAYOUT_BY_DISABLE_MODE_WIDTH);
        params.leftMargin = BitmapUtil.dipToPx(BannerLayoutInfo.BANNER_OUTER_LAYOUT_BY_DISABLE_MODE_LEFT_MARGIN);
        params.rightMargin = BitmapUtil.dipToPx(BannerLayoutInfo.BANNER_OUTER_LAYOUT_BY_DISABLE_MODE_RIGHT_MARGIN);
        bannerOuterLayoutByDisableMode = new RelativeLayout(activity);
        bannerOuterLayoutByDisableMode.setLayoutParams(params);
    }

    /**
     * Sets the tab button by disable mode.
     * 
     * @throws Exception the exception
     */
    private void setTabButtonByDisableMode() throws Exception {
        RelativeLayout.LayoutParams btCloseParams = new RelativeLayout.LayoutParams(0, 0);
        btCloseParams.width = BitmapUtil.dipToPx(BannerLayoutInfo.TAB_BUTTON_WIDTH);
        btCloseParams.height = BitmapUtil.dipToPx(BannerLayoutInfo.TAB_BUTTON_HEIGHT);
        tabButtonByDisableMode = new ImageButton(activity);
        tabButtonByDisableMode.setLayoutParams(btCloseParams);
        tabButtonByDisableMode.setBackgroundResource(activity.getResources().getIdentifier(
                bannerFrameImageColorInfo.getBannerTabClosed(), "drawable", activity.getPackageName()));
        try {
            tabButtonByDisableMode.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View v) {
                    Log.d(TAG, "Hide Button Clicked.");
                    if (!isShowingAnimation) {
                        try {
                            bodyLayout.setVisibility(View.GONE);
                            isShowingAnimation = true;
                            bodyLayout = (RelativeLayout)activity.getLayoutInflater().inflate(
                                    activity.getResources().getIdentifier("sample_cgp_promotion_default_layout", "layout",
                                            activity.getPackageName()), null);
                            setAnimationLayout();
                            setBannerLinearLayout();
                            setTabButtonByEnableMode();
                            setBannerButton();

                            bannerOuterLayoutByEnableMode.removeAllViews();
                            bannerLinearLayout.removeAllViews();
                            bodyLayout.removeAllViews();
                            bannerOuterLayoutByEnableMode.addView(bannerButton);
                            bannerLinearLayout.addView(tabButtonByEnableMode);
                            bannerLinearLayout.addView(bannerOuterLayoutByEnableMode);
                            animationLayout.addView(bannerLinearLayout);
                            bodyLayout.addView(animationLayout);
                            activity.addContentView(bodyLayout, new RelativeLayout.LayoutParams(
                                    LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
                            setLayoutAnimationSlideUpFromBottom(animationLayout);
                            activityHandler.sendEmptyMessageDelayed(0, BannerLayoutInfo.ANIMATION_DURATION);
                        } catch (Exception e) {
                            Log.e(TAG, e.getMessage(), e);
                        }
                    }
                }

                Handler activityHandler = new Handler() {
                    @Override
                    public void handleMessage(Message msg) {
                        isShowingAnimation = false;
                    }
                };
            });
        } catch (Exception e) {
            throw new Exception("Close button message handler error.");
        }
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
