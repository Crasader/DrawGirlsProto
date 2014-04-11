
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPDetailedProfile;
import com.hangame.hsp.HSPDetailedProfile.HSPIDPCode;
import com.hangame.hsp.HSPDetailedProfile.HSPPunishmentType;
import com.hangame.hsp.HSPProfile;
import com.hangame.hsp.HSPProfile.HSPLoadDetailedProfileCB;
import com.hangame.hsp.HSPProfile.HSPLoadProfilesCB;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;

import java.util.ArrayList;
import java.util.List;

public class HSPDetailedProfileAPI extends AbstractModule {
    private long mMemberNo = 4105000000131917L;

    private List<Long> mMemberList = null;

    public HSPDetailedProfileAPI() {
        super("HSPDetailedProfile");

        mMemberList = new ArrayList<Long>();
        mMemberList.add(mMemberNo);
    }

    public void testLoadUserDetailedProfile() {
        HSPProfile.loadProfiles(mMemberList, new HSPLoadProfilesCB() {
            @Override
            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                if (result.isSuccess()) {
                    if (profiles.size() == 0) {
                        log("No entry is exist.");
                    } else {
                        for (int index = 0; index < profiles.size(); index++) {
                            HSPProfile profile = profiles.get(index);

                            // Use HSPProfile Instance
                            profile.loadDetailedProfile(new HSPLoadDetailedProfileCB() {
                                @Override
                                public void onDetailedProfileLoad(HSPDetailedProfile detailedProfile, HSPResult result) {
                                    if (result.isSuccess()) {
                                        log("Is Admin: " + detailedProfile.isAdmin());
                                        log("RelationType From Me: " + detailedProfile.getRelationTypeFromMe());
                                        log("RelationType To Me: " + detailedProfile.getRelationTypeToMe());
                                        log("Todaywords: " + detailedProfile.getTodayWords());
                                        log("Age: " + detailedProfile.getAge());
                                        log("Gender: " + detailedProfile.getGender());
                                        log("Is Nickname changed: " + detailedProfile.isNicknameChanged());
                                        log("Age Exposed: " + detailedProfile.isAgeExposed());
                                        log("Gender Exposed: " + detailedProfile.isGenderExposed());
                                        log("Me2day Id exposed: "
                                                + detailedProfile.isIdpIDExposed(HSPIDPCode.HSP_IDP_ME2DAY));
                                        log("Twitter Id exposed: "
                                                + detailedProfile.isIdpIDExposed(HSPIDPCode.HSP_IDP_TWITTER));
                                        log("Profile Image Punishment Count: "
                                                + detailedProfile
                                                        .getPunishmentCount(HSPPunishmentType.HSP_PUNISHTYPE_PROFILEIMAGE));
                                        log("Profile Image Punishment End Date: "
                                                + detailedProfile
                                                        .getPunishmentEndDate(HSPPunishmentType.HSP_PUNISHTYPE_PROFILEIMAGE));
                                        log("Todaywords Punishment Count: "
                                                + detailedProfile
                                                        .getPunishmentCount(HSPPunishmentType.HSP_PUNISHTYPE_TODAYWORDS));
                                        log("Todaywords Punishment End Date: "
                                                + detailedProfile
                                                        .getPunishmentEndDate(HSPPunishmentType.HSP_PUNISHTYPE_TODAYWORDS));
                                        log("Me2day ID: " + detailedProfile.getIdpID(HSPIDPCode.HSP_IDP_ME2DAY));
                                        log("Twitter ID: " + detailedProfile.getIdpID(HSPIDPCode.HSP_IDP_TWITTER));
                                        log("User Game Data: " + detailedProfile.getGameUserData());
                                        for (final int playedGameNo : detailedProfile.getPlayedGameNoList()) {
                                            log("Played Game Number: " + playedGameNo);
                                        }
                                        detailedProfile
                                                .loadNameInAddressBook(new HSPDetailedProfile.HSPLoadNameInAddressBookCB() {
                                                    @Override
                                                    public void onNameLoad(String name, HSPResult result) {
                                                        if (result.isSuccess()) {
                                                            log("My Contact List: " + name);
                                                        } else {
                                                            log("My Contact List Load Failed: " + result);
                                                        }
                                                    }
                                                });
                                    } else {
                                        log("Load User detail profile is failed: " + result);
                                    }
                                }
                            });
                        }
                    }
                } else {
                    log("Load User profile is failed: " + result);
                }
            }
        });
    }
}
