
package com.hangame.hsp.sample.core.ui;

import com.hangame.hsp.sample.AbstractViewCategory;
import com.hangame.hsp.ui.HSPUiFactory;
import com.hangame.hsp.ui.HSPUiUri;

import java.util.LinkedHashMap;
import java.util.Map;

public class CsView extends AbstractViewCategory {

    public CsView() {
        super("CS");
    }

    public Map<String, HSPUiUri> getViewMap() {
        Map<String, HSPUiUri> viewMap = new LinkedHashMap<String, HSPUiUri>();

        viewMap.put("CS Center", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.SUPPORTS_CSCENTER));
        viewMap.put("Notice", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.SUPPORTS_NOTICE));
        viewMap.put("FAQ", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.SUPPORTS_FAQ));
        viewMap.put("Privacy Terms", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.SUPPORTS_TERMS_PRIVACY));
        viewMap.put("Hangame Terms", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.SUPPORTS_TERMS_USE));
        viewMap.put("SmartHangame Terms", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.SUPPORTS_TERMS_HANGAME));
        viewMap.put("Punish terms", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.SUPPORTS_TERMS_PUNISH));
        return viewMap;
    }
}
