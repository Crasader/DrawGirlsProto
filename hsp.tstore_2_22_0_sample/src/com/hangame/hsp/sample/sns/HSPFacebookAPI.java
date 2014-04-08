
package com.hangame.hsp.sample.sns;

import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.sample.core.UiController;
import com.hangame.hsp.sns.HSPFacebook;
import com.hangame.hsp.sns.HSPFacebook.HSPFacebookFeedByUICB;
import com.hangame.hsp.sns.HSPFacebook.HSPFacebookFeedCB;
import com.hangame.hsp.sns.HSPFacebook.HSPFacebookLoginCB;
import com.hangame.hsp.sns.HSPFacebook.HSPFacebookLogoutCB;
import com.hangame.hsp.sns.HSPFacebook.HSPFacebookQueryHSPMemberNosCB;
import com.hangame.hsp.sns.HSPFacebook.HSPFacebookSendAppRequestCB;
import com.hangame.hsp.sns.HSPFacebook.HSPFacebookUploadImageByUICB;
import com.hangame.hsp.sns.HSPFacebook.HSPFacebookUploadImageCB;
import com.hangame.hsp.sns.HSPFacebook.HSPFacebookVerifyAuthenticationCB;
import com.hangame.hsp.sns.HSPFacebookProfile;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class HSPFacebookAPI extends AbstractModule {

    public HSPFacebookAPI() {
        super("HSPFacebook");
    }

    public void testLogin() {
        
        HSPFacebook.login(true, new HSPFacebookLoginCB() {
            @Override
            public void onLogin(HSPResult result) {
                if (result.isSuccess()) {
                    log("Facebook Login SUCCESS!!!");
                } else {
                    log("Facebook Login Fail: " + result);
                }
            }
        });
    }

    public void testLogout() {
        HSPFacebook.logout(new HSPFacebookLogoutCB() {
            @Override
            public void onLogout(HSPResult result) {
                if (result.isSuccess()) {
                    log("Facebook Logout SUCCESS!!!");
                } else {
                    log("Facebook Logout Fail: " + result);
                }
            }
        });
    }

    public void testVerifyAhentication() {
        HSPFacebook.verifyAuthentication(new HSPFacebookVerifyAuthenticationCB() {
            @Override
            public void onAuthenticationVerify(final Map<String, Object> data, final HSPResult result) {
                if (result.isSuccess()) {
                    log("Facebook verify SUCCESS!!!");
                } else {
                    log("Facebook verify Fail: " + result);
                }
            }
        });
    }

    public void testFeedMessage() {
        HSPFacebook
                .feed(true,
                        "Facebook test feed message",
                        "http://static.naver.net/www/u/2010/0611/nmms_215646753.gif",
                        "naver",
                        "http://news.naver.com/sports/index.nhn?category=worldfootball&ctg=news&mod=read&office_id=311&article_id=0000129131",
                        null, null, new HSPFacebookFeedCB() {
                            @Override
                            public void onFeed(HSPResult result) {
                                if (result.isSuccess()) {
                                    log("Facebook feed SUCCESS!!!");
                                } else {
                                    log("Facebook feed Fail: " + result);
                                }
                            }
                        });
    }
    
    public void testGetMyFriendList() {
        
        List<String> fbIdList = new ArrayList<String>();
        fbIdList.add( "100001567626961");
        fbIdList.add( "100002193060172");
        fbIdList.add( "100005846133482");
                
        HSPFacebook
                .queryHSPMemberNos(
                        fbIdList,
                        new HSPFacebookQueryHSPMemberNosCB() {

                            @Override
                            public void onHSPMemberNosReceive(List<HSPFacebookProfile> fbFriendList,
                                    HSPResult result) {
                                if (result.isSuccess()) {
                                    log("Facebook get friend list SUCCESS!!!" + fbFriendList);
                                 } else {
                                     log("Facebook get friend list Fail!!!" + result);
                                 }
                                
                            }
                        });
    }
    
    public void testSendAppRequest() {
        List<String> fbIdList = new ArrayList<String>();
        fbIdList.add( "100002193060172");
        fbIdList.add( "100001567626961");
        
        HSPFacebook
        .sendAppRequest(fbIdList, "testSendAppRequestMessage",
                new HSPFacebookSendAppRequestCB() {
                    
                    @Override
                    public void onAppRequestSend(HSPResult result) {
                        // TODO Auto-generated method stub
                        if (result.isSuccess()) {
                            log("Facebook feed SUCCESS!!!");
                        } else {
                            log("Facebook feed Fail: " + result);
                        }
                    }
        });
    }
    
    public void testFeedImage() {
        Bitmap image = ((BitmapDrawable)UiController.getDrawable("test_image")).getBitmap();

        HSPFacebook.uploadImage(true, "test_image!!!", image, new HSPFacebookUploadImageCB() {
            @Override
            public void onImageUpload(HSPResult result) {
                if (result.isSuccess()) {
                    log("Facebook feed Image SUCCESS!!!");
                } else {
                    log("Facebook feed Image Fail: " + result);
                }
            }
        });
    }

    public void testFeedImagePath() {
        String imagePath = "/sdcard/DCIM/Camera/!1aㄱ.jpg";

        HSPFacebook.uploadImage(true, "test_image!!!", imagePath, new HSPFacebookUploadImageCB() {
            @Override
            public void onImageUpload(HSPResult result) {
                if (result.isSuccess()) {
                    log("Facebook feed Image SUCCESS!!!");
                } else {
                    log("Facebook feed Image Fail: " + result);
                }
            }
        });
    }

    public void testFeedMessageUI() {
        HSPFacebook
                .feedByUI(
                        true,
                        null,
                        "http://static.naver.net/www/u/2010/0611/nmms_215646753.gif",
                        "naver",
                        "http://news.naver.com/sports/index.nhn?category=worldfootball&ctg=news&mod=read&office_id=311&article_id=0000129131",
                        "Test caption", "Test description", new HSPFacebookFeedByUICB() {
                            @Override
                            public void onFeed(HSPResult result) {
                                if (result.isSuccess()) {
                                    log("Facebook feed ui SUCCESS!!!");
                                } else {
                                    log("Facebook feed ui Fail: " + result);
                                }
                            }
                        });
    }

    public void testFeedImageUI() {
        Bitmap image = ((BitmapDrawable)UiController.getDrawable("test_image")).getBitmap();

        HSPFacebook.uploadImageByUI(true, null, image, new HSPFacebookUploadImageByUICB() {
            @Override
            public void onImageUpload(HSPResult result) {
                if (result.isSuccess()) {
                    log("Facebook feed Image ui SUCCESS!!!");
                } else {
                    log("Facebook feed Image ui Fail: " + result);
                }
            }
        });
    }

    public void testFeedImagePathUI() {
        // String imagePath = "/data/data/" + getPackageName() + "/assets/test_image.png";
        String imagePath = "/sdcard/DCIM/Camera/!1aㄱ.jpg";

        HSPFacebook.uploadImageByUI(true, null, imagePath, new HSPFacebookUploadImageByUICB() {
            @Override
            public void onImageUpload(HSPResult result) {
                if (result.isSuccess()) {
                    log("Facebook feed Image ui SUCCESS!!!");
                } else {
                    log("Facebook feed Image ui Fail: " + result);
                }
            }
        });
    }

    public void testGetOAuth() {
        if (HSPFacebook.getOAuthInfo() != null) {
            log("Facebook OAuth ServiceProvider: " + HSPFacebook.getOAuthInfo().getServiceProvider());
            log("Facebook OAuth ConsumerKey: " + HSPFacebook.getOAuthInfo().getConsumerKey());
            log("Facebook OAuth ConsumerSecret: " + HSPFacebook.getOAuthInfo().getConsumerSecret());
            log("Facebook OAuth hasAccessToken: " + HSPFacebook.getOAuthInfo().hasAccessToken());
            log("Facebook OAuth AccessToken: " + HSPFacebook.getOAuthInfo().getAccessToken());
        } else {
            log("Facebook OAuth is null!!");
        }
    }
}
