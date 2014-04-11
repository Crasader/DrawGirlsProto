/*
 * @(#)SampleUtil.java $version 2011. 10. 26.
 *
 * Copyright 2011 NHN Corp. All rights Reserved. 
 * NHN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

package com.hangame.hsp.sample.util;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.Configuration;
import android.text.Html;
import android.text.SpannableStringBuilder;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Calendar;
import java.util.Locale;

/**
 * The Class SampleUtil.
 * 
 * @author Eom Iksang
 */
public class SampleUtil {
    /** The m progress dialog. */
    public static ProgressDialog mProgressDialog = null;

    /**
     * Toast msg.
     * 
     * @param activity the activity
     * @param message the message
     */
    public static void toastMsg(final Activity activity, final String message) {
        if (activity == null) {
            return;
        }
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(activity, message, Toast.LENGTH_SHORT).show();
            }
        });
    }

    /**
     * Prepend log entry.
     * 
     * @param logText the log text
     * @param cs the cs
     */
    private static void prependLogEntry(TextView logText, CharSequence cs) {
        SpannableStringBuilder contents = new SpannableStringBuilder(cs);
        contents.append('\n');
        contents.append(logText.getText());
        logText.setText(contents);
    }

    /**
     * Log text.
     * 
     * @param activity the activity
     * @param logText the log text
     * @param message the message
     */
    public static void logText(final Activity activity, final TextView logText, final String message) {
        if (activity == null) {
            return;
        }
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                SpannableStringBuilder contents = new SpannableStringBuilder();
                contents.append(Html.fromHtml("[" + getCurrentTime() + "] : "));
                contents.append(message);
                prependLogEntry(logText, contents);
            }
        });
    }

    /**
     * Gets the string.
     * 
     * @param context the context
     * @param tag the tag
     * @return the string
     */
    public static String getString(Context context, String tag) {
        int id = context.getResources().getIdentifier(tag, "string", context.getPackageName());
        if (id == 0) {
            return "";
        } else {
            String str = context.getResources().getString(id);
            return str;
        }
    }

    /**
     * Gets the current time.
     * 
     * @return the current time
     */
    public static String getCurrentTime() {
        Calendar cal = Calendar.getInstance();
        int year = cal.get(Calendar.YEAR);
        int month = (cal.get(Calendar.MONTH) + 1);
        int day = cal.get(Calendar.DAY_OF_MONTH);
        int hour = cal.get(Calendar.HOUR_OF_DAY);
        int minute = cal.get(Calendar.MINUTE);
        int second = cal.get(Calendar.SECOND);
        String currentTime = String.format("%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
        return currentTime;
    }

    /**
     * Pop dialog.
     * 
     * @param activity the activity
     * @param title the title
     * @param message the message
     * @param buttonName the button name
     */
    public static void popDialog(final Activity activity, final String title, final String message,
            final String buttonName) {
        if (activity == null) {
            return;
        }
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AlertDialog alertDialog = new AlertDialog.Builder(activity).setTitle(title).setMessage(message)
                        .setPositiveButton(buttonName, new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int whichButton) {
                                dialog.dismiss();
                            }
                        }).create();
                alertDialog.show();
            }
        });
    }

    /**
     * Show progress dialog.
     * 
     * @param activity the activity
     */
    public static void showProgressDialog(final Activity activity) {
        if (mProgressDialog == null || !mProgressDialog.isShowing()) {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mProgressDialog = ProgressDialog.show(activity, "Please wait...",
                            SampleUtil.getString(activity, "hsp.common.progress.message"));
                }
            });
        }
    }

    /**
     * Hide progress dialog.
     * 
     * @param activity the activity
     */
    public static void hideProgressDialog(final Activity activity) {
        if (mProgressDialog != null && mProgressDialog.isShowing()) {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mProgressDialog.dismiss();
                }
            });
        }
    }

    public static void setLocale(Context context, Locale locale) {
        if (locale != null) {
            Locale.setDefault(locale);
            Configuration config = new Configuration();

            config.locale = locale;
            context.getResources().updateConfiguration(config, null);
        }
    }

}
