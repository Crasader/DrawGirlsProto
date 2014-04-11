
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPDetailedProfile;
import com.hangame.hsp.HSPDetailedProfile.HSPIDPCode;
import com.hangame.hsp.HSPProfile;
import com.hangame.hsp.HSPProfile.HSPLoadDetailedProfileCB;
import com.hangame.hsp.HSPProfile.HSPLoadProfilesCB;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.HSPSocial;
import com.hangame.hsp.HSPSocial.HSPBlockMembersCB;
import com.hangame.hsp.HSPSocial.HSPFollowMembersCB;
import com.hangame.hsp.HSPSocial.HSPQueryFollowersCB;
import com.hangame.hsp.HSPSocial.HSPQueryFollowingMembersCB;
import com.hangame.hsp.HSPSocial.HSPQueryMembersPlayedGameCB;
import com.hangame.hsp.HSPSocial.HSPQueryMembersRecommendedCB;
import com.hangame.hsp.HSPSocial.HSPQueryMembersWithNicknameCB;
import com.hangame.hsp.auth.login.LoginService;
import com.hangame.hsp.sample.AbstractModule;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class HSPSocialAPI extends AbstractModule {
    private List<Long> mMemberNos = null;

    // social change event
    private HSPSocial.HSPAddFollowingListener mAddFollowingListener;

    private HSPSocial.HSPRemoveFollowingListener mRemoveFollowingListener;

    private HSPSocial.HSPAddBlockingListener mAddBlockingListener;

    private HSPSocial.HSPRemoveBlockingListener mRemoveBlockingListener;

    public HSPSocialAPI() {
        super("HSPSocial");

        long memberNo = 4105000000134579L;
        mMemberNos = new ArrayList<Long>();
        mMemberNos.add(memberNo);
    }

    public void testFollowMembers() {
        // 추가할 User의 memberNo = 4105000000134579

        HSPSocial.followMembers(mMemberNos, new HSPFollowMembersCB() {
            @Override
            public void onMembersFollow(List<Long> memberNos, HSPResult result) {
                if (result.isSuccess()) {
                    if (memberNos.size() == 0) {
                        log("No entry is exist.");
                    } else {
                        HSPProfile.loadProfiles(memberNos, new HSPLoadProfilesCB() {
                            @Override
                            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                                if (result.isSuccess()) {
                                    if (profiles != null) {
                                        HSPProfile profile = profiles.get(0);

                                        log("User member number: " + profile.getMemberNo());
                                        log("Nickname: " + profile.getNickname());

                                        profile.loadDetailedProfile(new HSPLoadDetailedProfileCB() {
                                            @Override
                                            public void onDetailedProfileLoad(HSPDetailedProfile detailedProfile,
                                                    HSPResult result) {
                                                if (result.isSuccess()) {
                                                    log("RelationType From Me: "
                                                            + detailedProfile.getRelationTypeFromMe());
                                                } else {
                                                    log("User Detail Profile fails to load: " + result);
                                                }
                                            }
                                        });
                                    }
                                } else {
                                    log("User Profile fails to load: " + result);
                                }
                            }
                        });
                    }
                } else {
                    log("Follow members fail: " + result);
                }
            }
        });
    }

    public void testBlockMembers() {
        // 차단할 User의 memberNo = 4105000000134579

        HSPSocial.blockMembers(mMemberNos, new HSPBlockMembersCB() {
            @Override
            public void onMembersBlock(List<Long> memberNos, HSPResult result) {
                if (result.isSuccess()) {
                    if (memberNos.size() == 0) {
                        log("No entry is exist.");
                    } else {
                        HSPProfile.loadProfiles(memberNos, new HSPLoadProfilesCB() {
                            @Override
                            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                                if (result.isSuccess()) {
                                    if (profiles != null) {
                                        HSPProfile profile = profiles.get(0);
                                        log("User Member number: " + profile.getMemberNo());
                                        log("Nickname: " + profile.getNickname());

                                        profile.loadDetailedProfile(new HSPLoadDetailedProfileCB() {
                                            @Override
                                            public void onDetailedProfileLoad(HSPDetailedProfile detailedProfile,
                                                    HSPResult result) {
                                                if (result.isSuccess()) {
                                                    log("RelationTypeFromMe: "
                                                            + detailedProfile.getRelationTypeFromMe());
                                                } else {
                                                    log("User Detail Profile fails to load: " + result);
                                                }
                                            }
                                        });
                                    }
                                } else {
                                    log("User Profile fails to load: " + result);
                                }
                            }
                        });
                    }
                } else {
                    log("Block members fail: " + result);
                }
            }
        });
    }

    public void testUnfollowMembers() {
        // Remove from follow list( Not Block )
        HSPSocial.unfollowMembers(mMemberNos, new HSPSocial.HSPUnfollowMembersCB() {
            @Override
            public void onMembersUnfollow(HSPResult result) {
                if (result.isSuccess()) {
                    log("Remove from Follow List has been successful.");
                } else {
                    log("Failed to Remove from Follow List: " + result);
                }
            }
        });
    }

    public void testUnblockMembers() {
        // Remove from blocking list
        HSPSocial.unblockMembers(mMemberNos, new HSPSocial.HSPUnblockMembersCB() {
            @Override
            public void onMembersUnblock(HSPResult result) {
                if (result.isSuccess()) {
                    log("Remove from blocking list has been successful.");
                } else {
                    log("Failed to Remove from blocking List: " + result);
                }
            }
        });
    }

    public void testQueryFollowingMemberCount() {
        // The number of following list
        HSPSocial.queryFollowingMemberCount(new HSPSocial.HSPQueryFollowingMemberCountCB() {
            @Override
            public void onMemberCountReceive(int count, HSPResult result) {
                if (result.isSuccess()) {
                    log("The number of following list: " + count);
                } else {
                    log("Failed to load following list: " + result);
                }
            }
        });
    }

    public void testQueryBlockingMemberCount() {
        // The number of blocking list
        HSPSocial.queryBlockingMemberCount(new HSPSocial.HSPQueryBlockingMemberCountCB() {
            @Override
            public void onMemberCountReceive(int count, HSPResult result) {
                if (result.isSuccess()) {
                    log("The number of blocking list: " + count);
                } else {
                    log("Failed to load blocking list: " + result);
                }
            }
        });
    }

    public void testQueryFollowingMembers() {
        // query following members
        HSPSocial.queryFollowingMembers(0, 50, new HSPQueryFollowingMembersCB() {
            @Override
            public void onMembersReceive(List<Long> memberNos, HSPResult result) {
                if (result.isSuccess()) {
                    if (memberNos.size() == 0) {
                        log("No Friend List exist.");
                    } else {
                        HSPProfile.loadProfiles(memberNos, new HSPLoadProfilesCB() {
                            @Override
                            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                                if (result.isSuccess()) {
                                    for (final HSPProfile profile : profiles) {
                                        log("User Member Number: " + profile.getMemberNo());
                                        log("Nickname: " + profile.getNickname());
                                        log("Last Login Time: " + profile.getLastLoginDate());
                                    }
                                } else {
                                    log("User Profile fails to load: " + result);
                                }
                            }
                        });
                    }
                } else {
                    log("Following Member fails to load : " + result);
                }
            }
        });
    }

    public void testQueryRecommendedMembers() {
        HSPSocial.queryMembersRecommended(HSPCore.getInstance().getGameNo(), 0, 30, new HSPQueryMembersRecommendedCB() {
            @Override
            public void onMembersReceive(List<Long> memberNos, HSPResult result) {
                if (result.isSuccess()) {
                    if (memberNos.size() == 0) {
                        log("No entry is exist.");
                    } else {
                        HSPProfile.loadProfiles(memberNos, new HSPLoadProfilesCB() {
                            @Override
                            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                                if (result.isSuccess()) {
                                    for (final HSPProfile profile : profiles) {
                                        log("User Member Number: " + profile.getMemberNo());
                                        log("Nickname: " + profile.getNickname());
                                        log("Last Login Time: " + profile.getLastLoginDate());
                                    }
                                } else {
                                    log("User Profile fails to load: " + result);
                                }
                            }
                        });
                    }
                } else {
                    log("Recommneded Member fails to load: " + result);
                }
            }
        });
    }

    public void testQueryMembersWithNickname() {
        // 별명으로 검색된 이용자의 목록
        String searchNickname = "player";
        HSPSocial.queryMembersWithNickname(searchNickname, 0, 10, new HSPQueryMembersWithNicknameCB() {
            @Override
            public void onMembersReceive(List<Long> memberNos, HSPResult result) {
                if (result.isSuccess()) {
                    if (memberNos.size() == 0) {
                        log("No entry is exist.");
                    } else {
                        HSPProfile.loadProfiles(memberNos, new HSPLoadProfilesCB() {
                            @Override
                            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                                if (result.isSuccess()) {
                                    for (final HSPProfile profile : profiles) {
                                        log("User member number: " + profile.getMemberNo());
                                        log("Nickname: " + profile.getNickname());
                                        log("Last Login Time: " + profile.getLastLoginDate());
                                    }
                                } else {
                                    log("User Profile fails to load: " + result);
                                }
                            }
                        });
                    }
                } else {
                    log("Querying with nickname user list fails to load: " + result);
                }
            }
        });
    }

    public void testQueryFollowers() {
        // 나를 친구로 등록한 이용자의 목록 요청
        HSPSocial.queryFollowers(0, 10, new HSPQueryFollowersCB() {
            @Override
            public void onMembersReceive(List<Long> memberNos, HSPResult result) {
                if (result.isSuccess()) {
                    if (memberNos.size() == 0) {
                        log("No entry is exist.");
                    } else {
                        HSPProfile.loadProfiles(memberNos, new HSPProfile.HSPLoadProfilesCB() {
                            @Override
                            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                                if (result.isSuccess()) {
                                    for (final HSPProfile profile : profiles) {
                                        log("User Member Number: " + profile.getMemberNo());
                                        log("Nickname: " + profile.getNickname());
                                        log("Last Login Time: " + profile.getLastLoginDate());
                                    }
                                } else {
                                    log("User Profile fails to load: " + result);
                                }
                            }
                        });
                    }
                } else {
                    log("Querying followers list fails to load: " + result);
                }
            }
        });
    }

    public void testQueryMembersPlayedGame() {
        // A list of users that played a particular game
        HSPSocial.queryMembersPlayedGame(HSPCore.getInstance().getGameNo(), 0, 10, new HSPQueryMembersPlayedGameCB() {
            @Override
            public void onMembersReceive(List<Long> memberNos, HSPResult result) {
                if (result.isSuccess()) {
                    if (memberNos.size() == 0) {
                        log("No entry is exist.");
                    } else {
                        HSPProfile.loadProfiles(memberNos, new HSPLoadProfilesCB() {
                            @Override
                            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                                if (result.isSuccess()) {
                                    for (final HSPProfile profile : profiles) {
                                        log("User Member Number: " + profile.getMemberNo());
                                        log("Nickname: " + profile.getNickname());
                                        log("Last Login Time: " + profile.getLastLoginDate());
                                    }
                                } else {
                                    log("User Profile fails to load: " + result);
                                }
                            }
                        });
                    }
                } else {
                    log("Querying members with particular game list fails to load: " + result);
                }
            }
        });
    }

    public void testQueryBlockingMembers() {
        // Load blocking list
        HSPSocial.queryBlockingMembers(0, 10, new HSPSocial.HSPQueryBlockingMembersCB() {
            @Override
            public void onMembersReceive(List<Long> memberNos, HSPResult result) {
                if (result.isSuccess()) {
                    log("Success to load blocking list");
                    
                    for (final long memberNo : memberNos) {
                        log("User Member Number: " + memberNo);
                    }
                } else {
                    log("Failed to load blocking list: " + result);
                }
            }
        });
    }

    public void testQueryFollowingMembersPlayedGame() {
        // Load following list with particular game played
        HSPSocial.queryFollowingMembersPlayedGame(HSPCore.getInstance().getGameNo(), 0, 10,
                new HSPSocial.HSPQueryFollowingMembersPlayedGameCB() {
                    @Override
                    public void onMembersReceive(List<Long> memberNos, HSPResult result) {
                        if (result.isSuccess()) {
                            log("Success to load list");
                            
                            for (final long memberNo : memberNos) {
                                log("User member number: " + memberNo);
                            }
                        } else {
                            log("Failed to load list: " + result);
                        }
                    }
                });
    }

    public void testQueryFollowingMemberIDss() {
        // Load following list with me2day id
        HSPSocial.queryFollowingMemberIDs(HSPIDPCode.HSP_IDP_ME2DAY, 0, 10,
                new HSPSocial.HSPQueryFollowingMemberIDsCB() {

                    @Override
                    public void onMemberIDsReceive(List<Long> memberNos, List<String> idpIds, HSPResult result) {
                        if (result.isSuccess()) {
                            log("Success to load list");
                            
                            for (int index = 0; index < memberNos.size(); index++) {
                                long memberNo = memberNos.get(index);
                                String me2dayId = idpIds.get(index);
                                log("User member number: " + memberNo + ", me2dayId : " + me2dayId);
                            }
                        } else {
                            log("Failed to load list: " + result);
                        }
                    }
                });
    }

    public void testQueryMembersWithPhoneNo() {
        // A particular user with a specific phone number from the list of search the user list
        List<String> phoneNos = new ArrayList<String>();
        phoneNos.add("01044812788");
        HSPSocial.queryMembersWithPhoneNo(phoneNos, mMemberNos, new HSPSocial.HSPQueryMembersWithPhoneNoCB() {
            @Override
            public void onMembersReceive(Map<String, Long> memberNoMap, HSPResult result) {
                if (result.isSuccess()) {
                    log("User with a phone number: " + memberNoMap);
                } else {
                    log("Failed to load list: " + result);
                }
            }
        });
    }

    public void testSendRecommendingSMS() {
        // Request recommending SMS
        HSPSocial.sendRecommendingSMS("01044812788", "01044812788", "test", new HSPSocial.HSPSendRecommendingSMSCB() {
            @Override
            public void onSMSSend(HSPResult result) {
                if (result.isSuccess()) {
                    log("Recommended SMS request succeeded");
                } else {
                    log("Recommended SMS request failed: " + result);
                }
            }
        });
    }

    public void testSendRecommendingEmail() {
        // Request recommending E-mail
        HSPSocial.sendRecommendingEmail("dm@hangame.co.jp", "", new HSPSocial.HSPSendRecommendingEmailCB() {
            @Override
            public void onEmailSend(HSPResult result) {
                if (result.isSuccess()) {
                    log("Recommended E-mail request succeeded");
                } else {
                    log("Recommended E-mail request failed: " + result);
                }
            }
        });
    }

    public void testRegisterListener() {
        mAddFollowingListener = new HSPSocial.HSPAddFollowingListener() {
            @Override
            public void onFollowingAdd(List<Long> memberNos) {
                // Called when add following
                log("onFollowingAdd");
            }

        };

        HSPSocial.addFollowingAddListener(mAddFollowingListener);

        mRemoveFollowingListener = new HSPSocial.HSPRemoveFollowingListener() {
            @Override
            public void onFollowingRemove(List<Long> memberNos) {
                // Called when remove following
                log("onFollowingRemove");
            }

        };

        HSPSocial.addFollowingRemoveListener(mRemoveFollowingListener);

        mAddBlockingListener = new HSPSocial.HSPAddBlockingListener() {
            @Override
            public void onBlockingAdd(List<Long> memberNos) {
                // Called when add user to blocking list
                log("onBlockingAdd");
            }

        };

        HSPSocial.addBlockingAddListener(mAddBlockingListener);

        mRemoveBlockingListener = new HSPSocial.HSPRemoveBlockingListener() {
            @Override
            public void onBlockingRemove(List<Long> memberNos) {
                // Called when remove user to blocking list
                log("onBlockingRemove");
            }
        };

        HSPSocial.addBlockingRemoveListener(mRemoveBlockingListener);
    }

    public void testUnregisterListener() {
        HSPSocial.removeFollowingAddListener(mAddFollowingListener);
        HSPSocial.removeFollowingRemoveListener(mRemoveFollowingListener);
        HSPSocial.removeBlockingAddListener(mAddBlockingListener);
        HSPSocial.removeBlockingRemoveListener(mRemoveBlockingListener);
    }
    
    public void testQueryHSPMemberNos() {
        List<String> idpIds = new ArrayList<String>();
        
        idpIds.add(LoginService.getLoginService().getUserId());
        
        HSPSocial.queryHSPMemberNos(idpIds, new HSPSocial.HSPQueryHSPMemeberNosCB() {
            
            @Override
            public void onHSPMemberNosReceive(Map<String, Long> memberNoMap, HSPResult result) {
                if (result.isSuccess()) {
                    log("HSPSocial.queryHSPMemberNos is success");
                    
                    for (Map.Entry<String, Long> entry : memberNoMap.entrySet()) {
                        log("HSP idpId=" + entry.getKey() + ", HSP MemberNo=" + entry.getValue());
                    }
                    
                } else {
                    log("HSPSocial.queryHSPMemberNos is failed: " + result);
                }
            }
        });
    }

    public void testQueryIdpIds() {
        List<Long> memberNos = new ArrayList<Long>();
        
        memberNos.add(HSPCore.getInstance().getMemberNo());
        
        HSPSocial.queryIdpIds(memberNos, new HSPSocial.HSPQueryIdpIdsCB() {
            
            @Override
            public void onOauthIDsReceive(Map<Long, String> oauthIDsMap, HSPResult result) {
                if (result.isSuccess()) {
                    log("HSPSocial.queryIdpIds is success");
                    
                    for (Map.Entry<Long, String> entry : oauthIDsMap.entrySet()) {
                        log("HSP MemberNo =" + entry.getKey() + ", IdpID =" + entry.getValue());
                    }
                    
                } else {
                    log("HSPSocial.queryIdpIds is failed: " + result);
                }
            }
        });
    }
    
}
