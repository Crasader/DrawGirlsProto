
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPAchievement;
import com.hangame.hsp.HSPAchievement.HSPDownloadAchievedIconCB;
import com.hangame.hsp.HSPAchievement.HSPDownloadUnachievedIconCB;
import com.hangame.hsp.HSPAchievement.HSPLoadAchievementsCB;
import com.hangame.hsp.HSPAchievement.HSPReportAchievementCB;
import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;

import android.graphics.Bitmap;

import java.util.List;

public class HSPAchievementAPI extends AbstractModule {
    private long mMemberNo = 4105000000131917L;

    public HSPAchievementAPI() {
        super("HSPAchievement");
    }

    public void testLoadAchievement() {
        HSPAchievement.loadAchievements(HSPCore.getInstance().getGameNo(), mMemberNo, new HSPLoadAchievementsCB() {
            @Override
            public void onAchievementsLoad(List<HSPAchievement> achievements, int totalScore, int totalCount,
                    int totalAchievedScore, int totalAchievedCount, HSPResult result) {
                if (result.isSuccess()) {
                    log("Total Achievement Count: " + totalCount);
                    log("Total Achievement Score: " + totalScore);
                    log("Total Achieved Count: " + totalAchievedCount);
                    log("Total Achieved Score: " + totalAchievedScore);

                    for (int index = 0; index < achievements.size(); index++) {
                        HSPAchievement achievement = achievements.get(index);

                        log("ID: " + achievement.getAchievementID());
                        log("Title: " + achievement.getTitle());
                        log("Detail: " + achievement.getDetail());
                        log("Score: " + achievement.getScore());
                        log("Achieved Icon URL: " + achievement.getAchievedIconURL());
                        log("Unachieved Icon URL: " + achievement.getUnachievedIconURL());
                        log("IsAchieved: " + achievement.isAchieved());
                        achievement.downloadAchievedIcon(new HSPDownloadAchievedIconCB() {
                            
                            @Override
                            public void onIconDownload(Bitmap icon, HSPResult result) {
                                if(result.isSuccess()){
                                    log("downloadAchievedIcon success: " + icon);
                                } else {
                                    log("downloadAchievedIcon fail: " + result);
                                }
                            }
                        });

                        achievement.downloadUnachievedIcon(new HSPDownloadUnachievedIconCB() {
                            
                            @Override
                            public void onIconDownload(Bitmap icon, HSPResult result) {
                                if(result.isSuccess()){
                                    log("downloadUnachievedIcon success: " + icon);
                                } else {
                                    log("downloadUnachievedIcon fail: " + result);
                                }
                            }
                        });
                    }
                } else {
                    log("Load Achievement List Fail: " + result);
                }
            }
        });
    }

    public void testLoadAchievedInfo() {
        HSPAchievement.loadAchievedInfo(HSPCore.getInstance().getGameNo(), mMemberNo,
                new HSPAchievement.HSPLoadAchievedInfoCB() {
                    @Override
                    public void onAchievedInfoLoad(int totalAchievedScore, int totalAchievedCount, HSPResult result) {
                        if (result.isSuccess()) {
                            log("Total Achieved Score: " + totalAchievedScore);
                            log("Total Achieved Count: " + totalAchievedCount);
                        } else {
                            log("Failed to Load Achievement Info: " + result);
                        }
                    }
                });
    }

    public void testReportAchievement() {
        String achievementID = HSPCore.getInstance().getGameNo() + "_00001";
        HSPAchievement.reportAchievement(achievementID, new HSPReportAchievementCB() {
            @Override
            public void onAchievementReport(HSPResult result) {
                if (result.isSuccess()) {
                    log("Report Achievement is success.");
                } else {
                    log("Report Achievement is fail: " + result);
                    // If failed, the achievement information is cached.
                    // The next time you login, or the network is activated the information is reported automatically.
                }
            }
        });
    }
}
