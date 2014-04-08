/*
 * @(#)SampleDrawer.java $version 2011. 10. 26.
 *
 * Copyright 2011 NHN Corp. All rights Reserved. 
 * NHN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

package com.hangame.hsp.sample.promotion;

import com.hangame.hsp.cgp.HSPCGP;
import com.hangame.hsp.sample.util.SampleUtil;
import com.hangame.hsp.util.Log;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

/**
 * The Class SampleDrawer.
 * 
 * @author Eom Iksang
 */
public class CGPButtonBridgeView {

    /** The TAG. */
    private final String TAG = "CGPButtonBridgeView";

    /** The instance. */
    private static CGPButtonBridgeView instance = null;

    /**
     * Instantiates a new sample drawer.
     */
    private CGPButtonBridgeView() {
    }

    /**
     * Gets the single instance of SampleDrawer.
     * 
     * @return single instance of SampleDrawer
     */
    public static CGPButtonBridgeView getInstance() {
        if (instance == null) {
            instance = new CGPButtonBridgeView();
        }
        return instance;
    }

    /** The button layout. */
    private static RelativeLayout buttonLayout;

    /**
     * Draw button.
     * 
     * @param activity the activity
     */
    public void drawButton(final CGPBaseActivity activity, final boolean installRewardCheck) {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (buttonLayout != null) {
                    buttonLayout.removeAllViews();
                    buttonLayout.setVisibility(RelativeLayout.INVISIBLE);
                }
                buttonLayout = (RelativeLayout)activity.getLayoutInflater().inflate(
                        activity.getResources().getIdentifier("sample_cgp_promotion_button_layout", "layout",
                                activity.getPackageName()), null);
                LinearLayout linearLayout = new LinearLayout(activity);
                linearLayout.setOrientation(LinearLayout.HORIZONTAL);
                // linearLayout.setBackgroundColor(Color.MAGENTA);
                setLLParams(linearLayout);

                ImageButton button = new ImageButton(activity);
                setLLParams(button);
                Bitmap bm = null;
                if(installRewardCheck == true) {
                    bm = HSPCGP.getPromotionInfo().getButtonImg_install();
                } else {
                    bm = HSPCGP.getPromotionInfo().getButtonImg();
                }
                button.setImageBitmap(bm);
                button.setOnClickListener(new OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Log.d(TAG, "Show Button Clicked.");
                        SampleUtil.logText(activity, activity.getLogTextView(), "Button clicked. : launchPromotion");
                        HSPCGP.launchPromotion(activity);
                        buttonLayout.removeAllViews();
                        buttonLayout.setVisibility(RelativeLayout.INVISIBLE);
                    }
                });
                TextView bubbleText = new TextView(activity);
                bubbleText.setText(HSPCGP.getPromotionInfo().getBubbleText());
                bubbleText.setTextColor(Color.YELLOW);
                setLLParams(bubbleText);

                linearLayout.addView(button);
                linearLayout.addView(bubbleText);

                buttonLayout.addView(linearLayout);
                activity.addContentView(buttonLayout, new RelativeLayout.LayoutParams(LayoutParams.FILL_PARENT,
                        LayoutParams.FILL_PARENT));
            }
        });
    }

    /**
     * Sets the lL params.
     * 
     * @param view the new lL params
     */
    private void setLLParams(View view) {
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT);
        view.setLayoutParams(params);
    }
}
