
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPRanking;
import com.hangame.hsp.HSPRanking.HSPLoadRankingsCB;
import com.hangame.hsp.HSPRanking.HSPQueryScoresByScopeCB;
import com.hangame.hsp.HSPRanking.HSPRankingKey;
import com.hangame.hsp.HSPRanking.HSPRankingPeriod;
import com.hangame.hsp.HSPRanking.HSPRankingScope;
import com.hangame.hsp.HSPRanking.HSPReportRankingCB;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.HSPScore;
import com.hangame.hsp.sample.AbstractModule;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;

public class HSPRankingAPI extends AbstractModule {
    public HSPRankingAPI() {
        super("HSPRanking");
    }

    public void testLoadRankings() {

        HSPRanking.loadRankings(HSPCore.getInstance().getGameNo(), new HSPLoadRankingsCB() {
            @Override
            public void onRankingsLoad(List<HSPRanking> rankings, int repFactor, HSPRankingPeriod repPeriod,
                    HSPResult result) {
                if (result.isSuccess()) {
                    log("Rep Factor: " + repFactor);
                    log("Rep Period: " + repPeriod);

                    for (final HSPRanking ranking : rankings) {
                        log("ranking.getFactor(): " + ranking.getFactor());
                        log("ranking.getName(): " + ranking.getName());
                        log("ranking.getUnit(): " + ranking.getUnit());
                        log("ranking.getPeriods(): " + ranking.getPeriods());
                        for (HSPRankingPeriod period : ranking.getPeriods()) {
                            log("ranking.getResetDate(" + period + "): " + ranking.getResetDate(period));
                        }
                    }
                } else {
                    log("Ranking information fails to load: " + result);
                }
            }
        });
    }

    public void testLoadRanking() {
        HSPRanking.loadRanking(HSPCore.getInstance().getGameNo(), 0, new HSPRanking.HSPLoadRankingCB() {
            @Override
            public void onRankingLoad(HSPRanking ranking, HSPResult result) {
                if (result.isSuccess()) {
                    log("ranking.getFactor(): " + ranking.getFactor());
                    log("ranking.getName(): " + ranking.getName());
                    for (final HSPRanking.HSPRankingPeriod period : ranking.getPeriods()) {
                        log(" ranking.getPeriods(): " + period);
                    }
                    log("ranking.getUnit(): " + ranking.getUnit());
                    for (HSPRankingPeriod period : ranking.getPeriods()) {
                        log("ranking.getResetDate(" + period + "): " + ranking.getResetDate(period));
                    }
                } else {
                    log("Ranking information fails to load: " + result);
                }
            }
        });
    }

    public void testQueryScores() {
        long memberNo = HSPCore.getInstance().getMemberNo();
        List<HSPRankingKey> rankingKeys = new ArrayList<HSPRankingKey>();
        rankingKeys.add(HSPRankingKey.getRankingKey(0, HSPRanking.HSPRankingPeriod.HSP_RANKINGPERIOD_DAILY));

        HSPRanking.queryScores(memberNo, HSPCore.getInstance().getGameNo(), rankingKeys,
                new HSPRanking.HSPQueryScoresCB() {
                    @Override
                    public void onScoresReceive(List<HSPScore> scores, HSPResult result) {
                        if (result.isSuccess()) {
                            for (final HSPScore score : scores) {
                                log("score.getMemberNo(): " + score.getMemberNo());
                                log("score.getGrade(): " + score.getGrade());
                                log("score.getScore(): " + score.getScore());
                                log("score.getChangeGrade(): " + score.getChangeGrade());
                                log("score.getExtraData(): " + score.getExtraData());
                            }
                        } else {
                            log("Ranking Score fails to load: " + result);
                        }
                    }
                });
    }

    public void testReportRanking() {
        HSPRanking.reportRankingScore(100.0, 1, null, new HSPReportRankingCB() {
            @Override
            public void onRankingReport(HSPResult result) {
                if (result.isSuccess()) {
                    log("Ranking Score Reporting is successful.");
                } else {
                    log("Ranking Score Reporting is failed: " + result);
                    // If failed, the ranking score information is cached.
                    // The next time you login, or the network is activated the information is reported automatically.
                }
            }
        });

    }

    public void testQueryAllScoresByScope() {
        HSPRanking.loadRankings(HSPCore.getInstance().getGameNo(), new HSPLoadRankingsCB() {
            @Override
            public void onRankingsLoad(List<HSPRanking> rankings, int repFactor, HSPRankingPeriod repPeriod,
                    HSPResult result) {
                if (result.isSuccess()) {
                    for (final HSPRanking ranking : rankings) {
                        // Check rep ranking
                        if (ranking.getFactor() == repFactor) {
                            // Ranking index start from 1. Load 10 ranking list
                            ranking.queryScoresByScope(HSPRankingScope.HSP_RANKINGSCOPE_ALL,
                                    HSPRankingPeriod.HSP_RANKINGPERIOD_TOTAL, 1, 10, new HSPQueryScoresByScopeCB() {
                                        @Override
                                        public void onScoresReceive(HSPScore myScore, List<HSPScore> scores,
                                                HSPResult result) {
                                            if (result.isSuccess()) {
                                                log("My Ranking Score : " + formatRankingScore(myScore.getScore())
                                                        + ranking.getUnit());
                                                for (int i = 0; i < scores.size(); i++) {
                                                    HSPScore score = scores.get(i);
                                                    log("Member Number : " + score.getMemberNo() + " Grade : "
                                                            + score.getGrade() + " Score : "
                                                            + formatRankingScore(score.getScore()) + ranking.getUnit());
                                                }
                                            } else {
                                                log("Ranking Score fails to load : [ " + result + " ]");
                                            }
                                        }
                                    });
                            break;
                        }

                    }
                } else {
                    log("Ranking information fails to load: " + result);
                }
            }
        });
    }

    public void testQueryFriendScoresByScope() {
        HSPRanking.loadRankings(HSPCore.getInstance().getGameNo(), new HSPLoadRankingsCB() {
            @Override
            public void onRankingsLoad(List<HSPRanking> rankings, int repFactor, HSPRankingPeriod repPeriod,
                    HSPResult result) {
                if (result.isSuccess()) {
                    for (final HSPRanking ranking : rankings) {
                        // Check rep ranking
                        if (ranking.getFactor() == repFactor) {
                            // Ranking index start from 1. Load 10 ranking list
                            ranking.queryScoresByScope(HSPRankingScope.HSP_RANKINGSCOPE_FRIEND,
                                    HSPRankingPeriod.HSP_RANKINGPERIOD_TOTAL, 1, 10, new HSPQueryScoresByScopeCB() {
                                        @Override
                                        public void onScoresReceive(HSPScore myScore, List<HSPScore> scores,
                                                HSPResult result) {
                                            if (result.isSuccess()) {
                                                log("My Ranking Score : " + formatRankingScore(myScore.getScore())
                                                        + ranking.getUnit());
                                                for (int i = 0; i < scores.size(); i++) {
                                                    HSPScore score = scores.get(i);
                                                    log("Member Number : " + score.getMemberNo() + " Grade : "
                                                            + score.getGrade() + " Score : "
                                                            + formatRankingScore(score.getScore()) + ranking.getUnit());
                                                }
                                            } else {
                                                log("Ranking Score fails to load : [ " + result + " ]");
                                            }
                                        }
                                    });
                            break;
                        }

                    }
                } else {
                    log("Ranking information fails to load: " + result);
                }
            }
        });
    }

    public void testQueryGameMateScoresByScope() {
        HSPRanking.loadRankings(HSPCore.getInstance().getGameNo(), new HSPLoadRankingsCB() {
            @Override
            public void onRankingsLoad(List<HSPRanking> rankings, int repFactor, HSPRankingPeriod repPeriod,
                    HSPResult result) {
                if (result.isSuccess()) {
                    for (final HSPRanking ranking : rankings) {
                        // Check rep ranking
                        if (ranking.getFactor() == repFactor) {
                            // Ranking index start from 1. Load 10 ranking list
                            ranking.queryScoresByScope(HSPRankingScope.HSP_RANKINGSCOPE_GAMEMATE,
                                    HSPRankingPeriod.HSP_RANKINGPERIOD_TOTAL, 1, 10, new HSPQueryScoresByScopeCB() {
                                        @Override
                                        public void onScoresReceive(HSPScore myScore, List<HSPScore> scores,
                                                HSPResult result) {
                                            if (result.isSuccess()) {
                                                log("My Ranking Score : " + formatRankingScore(myScore.getScore())
                                                        + ranking.getUnit());
                                                for (int i = 0; i < scores.size(); i++) {
                                                    HSPScore score = scores.get(i);
                                                    log("Member Number : " + score.getMemberNo() + " Grade : "
                                                            + score.getGrade() + " Score : "
                                                            + formatRankingScore(score.getScore()) + ranking.getUnit());
                                                }
                                            } else {
                                                log("Ranking Score fails to load : [ " + result + " ]");
                                            }
                                        }
                                    });
                            break;
                        }

                    }
                } else {
                    log("Ranking information fails to load: " + result);
                }
            }
        });
    }

    public void testQueryMemberNosScoresByScope() {
        HSPRanking.loadRankings(HSPCore.getInstance().getGameNo(), new HSPLoadRankingsCB() {
            @Override
            public void onRankingsLoad(List<HSPRanking> rankings, int repFactor, HSPRankingPeriod repPeriod,
                    HSPResult result) {
                if (result.isSuccess()) {
                    for (final HSPRanking ranking : rankings) {
                        // Check rep ranking
                        if (ranking.getFactor() == repFactor) {
                            List<Long> memberNos = new ArrayList<Long>();
                            memberNos.add(4105000000134579L);
                            memberNos.add(4105000000137637L);
                            memberNos.add(HSPCore.getInstance().getMemberNo());

                            // Ranking index start from 1. Load 10 ranking list
                            ranking.queryScoresByScopeInMembers(memberNos, HSPRankingPeriod.HSP_RANKINGPERIOD_TOTAL, 1,
                                    10, new HSPQueryScoresByScopeCB() {
                                        @Override
                                        public void onScoresReceive(HSPScore myScore, List<HSPScore> scores,
                                                HSPResult result) {
                                            if (result.isSuccess()) {
                                                log("My Ranking Score : " + formatRankingScore(myScore.getScore())
                                                        + ranking.getUnit());
                                                for (int i = 0; i < scores.size(); i++) {
                                                    HSPScore score = scores.get(i);
                                                    log("Member Number : " + score.getMemberNo() + " Grade : "
                                                            + score.getGrade() + " Score : "
                                                            + formatRankingScore(score.getScore()) + ranking.getUnit());
                                                }
                                            } else {
                                                log("Ranking Score fails to load : [ " + result + " ]");
                                            }
                                        }
                                    });
                            break;
                        }

                    }
                } else {
                    log("Ranking information fails to load: " + result);
                }
            }
        });
    }

    public void testQueryAllPreviousScoresByScope() {
        HSPRanking.loadRankings(HSPCore.getInstance().getGameNo(), new HSPLoadRankingsCB() {
            @Override
            public void onRankingsLoad(List<HSPRanking> rankings, int repFactor, HSPRankingPeriod repPeriod,
                    HSPResult result) {
                if (result.isSuccess()) {
                    for (final HSPRanking ranking : rankings) {
                        // Check rep ranking
                        if (ranking.getFactor() == repFactor) {
                            // Ranking index start from 1. Load 10 ranking list
                            ranking.queryPreviousScoresByScope(HSPRankingScope.HSP_RANKINGSCOPE_ALL,
                                    HSPRankingPeriod.HSP_RANKINGPERIOD_TOTAL, 1, 10, new HSPQueryScoresByScopeCB() {
                                        @Override
                                        public void onScoresReceive(HSPScore myScore, List<HSPScore> scores,
                                                HSPResult result) {
                                            if (result.isSuccess()) {
                                                log("My Ranking Score : " + formatRankingScore(myScore.getScore())
                                                        + ranking.getUnit());
                                                for (int i = 0; i < scores.size(); i++) {
                                                    HSPScore score = scores.get(i);
                                                    log("Member Number : " + score.getMemberNo() + " Grade : "
                                                            + score.getGrade() + " Score : "
                                                            + formatRankingScore(score.getScore()) + ranking.getUnit());
                                                }
                                            } else {
                                                log("Ranking Score fails to load : [ " + result + " ]");
                                            }
                                        }
                                    });
                            break;
                        }

                    }
                } else {
                    log("Ranking information fails to load: " + result);
                }
            }
        });
    }

    public void testQueryFriendPreviousScoresByScope() {
        HSPRanking.loadRankings(HSPCore.getInstance().getGameNo(), new HSPLoadRankingsCB() {
            @Override
            public void onRankingsLoad(List<HSPRanking> rankings, int repFactor, HSPRankingPeriod repPeriod,
                    HSPResult result) {
                if (result.isSuccess()) {
                    for (final HSPRanking ranking : rankings) {
                        // Check rep ranking
                        if (ranking.getFactor() == repFactor) {
                            // Ranking index start from 1. Load 10 ranking list
                            ranking.queryPreviousScoresByScope(HSPRankingScope.HSP_RANKINGSCOPE_FRIEND,
                                    HSPRankingPeriod.HSP_RANKINGPERIOD_TOTAL, 1, 10, new HSPQueryScoresByScopeCB() {
                                        @Override
                                        public void onScoresReceive(HSPScore myScore, List<HSPScore> scores,
                                                HSPResult result) {
                                            if (result.isSuccess()) {
                                                log("My Ranking Score : " + formatRankingScore(myScore.getScore())
                                                        + ranking.getUnit());
                                                for (int i = 0; i < scores.size(); i++) {
                                                    HSPScore score = scores.get(i);
                                                    log("Member Number : " + score.getMemberNo() + " Grade : "
                                                            + score.getGrade() + " Score : "
                                                            + formatRankingScore(score.getScore()) + ranking.getUnit());
                                                }
                                            } else {
                                                log("Ranking Score fails to load : [ " + result + " ]");
                                            }
                                        }
                                    });
                            break;
                        }

                    }
                } else {
                    log("Ranking information fails to load: " + result);
                }
            }
        });
    }

    public void testQueryGameMatePreviousScoresByScope() {
        HSPRanking.loadRankings(HSPCore.getInstance().getGameNo(), new HSPLoadRankingsCB() {
            @Override
            public void onRankingsLoad(List<HSPRanking> rankings, int repFactor, HSPRankingPeriod repPeriod,
                    HSPResult result) {
                if (result.isSuccess()) {
                    for (final HSPRanking ranking : rankings) {
                        // Check rep ranking
                        if (ranking.getFactor() == repFactor) {
                            // Ranking index start from 1. Load 10 ranking list
                            ranking.queryPreviousScoresByScope(HSPRankingScope.HSP_RANKINGSCOPE_GAMEMATE,
                                    HSPRankingPeriod.HSP_RANKINGPERIOD_TOTAL, 1, 10, new HSPQueryScoresByScopeCB() {
                                        @Override
                                        public void onScoresReceive(HSPScore myScore, List<HSPScore> scores,
                                                HSPResult result) {
                                            if (result.isSuccess()) {
                                                log("My Ranking Score : " + formatRankingScore(myScore.getScore())
                                                        + ranking.getUnit());
                                                for (int i = 0; i < scores.size(); i++) {
                                                    HSPScore score = scores.get(i);
                                                    log("Member Number : " + score.getMemberNo() + " Grade : "
                                                            + score.getGrade() + " Score : "
                                                            + formatRankingScore(score.getScore()) + ranking.getUnit());
                                                }
                                            } else {
                                                log("Ranking Score fails to load : [ " + result + " ]");
                                            }
                                        }
                                    });
                            break;
                        }

                    }
                } else {
                    log("Ranking information fails to load: " + result);
                }
            }
        });
    }

    public void testQueryMembersPreviousScoresByScope() {
        HSPRanking.loadRankings(HSPCore.getInstance().getGameNo(), new HSPLoadRankingsCB() {
            @Override
            public void onRankingsLoad(List<HSPRanking> rankings, int repFactor, HSPRankingPeriod repPeriod,
                    HSPResult result) {
                if (result.isSuccess()) {
                    for (final HSPRanking ranking : rankings) {
                        // Check rep ranking
                        if (ranking.getFactor() == repFactor) {
                            List<Long> memberNos = new ArrayList<Long>();
                            memberNos.add(4105000000134579L);
                            memberNos.add(4105000000137637L);
                            memberNos.add(HSPCore.getInstance().getMemberNo());

                            // Ranking index start from 1. Load 10 ranking list
                            ranking.queryPreviousScoresByScopeInMembers(memberNos,
                                    HSPRankingPeriod.HSP_RANKINGPERIOD_TOTAL, 1, 10, new HSPQueryScoresByScopeCB() {
                                        @Override
                                        public void onScoresReceive(HSPScore myScore, List<HSPScore> scores,
                                                HSPResult result) {
                                            if (result.isSuccess()) {
                                                log("My Ranking Score : " + formatRankingScore(myScore.getScore())
                                                        + ranking.getUnit());
                                                for (int i = 0; i < scores.size(); i++) {
                                                    HSPScore score = scores.get(i);
                                                    log("Member Number : " + score.getMemberNo() + " Grade : "
                                                            + score.getGrade() + " Score : "
                                                            + formatRankingScore(score.getScore()) + ranking.getUnit());
                                                }
                                            } else {
                                                log("Ranking Score fails to load : [ " + result + " ]");
                                            }
                                        }
                                    });
                            break;
                        }

                    }
                } else {
                    log("Ranking information fails to load: " + result);
                }
            }
        });
    }

    public void testQueryScoresAroundMemberNo() {
        HSPRanking.loadRanking(HSPCore.getInstance().getGameNo(), 0, new HSPRanking.HSPLoadRankingCB() {
            @Override
            public void onRankingLoad(HSPRanking ranking, HSPResult result) {
                if (result.isSuccess()) {
                    long memberNo = HSPCore.getInstance().getMemberNo();
                    HSPRanking.HSPRankingPeriod period = ranking.getPeriods().get(0);
                    ranking.queryScoresAroundMemberNo(memberNo, period, 5, 5,
                            new HSPRanking.HSPQueryScoresAroundMemberNoCB() {
                                @Override
                                public void onScoresReceive(List<HSPScore> scores, HSPResult result) {
                                    if (result.isSuccess()) {
                                        for (final HSPScore score : scores) {
                                            log("score.getMemberNo(): " + score.getMemberNo());
                                            log("score.getGrade(): " + score.getGrade());
                                            log("score.getScore(): " + score.getScore());
                                            log("score.getChangeGrade(): " + score.getChangeGrade());
                                            log("score.getExtraData(: " + score.getExtraData());
                                        }
                                    } else {
                                        log("Ranking Score fails to load: " + result);
                                    }
                                }
                            });
                } else {
                    log("Ranking information fails to load: " + result);
                }
            }
        });
    }

    public void testQueryPreviousScoresAroundMemberNo() {
        HSPRanking.loadRanking(HSPCore.getInstance().getGameNo(), 0, new HSPRanking.HSPLoadRankingCB() {
            @Override
            public void onRankingLoad(HSPRanking ranking, HSPResult result) {
                if (result.isSuccess()) {
                    long memberNo = HSPCore.getInstance().getMemberNo();
                    HSPRanking.HSPRankingPeriod period = ranking.getPeriods().get(0);
                    ranking.queryPreviousScoresAroundMemberNo(memberNo, period, 5, 5,
                            new HSPRanking.HSPQueryScoresAroundMemberNoCB() {
                                @Override
                                public void onScoresReceive(List<HSPScore> scores, HSPResult result) {
                                    if (result.isSuccess()) {
                                        for (final HSPScore score : scores) {
                                            log("score.getMemberNo(): " + score.getMemberNo());
                                            log("score.getGrade(): " + score.getGrade());
                                            log("score.getScore(): " + score.getScore());
                                            log("score.getChangeGrade(): " + score.getChangeGrade());
                                            log("score.getExtraData(: " + score.getExtraData());
                                        }
                                    } else {
                                        log("Ranking Score fails to load: " + result);
                                    }
                                }
                            });
                } else {
                    log("Ranking information fails to load: " + result);
                }
            }
        });
    }

    private static String formatRankingScore(double score) {
        final DecimalFormat sRankingScoreFormat = new DecimalFormat("#,##0.00");

        boolean isMinus = false;

        if (score < 0.0) {
            score *= -1.0;
            isMinus = true;
        }

        String scoreStr = sRankingScoreFormat.format(score);

        if (scoreStr.endsWith(".00")) {
            scoreStr = scoreStr.substring(0, scoreStr.length() - 3);
        }

        if (isMinus) {
            scoreStr = "-" + scoreStr;
        }

        return scoreStr;
    }
}
