
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPDetailedProfile.HSPGender;
import com.hangame.hsp.HSPDetailedProfile.HSPIDPCode;
import com.hangame.hsp.HSPMyProfile;
import com.hangame.hsp.HSPMyProfile.HSPLoadMyProfileCB;
import com.hangame.hsp.HSPMyProfile.HSPReportTodayWordsCB;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.sample.core.UiController;
import com.hangame.hsp.ui.DialogManager;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;

public class HSPMyProfileAPI extends AbstractModule {
    // myProfile change event
    private HSPMyProfile.HSPChangeMyProfileListener mChangeMyProfileListener;

    public HSPMyProfileAPI() {
        super("HSPMyProfile");
    }

    public void testloadMyProfile() {
        // Load my profile
        HSPMyProfile.loadMyProfile(new HSPLoadMyProfileCB() {
            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    log("Hangame ID: " + myProfile.getHangameID());
                    log("Use AddressBook: " + myProfile.useAddressBook());
                    log("Phone Number: " + myProfile.getPhoneNo());
                    log("Member Number: " + myProfile.getMemberNo());
                    log("Nickname: " + myProfile.getNickname());
                    log("Is Valid: " + myProfile.isValid());
                    log("OnlineExposed: " + myProfile.isOnlineExposed());
                    log("RecentPlayed GameNo: " + myProfile.getRecentPlayedGameNo());
                    log("Last Login Time: " + myProfile.getLastLoginDate());
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }

    public void testReportNickname() {
        // Load my profile
        HSPMyProfile.loadMyProfile(new HSPLoadMyProfileCB() {
            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    // Request change nickname
                    myProfile.reportNickname("Change my nickname", new HSPMyProfile.HSPReportNicknameCB() {
                        @Override
                        public void onReport(HSPResult result) {
                            if (result.isSuccess()) {
                                log("Change nickname is Success");
                            } else {
                                log("Change nickname is Fail: " + result);
                            }
                        }
                    });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }

    public void testReportTodayWords() {
        // Load my profile
        HSPMyProfile.loadMyProfile(new HSPLoadMyProfileCB() {
            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    // Request change todayword
                    myProfile.reportTodayWords("Change Todaywords", new HSPReportTodayWordsCB() {
                        @Override
                        public void onReport(HSPResult result) {
                            if (result.isSuccess()) {
                                log("Change Todaywords is Success");
                            } else {
                                log("Change Todaywords is Fail: " + result);
                            }
                        }
                    });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }

    public void testUploadProfileImage() {
        HSPMyProfile.loadMyProfile(new HSPMyProfile.HSPLoadMyProfileCB() {
            @Override
            public void onMyProfileLoad(HSPMyProfile profile, HSPResult result) {
                if (result.isSuccess()) {
                    Bitmap image = ((BitmapDrawable)UiController.getDrawable("test_image")).getBitmap();

                    profile.uploadProfileImage(image, new HSPMyProfile.HSPUploadProfileImageCB() {
                        @Override
                        public void onUpload(HSPResult result) {
                            if (result.isSuccess()) {
                                log("uploadProfileImage is Success");
                            } else {
                                log("uploadProfileImage is Fail: " + result);
                            }
                        }
                    });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }

        });
    }

    public void testDeleteProfileImage() {
        HSPMyProfile.loadMyProfile(new HSPMyProfile.HSPLoadMyProfileCB() {
            @Override
            public void onMyProfileLoad(HSPMyProfile profile, HSPResult result) {
                if (result.isSuccess()) {
                    profile.deleteProfileImage(new HSPMyProfile.HSPDeleteProfileImageCB() {

                        @Override
                        public void onDelete(HSPResult result) {
                            if (result.isSuccess()) {
                                log("deleteProfileImage is Success");
                            } else {
                                log("deleteProfileImage is Fail: " + result);
                            }
                        }
                    });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }

        });

    }

    public void testReportBirthDate() {
        HSPMyProfile.loadMyProfile(new HSPLoadMyProfileCB() {

            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    // Report birthday
                    myProfile.reportBirthDate(1982, 2, 16, new HSPMyProfile.HSPReportBirthDateCB() {
                        @Override
                        public void onReport(HSPResult result) {
                            if (result.isSuccess()) {
                                log("Birthday report is success.");
                            } else {
                                log("Birthday report is failed: " + result);
                            }
                        }
                    });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }

    public void testReportGender() {
        HSPMyProfile.loadMyProfile(new HSPLoadMyProfileCB() {

            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    // Report gender
                    myProfile.reportGender(HSPGender.HSP_GENDER_MALE, new HSPMyProfile.HSPReportGenderCB() {
                        @Override
                        public void onReport(HSPResult result) {
                            if (result.isSuccess()) {
                                log("Gender report is success.");
                            } else {
                                log("Gender report is failed: " + result);
                            }
                        }
                    });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }

    public void testConfirmToExposeOnline() {
        HSPMyProfile.loadMyProfile(new HSPLoadMyProfileCB() {

            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    // confirm to expose online
                    myProfile.confirmToExposeOnline(true, new HSPMyProfile.HSPConfirmToExposeOnlineCB() {
                        @Override
                        public void onConfirm(HSPResult result) {
                            if (result.isSuccess()) {
                                log("ConfirmToExposeOnline is success.");
                            } else {
                                log("ConfirmToExposeOnline is failed: " + result);
                            }
                        }
                    });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }

    public void testConfirmToExposeAge() {
        HSPMyProfile.loadMyProfile(new HSPLoadMyProfileCB() {

            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    // confirm to expose age
                    myProfile.confirmToExposeAge(true, new HSPMyProfile.HSPConfirmToExposeAgeCB() {
                        @Override
                        public void onConfirm(HSPResult result) {
                            if (result.isSuccess()) {
                                log("ConfirmToExposeAge is success.");
                            } else {
                                log("ConfirmToExposeAge is failed: " + result);
                            }
                        }
                    });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }

    public void testConfirmToExposeGender() {
        HSPMyProfile.loadMyProfile(new HSPLoadMyProfileCB() {

            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    // confirm to expose gender
                    myProfile.confirmToExposeGender(true, new HSPMyProfile.HSPConfirmToExposeGenderCB() {
                        @Override
                        public void onConfirm(HSPResult result) {
                            if (result.isSuccess()) {
                                log("ConfirmToExposeGender is success.");
                            } else {
                                log("ConfirmToExposeGender is failed: " + result);
                            }
                        }
                    });

                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }

    public void testConfirmToExposeIdpIDByMe2day() {
        HSPMyProfile.loadMyProfile(new HSPLoadMyProfileCB() {

            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    // confirm to expose me2day id
                    myProfile.confirmToExposeIdpID(HSPIDPCode.HSP_IDP_ME2DAY, true,
                            new HSPMyProfile.HSPConfirmToExposeIdpIDCB() {
                                @Override
                                public void onConfirm(HSPResult result) {
                                    if (result.isSuccess()) {
                                        log("ConfirmToExpose me2day id is success.");
                                    } else {
                                        log("ConfirmToExpose me2day id is failed: " + result);
                                    }
                                }
                            });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }

    public void testConfirmToExposeIdpIDByTwitter() {
        HSPMyProfile.loadMyProfile(new HSPLoadMyProfileCB() {

            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    // confirm to expose twitter id
                    myProfile.confirmToExposeIdpID(HSPIDPCode.HSP_IDP_TWITTER, true,
                            new HSPMyProfile.HSPConfirmToExposeIdpIDCB() {
                                @Override
                                public void onConfirm(HSPResult result) {
                                    if (result.isSuccess()) {
                                        log("ConfirmToExpose twitter id is success.");
                                    } else {
                                        log("ConfirmToExpose twitter id is failed: " + result);
                                    }
                                }
                            });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }

    public void testConfirmToUseAddressbook() {
        HSPMyProfile.loadMyProfile(new HSPLoadMyProfileCB() {

            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    // confirm to use address book
                    myProfile.confirmToUseAddressbook(true, new HSPMyProfile.HSPConfirmToUseAddressbookCB() {
                        @Override
                        public void onConfirm(HSPResult result) {
                            if (result.isSuccess()) {
                                log("ConfirmToUseAddressbook is success.");
                            } else {
                                log("ConfirmToUseAddressbook is failed: " + result);
                            }
                        }
                    });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }

    public void testReportGameUserData() {
        HSPMyProfile.loadMyProfile(new HSPMyProfile.HSPLoadMyProfileCB() {
            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    log("Load my profile success.");

                    String gameUserData = "[ [ { \"등급 - 1\" : \"중수\" }, { \"보유 머니 - 1\" : \"70만쩐\" }] ]";

                    myProfile.reportGameUserData(gameUserData, new HSPMyProfile.HSPReportGameUserDataCB() {
                        @Override
                        public void onReport(HSPResult result) {
                            if (result.isSuccess()) {
                                log("HSPMyProfile.reportGameUserData is success");
                            } else {
                                log("HSPMyProfile.reportGameUserData is failed: " + result);
                            }
                        }
                    });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }
    
    
    public void testUploadImageFromPath() {

        HSPMyProfile.loadMyProfile(new HSPLoadMyProfileCB() {

            @Override
            public void onMyProfileLoad(HSPMyProfile myProfile, HSPResult result) {
                if (result.isSuccess()) {
                    // confirm to use address book
                    DialogManager.showProgressDialog();
                    myProfile.uploadImageFromPath("/sdcard/Pictures/Screenshots/test.png", new HSPMyProfile.HSPUploadProfileImageCB() {
                        @Override
                        public void onUpload(HSPResult result) {
                            DialogManager.closeProgressDialog();
                            if (result.isSuccess()) {
                                log("UploadImageFromPath is success.");
                            } else {
                                log("UploadImageFromPath is failed: " + result);
                            }
                        }
                    });
                } else {
                    log("Load my profile is Failed: " + result);
                }
            }
        });
    }

    public void testRegisterListener() {
        // An event that is invoked when my profile is changed
        mChangeMyProfileListener = new HSPMyProfile.HSPChangeMyProfileListener() {
            @Override
            public void onMyProfileUseAddressBookChange(boolean expose) {
                // Whether to use their address book is called when changes are made
                log("onMyProfileUseAddressBookChange");
            }

            @Override
            public void onMyProfileTodayWordsChange(String todayWords) {
                // Called when todayword is changed
                log("onMyProfileTodayWordsChange");
            }

            @Override
            public void onMyProfilePhoneNoChange(String phoneNo) {
                // Called when phone number is changed
                log("onMyProfilePhoneNoChange");
            }

            @Override
            public void onMyProfileNicknameChange(String nickname) {
                // Called when nickname is changed
                log("onMyProfileNicknameChange");
            }

            @Override
            public void onMyProfileImageChange(Bitmap image) {
                // Called when profile image is changed
                log("onMyProfileImageChange");
            }

            @Override
            public void onMyProfileIdpIDChange() {
                // Called when sns id is changed
                log("onMyProfileIdpIDChange");
            }

            @Override
            public void onMyProfileGenderChange(HSPGender gender) {
                // Called when gender is changed
                log("onMyProfileGenderChange");
            }

            @Override
            public void onMyProfileGameUserDataChange(String gameUserData) {
                // Called when game data is changed
                log("onMyProfileGameUserDataChange");
            }

            @Override
            public void onMyProfileExposeOnlineChange(boolean expose) {
                // Called when my online expose is changed
                log("onMyProfileExposeOnlineChange");
            }

            @Override
            public void onMyProfileExposeIdpIDChange() {
                // Called when sns id expose information is changed
                log("onMyProfileExposeIdpIDChange");
            }

            @Override
            public void onMyProfileExposeGenderChange(boolean expose) {
                // Called when gender expose information is changed
                log("onMyProfileExposeGenderChange");
            }

            @Override
            public void onMyProfileExposeAgeChange(boolean expose) {
                // Called when age expose information is changed
                log("onMyProfileExposeAgeChange");
            }

            @Override
            public void onMyProfileBirthDateChange(int age) {
                // Called when birthday is changed
                log("onMyProfileBirthDateChange");
            }

            @Override
            public void onMyProfileAllPropertiesChange() {
                // Called when my all information is changed
                log("onMyProfileAllPropertiesChange");
            }
        };

        HSPMyProfile.addMyProfileChangeListener(mChangeMyProfileListener);
    }

    public void testUnregisterListener() {
        HSPMyProfile.removeMyProfileChangeListener(mChangeMyProfileListener);
    }
}
