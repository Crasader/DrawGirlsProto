
package com.hangame.hsp.sample.view_toast_new;

import com.hangame.hsp.sample.AbstractViewCategory;
import com.hangame.hsp.ui.HSPUiFactory;
import com.hangame.hsp.ui.HSPUiUri;

import java.util.LinkedHashMap;
import java.util.Map;

public class AchievementView extends AbstractViewCategory {

    public AchievementView() {
        super("Achievement");
    }

    public Map<String, HSPUiUri> getViewMap() {
        Map<String, HSPUiUri> viewMap = new LinkedHashMap<String, HSPUiUri>();

        viewMap.put("Achievement List", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.ACHIEVEMENT_LIST));
        return viewMap;
    }
}
