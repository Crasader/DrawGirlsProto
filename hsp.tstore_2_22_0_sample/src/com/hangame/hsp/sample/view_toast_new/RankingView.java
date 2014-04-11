
package com.hangame.hsp.sample.view_toast_new;

import com.hangame.hsp.sample.AbstractViewCategory;
import com.hangame.hsp.ui.HSPUiFactory;
import com.hangame.hsp.ui.HSPUiUri;

import java.util.LinkedHashMap;
import java.util.Map;

public class RankingView extends AbstractViewCategory {

    public RankingView() {
        super("Ranking");
    }

    public Map<String, HSPUiUri> getViewMap() {
        Map<String, HSPUiUri> viewMap = new LinkedHashMap<String, HSPUiUri>();

        viewMap.put("Ranking List", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.RANKING_LIST));
        viewMap.put("Ranking Detail", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.RANKING_DETAIL));

        return viewMap;
    }
}
