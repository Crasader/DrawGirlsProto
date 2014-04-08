
package com.hangame.hsp.sample.core.ui;

import com.hangame.hsp.sample.AbstractViewCategory;
import com.hangame.hsp.ui.HSPUiFactory;
import com.hangame.hsp.ui.HSPUiUri;

import java.util.LinkedHashMap;
import java.util.Map;

public class GameplusView extends AbstractViewCategory {

    public GameplusView() {
        super("Game+");
    }

    public Map<String, HSPUiUri> getViewMap() {
        Map<String, HSPUiUri> viewMap = new LinkedHashMap<String, HSPUiUri>();

        viewMap.put("Game+", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.GAMEPLUS));

        return viewMap;
    }
}
