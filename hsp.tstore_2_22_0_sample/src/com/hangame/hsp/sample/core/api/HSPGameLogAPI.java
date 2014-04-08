
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPGameLog;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;

import java.util.ArrayList;
import java.util.List;

public class HSPGameLogAPI extends AbstractModule {
    public HSPGameLogAPI() {
        super("HSPGameLog");
    }

    public void testLoadGameLogs() {
        int gameNo = HSPCore.getInstance().getGameNo();
        List<Integer> gameNos = new ArrayList<Integer>();
        gameNos.add(gameNo);

        long memberNo = HSPCore.getInstance().getMemberNo();

        HSPGameLog.loadGameLogs(gameNos, memberNo, new HSPGameLog.HSPLoadGameLogsCB() {
            @Override
            public void onGameLogsLoad(List<HSPGameLog> gameLogs, HSPResult result) {
                if (result.isSuccess()) {
                    for (final HSPGameLog gameLog : gameLogs) {
                        log("Game Number : " + gameLog.getGameNo());
                        log("First played date : " + gameLog.getFirstPlayedDate());
                        log("Last played date : " + gameLog.getLastPlayedDate());
                    }
                } else {
                    log("Failed to load game list : [ " + result + " ]");
                }
            }
        });
    }
}
