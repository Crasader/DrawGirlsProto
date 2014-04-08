
package com.hangame.hsp.sample.view_toast_new;

import com.hangame.hsp.sample.AbstractViewCategory;
import com.hangame.hsp.ui.HSPUiFactory;
import com.hangame.hsp.ui.HSPUiUri;

import java.util.LinkedHashMap;
import java.util.Map;

public class SettingView extends AbstractViewCategory {

    public SettingView() {
        super("Setting");
    }

    public Map<String, HSPUiUri> getViewMap() {
        Map<String, HSPUiUri> viewMap = new LinkedHashMap<String, HSPUiUri>();

        viewMap.put("More View", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.MORE_VIEW));

        viewMap.put("Setting", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.SETTINGS));

        return viewMap;
    }
}
