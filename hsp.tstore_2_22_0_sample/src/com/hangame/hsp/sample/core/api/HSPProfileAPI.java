
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPProfile;
import com.hangame.hsp.HSPProfile.HSPLoadProfilesCB;
import com.hangame.hsp.HSPProfile.HSPRequestProileImageUrlsCB;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;

import android.graphics.Bitmap;

import java.util.ArrayList;
import java.util.List;

public class HSPProfileAPI extends AbstractModule {
    private long mMemberNo = 4105000000131917L;

    private List<Long> mMemberList = null;

    public HSPProfileAPI() {
        super("HSPProfile");

        mMemberList = new ArrayList<Long>();
        mMemberList.add(mMemberNo);
    }

    public void testLoadUserProfile() {
        HSPProfile.loadProfiles(mMemberList, new HSPLoadProfilesCB() {
            @Override
            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                if (result.isSuccess()) {
                    if (profiles.size() == 0) {
                        log("No entry is exist.");
                    } else {
                        for (int index = 0; index < profiles.size(); index++) {
                            HSPProfile profile = profiles.get(index);

                            log("Member Number: " + profile.getMemberNo());
                            log("Nickname: " + profile.getNickname());
                            log("Is Valid: " + profile.isValid());
                            log("OnlineExposed: " + profile.isOnlineExposed());
                            log("RecentPlayed GameNo: " + profile.getRecentPlayedGameNo());
                            log("Last Login Time : " + profile.getLastLoginDate());
                        }
                    }
                } else {
                    log("Load User profile is failed: " + result);
                }
            }
        });
    }

    public void testGetProfileImageUrl() {
        // user memberNo
        long memberNo = HSPCore.getInstance().getMemberNo();

        String profileThumbnailImageUrl = HSPProfile.getProfileImageURL(memberNo, true);

        log("Profile Thumbnail Image URL: " + profileThumbnailImageUrl);

        String ProfileImageUrl = HSPProfile.getProfileImageURL(memberNo, false);

        log("Profile Image URL: " + ProfileImageUrl);
    }

    public void testGetProfileImageUrls() {
        List<Long> memberNos = new ArrayList<Long>();
        memberNos.add(HSPCore.getInstance().getMemberNo());

        HSPProfile.requestProfileImageUrls(memberNos, true, new HSPRequestProileImageUrlsCB() {

            @Override
            public void onProfileImageUrlsReceive(List<String> imageUrls, HSPResult result) {
                if (result.isSuccess()) {
                    if (imageUrls.size() == 0) {
                        log("Profile Thumbnail Image Urls is empty!!! ");
                    } else {
                        for (String url : imageUrls) {
                            log("Profile Thumbnail Image Url: " + url);
                        }
                    }
                } else {
                    log("Profile Thumbnail Image Urls request fail: " + result);
                }

            }
        });

        HSPProfile.requestProfileImageUrls(memberNos, false, new HSPRequestProileImageUrlsCB() {

            @Override
            public void onProfileImageUrlsReceive(List<String> imageUrls, HSPResult result) {
                if (result.isSuccess()) {
                    if (imageUrls.size() == 0) {
                        log("Profile Image Urls is empty!!! ");
                    } else {
                        for (String url : imageUrls) {
                            log("Profile Image Url: " + url);
                        }
                    }
                } else {
                    log("Profile Image Urls request fail: " + result);
                }

            }
        });
    }

    public void testDownloadProfileImage() {
        HSPProfile.loadProfiles(mMemberList, new HSPLoadProfilesCB() {
            @Override
            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                if (result.isSuccess()) {
                    HSPProfile profile = profiles.get(0);

                    if (profile != null) {
                        log("Profile Image URL via member function : " + profile.getProfileImageURL(true));

                        // Download Profile Image
                        profile.downloadProfileImage(true, new HSPProfile.HSPDownloadProfileImageCB() {

                            @Override
                            public void onProfileImageDownload(Bitmap image, HSPResult result) {
                                if (result.isSuccess()) {
                                    log("Profile Image: " + image);
                                } else {
                                    log("Profile Image download failed: " + result);
                                }
                            }
                        });
                    }
                } else {
                    log("Load User profile is failed: " + result);
                }
            }
        });

        String profileImageUrl = HSPProfile.getProfileImageURL(mMemberNo, true);
        log("Profile Image URL via static function: " + profileImageUrl);
    }
}
