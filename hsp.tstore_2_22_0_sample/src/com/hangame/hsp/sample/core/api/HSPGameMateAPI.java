
package com.hangame.hsp.sample.core.api;

/*
import com.hangame.hsp.HSPGameMate;
import com.hangame.hsp.HSPGameMate.HSPGameMateRelationType;
*/
import com.hangame.hsp.HSPProfile;
import com.hangame.hsp.HSPProfile.HSPLoadProfilesCB;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;

import java.util.List;

public class HSPGameMateAPI extends AbstractModule {
   
    private long mMemberNo = 4105000000131917L;
    /*
    // GameMate event Listener
    private HSPGameMate.HSPGameMateRequestProposalListener mGameMateRequestProposalListener;

    private HSPGameMate.HSPGameMateRequestAcceptionListener mGameMateRequestAcceptionListener;

    private HSPGameMate.HSPGameMateRequestRejectionListener mGameMateRequestRejectListener;

    private HSPGameMate.HSPGameMateRemovalListener mGameMateRemovalListener;
    */
    public HSPGameMateAPI() {
        super("HSPGameMate");
    }
    /*
    public void testPropose() {
        HSPGameMate.propose(mMemberNo, new HSPGameMate.HSPProposeGameMateCB() {

            @Override
            public void onGameMatePropose(HSPResult result) {
                if (result.isSuccess()) {
                    log("GameMate propose SUCCESS!!!");
                } else {
                    log("GameMate propose Fail: " + result);
                }
            }
        });
    }

    public void testAccept() {
        HSPGameMate.accept(mMemberNo, new HSPGameMate.HSPAcceptGameMateCB() {

            @Override
            public void onGameMateAccept(HSPResult result) {
                if (result.isSuccess()) {
                    log("GameMate accept SUCCESS!!!");
                } else {
                    log("GameMate accept Fail: " + result);
                }
            }
        });
    }

    public void testReject() {
        HSPGameMate.reject(mMemberNo, new HSPGameMate.HSPRejectGameMateCB() {

            @Override
            public void onGameMateReject(HSPResult result) {
                if (result.isSuccess()) {
                    log("GameMate reject SUCCESS!!!");
                } else {
                    log("GameMate reject Fail: " + result);
                }
            }
        });
    }

    public void testRemove() {
        HSPGameMate.remove(mMemberNo, new HSPGameMate.HSPRemoveGameMateCB() {

            @Override
            public void onGameMateRemove(HSPResult result) {
                if (result.isSuccess()) {
                    log("GameMate remove SUCCESS!!!");
                } else {
                    log("GameMate remove Fail: " + result);
                }
            }
        });
    }

    public void testQuerySentProposals() {
        HSPGameMate.querySentProposals(new HSPGameMate.HSPQuerySentGameMateProposalsCB() {

            @Override
            public void onSentGameMateProposalsReceive(List<Long> memberNos, HSPResult result) {
                if (result.isSuccess()) {
                    if (memberNos.size() == 0) {
                        log("Sent Proposal List is empty!!!");
                    } else {
                        HSPProfile.loadProfiles(memberNos, new HSPLoadProfilesCB() {

                            @Override
                            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                                if (result.isSuccess()) {
                                    // user profile
                                    for (final HSPProfile profile : profiles) {
                                        log("Member Number: " + profile.getMemberNo());
                                        log("Nickname: " + profile.getNickname());
                                        log("Recent Login Time: " + profile.getLastLoginDate());
                                    }
                                } else {
                                    log("Failed to load user profile: " + result);
                                }
                            }
                        });
                    }
                } else {
                    log("Failed to load sent proposal list: " + result);
                }
            }
        });
    }

    public void testQueryReceivedProposals() {
        HSPGameMate.queryReceivedProposals(new HSPGameMate.HSPQueryReceivedGameMateProposalsCB() {

            @Override
            public void onReceivedGameMateProposalsReceive(List<Long> memberNos, HSPResult result) {
                if (result.isSuccess()) {
                    if (memberNos.size() == 0) {
                        log("Received Proposal List is empty!!!");
                    } else {
                        HSPProfile.loadProfiles(memberNos, new HSPLoadProfilesCB() {

                            @Override
                            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                                if (result.isSuccess()) {
                                    // user profile
                                    for (final HSPProfile profile : profiles) {
                                        log("Member Number: " + profile.getMemberNo());
                                        log("Nickname: " + profile.getNickname());
                                        log("Recent Login Time: " + profile.getLastLoginDate());
                                    }
                                } else {
                                    log("Failed to load user profile: " + result);
                                }
                            }
                        });
                    }
                } else {
                    log("Failed to load received proposal list: " + result);
                }
            }
        });
    }

    public void testQueryGameMateCount() {
        HSPGameMate.queryGameMateCount(new HSPGameMate.HSPQueryGameMateCountCB() {

            @Override
            public void onGameMateCountReceive(int gameMateCount, HSPResult result) {
                if (result.isSuccess()) {
                    log("GameMate Count : " + gameMateCount);
                } else {
                    log("Failed to load gamemate Count: " + result);
                }
            }
        });
    }

    public void testQueryGameMateList() {
        HSPGameMate.queryGameMateList(0, 10, new HSPGameMate.HSPQueryGameMateListCB() {

            @Override
            public void onGameMateListReceive(List<Long> memberNos, HSPResult result) {
                if (result.isSuccess()) {
                    if (memberNos.size() == 0) {
                        log("List is empty!!!");
                    } else {
                        HSPProfile.loadProfiles(memberNos, new HSPLoadProfilesCB() {

                            @Override
                            public void onProfilesLoad(List<HSPProfile> profiles, HSPResult result) {
                                if (result.isSuccess()) {
                                    // user profile
                                    for (final HSPProfile profile : profiles) {
                                        log("Member Number: " + profile.getMemberNo());
                                        log("Nickname: " + profile.getNickname());
                                        log("Recent Login Time: " + profile.getLastLoginDate());
                                    }
                                } else {
                                    log("Failed to load user profile: " + result);
                                }
                            }
                        });
                    }
                } else {
                    log("Failed to load GameMate list: " + result);
                }
            }
        });
    }

    public void testQueryRelation() {
        HSPGameMate.queryRelation(mMemberNo, new HSPGameMate.HSPQueryRelationCB() {

            @Override
            public void onRelationReceive(HSPGameMateRelationType relation, HSPResult result) {
                if (result.isSuccess()) {
                    log("Relation: " + relation);
                } else {
                    log("Failed to load GameMate Relation: " + result);
                }
            }
        });
    }

    public void testRegisterListener() {
        mGameMateRequestProposalListener = new HSPGameMate.HSPGameMateRequestProposalListener() {

            @Override
            public void onGameMatePropasalReceive(long memberNo) {
                log("onGameMatePropasalReceive memberNo : " + memberNo);
            }
        };
        HSPGameMate.addGameMateRequestProposalListener(mGameMateRequestProposalListener);

        mGameMateRequestAcceptionListener = new HSPGameMate.HSPGameMateRequestAcceptionListener() {

            @Override
            public void onGameMatePropasalAccept(long memberNo) {
                log("onGameMatePropasalAccept memberNo : " + memberNo);
            }
        };
        HSPGameMate.addGameMateRequestAcceptionListener(mGameMateRequestAcceptionListener);

        mGameMateRequestRejectListener = new HSPGameMate.HSPGameMateRequestRejectionListener() {

            @Override
            public void onGameMatePropasalReject(long memberNo) {
                log("onGameMatePropasalReject memberNo : " + memberNo);
            }
        };
        HSPGameMate.addGameMateRequestRejectionListener(mGameMateRequestRejectListener);

        mGameMateRemovalListener = new HSPGameMate.HSPGameMateRemovalListener() {

            @Override
            public void onGameMatePropasalRemove(long memberNo) {
                log("onGameMatePropasalDismiss memberNo : " + memberNo);
            }
        };
        HSPGameMate.addGameMateRemovalListener(mGameMateRemovalListener);
    }

    public void testUnregisterListener() {
        HSPGameMate.removeGameMateRequestProposalListener(mGameMateRequestProposalListener);
        HSPGameMate.removeGameMateRequestAcceptionListener(mGameMateRequestAcceptionListener);
        HSPGameMate.removeGameMateRequestRejectionListener(mGameMateRequestRejectListener);
        HSPGameMate.removeGameMateRemovalListener(mGameMateRemovalListener);
    }
    */
}
