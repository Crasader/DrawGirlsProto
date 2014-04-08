
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPGame;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.sample.core.UiController;

import android.graphics.Bitmap;

import java.util.ArrayList;
import java.util.List;

public class HSPGameAPI extends AbstractModule {

    public HSPGameAPI() {
        super("HSPGame");
    }

    public void testLaunchApp() {
        int gameNo = HSPCore.getInstance().getGameNo();
        List<Integer> gameNos = new ArrayList<Integer>();
        gameNos.add(gameNo);

        HSPGame.loadGames(gameNos, new HSPGame.HSPLoadGamesCB() {
            @Override
            public void onGamesLoad(List<HSPGame> games, HSPResult result) {
                if (result.isSuccess()) {
                    if (games.size() == 0) {
                        log("No game list exist.");
                    } else {
                        HSPGame game = games.get(0);

                        game.launchApp(null);
                    }
                } else {
                    log("Failed to load game list: " + result);
                }
            }
        });
    }

    public void testLaunchStore() {
        int gameNo = HSPCore.getInstance().getGameNo();
        List<Integer> gameNos = new ArrayList<Integer>();
        gameNos.add(gameNo);

        HSPGame.loadGames(gameNos, new HSPGame.HSPLoadGamesCB() {
            @Override
            public void onGamesLoad(List<HSPGame> games, HSPResult result) {
                if (result.isSuccess()) {
                    if (games.size() == 0) {
                        log("No game list exist.");
                    } else {
                        HSPGame game = games.get(0);

                        game.launchStore(null);
                    }
                } else {
                    log("Failed to load game list: " + result);
                }
            }
        });
    }

    public void testLaunchAppOrStore() {
        int gameNo = HSPCore.getInstance().getGameNo();
        List<Integer> gameNos = new ArrayList<Integer>();
        gameNos.add(gameNo);

        HSPGame.loadGames(gameNos, new HSPGame.HSPLoadGamesCB() {
            @Override
            public void onGamesLoad(List<HSPGame> games, HSPResult result) {
                if (result.isSuccess()) {
                    if (games.size() == 0) {
                        log("No game list exist.");
                    } else {
                        HSPGame game = games.get(0);

                        game.launchAppOrStore(UiController.getActivity(), null);
                    }
                } else {
                    log("Failed to load game list: " + result);
                }
            }
        });
    }

    public void testLoadGamesWithIndex() {
        // Request open game in current connected zone
        HSPGame.loadGames(0, 10, new HSPGame.HSPLoadGamesCB() {
            @Override
            public void onGamesLoad(List<HSPGame> games, HSPResult result) {
                if (result.isSuccess()) {
                    if (games.size() == 0) {
                        log("No game list exist.");
                    } else {
                        for (final HSPGame game : games) {
                            log("Game Number: " + game.getGameNo());
                            log("Game ID: " + game.getGameID());
                            log("Game Name: " + game.getGameName());
                            log("Game status code: " + game.getStatusCode());
                            log("Game install URL: " + game.getRedirectionURL());
                            log("Bundle ID: " + game.getBundleID());
                            log("Custom URL: " + game.getCustomURL());
                            log("Is supported: " + game.isSupported());
                            log("Is installed: " + game.isInstalled());
                        }
                    }
                } else {
                    log("Failed to load game list: " + result);
                }
            }
        });
    }

    public void testLoadGamesWithGameNo() {
        List<Integer> gameNos = new ArrayList<Integer>();
        gameNos.add(101);
        gameNos.add(9001);
        gameNos.add(9101);
        gameNos.add(9301);

        // Request open game in current connected zone
        HSPGame.loadGames(gameNos, new HSPGame.HSPLoadGamesCB() {
            @Override
            public void onGamesLoad(List<HSPGame> games, HSPResult result) {
                if (result.isSuccess()) {
                    if (games.size() == 0) {
                        log("No game list exist.");
                    } else {
                        for (final HSPGame game : games) {
                            log("Game Number: " + game.getGameNo());
                            log("Game ID: " + game.getGameID());
                            log("Game Name: " + game.getGameName());
                            log("Game status code: " + game.getStatusCode());
                            log("Game install URL: " + game.getRedirectionURL());
                            log("Bundle ID: " + game.getBundleID());
                            log("Custom URL: " + game.getCustomURL());
                            log("Is supported: " + game.isSupported());
                            log("Is installed: " + game.isInstalled());
                        }
                    }
                } else {
                    log("Failed to load game list: " + result);
                }
            }
        });
    }

    public void testDownloadGameIcon() {
        int gameNo = HSPCore.getInstance().getGameNo();
        List<Integer> gameNos = new ArrayList<Integer>();
        gameNos.add(gameNo);

        HSPGame.loadGames(gameNos, new HSPGame.HSPLoadGamesCB() {
            @Override
            public void onGamesLoad(List<HSPGame> games, HSPResult result) {
                if (result.isSuccess()) {
                    HSPGame game = games.get(0);

                    game.downloadGameIcon(new HSPGame.HSPDownloadGameIconCB() {
                        @Override
                        public void onIconDownload(Bitmap icon, HSPResult result) {
                            if (result.isSuccess()) {
                                log("Game Icon download has been successful.");
                            } else {
                                log("Failed to download game icon: " + result);
                            }
                        }
                    });
                } else {
                    log("Failed to load game list: " + result);
                }
            }
        });
    }
}
