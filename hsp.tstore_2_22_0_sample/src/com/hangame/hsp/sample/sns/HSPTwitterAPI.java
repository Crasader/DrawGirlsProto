
package com.hangame.hsp.sample.sns;

import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.sample.core.UiController;
import com.hangame.hsp.sns.HSPTwitter;
import com.hangame.hsp.sns.HSPTwitter.HSPTwitterFeedByUICB;
import com.hangame.hsp.sns.HSPTwitter.HSPTwitterFeedCB;
import com.hangame.hsp.sns.HSPTwitter.HSPTwitterLoginCB;
import com.hangame.hsp.sns.HSPTwitter.HSPTwitterLogoutCB;
import com.hangame.hsp.sns.HSPTwitter.HSPTwitterVerifyAuthenticationCB;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;

import java.util.Map;

public class HSPTwitterAPI extends AbstractModule {

    public HSPTwitterAPI() {
        super("HSPTwitter");
    }

    public void testTwitterLogin() {
        HSPTwitter.login(true, new HSPTwitterLoginCB() {
            @Override
            public void onLogin(HSPResult result) {
                if (result.isSuccess()) {
                    log("Twitter Login SUCCESS!!!");
                } else {
                    log("Twitter Login Fail: " + result);
                }
            }
        });
    }

    public void testTwitterLogout() {
        HSPTwitter.logout(new HSPTwitterLogoutCB() {
            @Override
            public void onLogout(HSPResult result) {
                if (result.isSuccess()) {
                    log("Twitter Logout SUCCESS!!!");
                } else {
                    log("Twitter Logout Fail: " + result);
                }
            }
        });
    }

    public void testTwitterVerifyAhentication() {
        HSPTwitter.verifyAuthentication(new HSPTwitterVerifyAuthenticationCB() {
            @Override
            public void onAuthenticationVerify(final Map<String, Object> data, final HSPResult result) {
                if (result.isSuccess()) {
                    log("Twitter verifyAuthentication SUCCESS!!!");
                } else {
                    log("Twitter verifyAuthentication Fail: " + result);
                }
            }
        });
    }

    public void testTwitterFeedMessage() {
        HSPTwitter.feed(true, "Hello~", new HSPTwitterFeedCB() {
            @Override
            public void onFeed(HSPResult result) {
                if (result.isSuccess()) {
                    log("Twitter feed SUCCESS!!!");
                } else {
                    log("Twitter feed Fail: " + result);
                }
            }
        });
    }

    public void testTwitterFeedImage() {
        Bitmap image = ((BitmapDrawable)UiController.getDrawable("test_image")).getBitmap();

        HSPTwitter.feed(true, "Hangame Test!!!!", image, new HSPTwitterFeedCB() {
            @Override
            public void onFeed(HSPResult result) {
                if (result.isSuccess()) {
                    log("Twitter feed Image SUCCESS!!!");
                } else {
                    log("Twitter feed Image Fail: " + result);
                }
            }
        });
    }

    public void testTwitterFeedUI() {
        Bitmap image = ((BitmapDrawable)UiController.getDrawable("test_image")).getBitmap();

        HSPTwitter.feedByUI(true, null, image, new HSPTwitterFeedByUICB() {
            @Override
            public void onFeed(HSPResult result) {
                if (result.isSuccess()) {
                    log("Twitter feed ui SUCCESS!!!");
                } else {
                    log("Twitter feed ui Fail: " + result);
                }
            }
        });
    }

    public void testTwitterFeedImagePath() {
        String imagePath = "/sdcard/DCIM/Camera/!1aㄱ.jpg";

        HSPTwitter.feed(true, "Hangame Test!!!!", imagePath, new HSPTwitterFeedCB() {
            @Override
            public void onFeed(HSPResult result) {
                if (result.isSuccess()) {
                    log("Twitter feed Image Path SUCCESS!!!");
                } else {
                    log("Twitter feed Image Path Fail: " + result);
                }
            }
        });
    }

    public void testTwitterFeedImagePathUI() {
        String imagePath = "/sdcard/DCIM/Camera/!1aㄱ.jpg";

        HSPTwitter.feedByUI(true, null, imagePath, new HSPTwitterFeedByUICB() {
            @Override
            public void onFeed(HSPResult result) {
                if (result.isSuccess()) {
                    log("Twitter feed ui SUCCESS!!!");
                } else {
                    log("Twitter feed ui Fail: " + result);
                }
            }
        });
    }

    public void testTwitterFeedUIWithoutImage() {
        HSPTwitter.feedByUI(true, null, new HSPTwitterFeedByUICB() {
            @Override
            public void onFeed(HSPResult result) {
                if (result.isSuccess()) {
                    log("Twitter feed ui SUCCESS!!!");
                } else {
                    log("Twitter feed ui Fail: " + result);
                }
            }
        });
    }

    public void testTwitterGetOAuth() {
        log("HSPTwitter.getOAuthInfo()");

        if (HSPTwitter.getOAuthInfo() != null) {
            log("ServiceProvider: " + HSPTwitter.getOAuthInfo().getServiceProvider());
            log("ConsumerKey: " + HSPTwitter.getOAuthInfo().getConsumerKey());
            log("ConsumerSecret: " + HSPTwitter.getOAuthInfo().getConsumerSecret());
            log("hasAccessToken: " + HSPTwitter.getOAuthInfo().hasAccessToken());
            log("AccessToken: " + HSPTwitter.getOAuthInfo().getAccessToken());
            log("AccessSecret: " + HSPTwitter.getOAuthInfo().getAccessTokenSecret());
        } else {
            log("AuthInfo is null!!");
        }
    }
}
