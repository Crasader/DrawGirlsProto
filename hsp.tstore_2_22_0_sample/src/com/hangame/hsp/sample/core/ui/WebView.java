
package com.hangame.hsp.sample.core.ui;

import com.hangame.hsp.sample.AbstractViewCategory;
import com.hangame.hsp.ui.HSPUiFactory;
import com.hangame.hsp.ui.HSPUiUri;

import java.util.LinkedHashMap;
import java.util.Map;

public class WebView extends AbstractViewCategory {

    public WebView() {
        super("Webview");
    }

    public Map<String, HSPUiUri> getViewMap() {
        Map<String, HSPUiUri> viewMap = new LinkedHashMap<String, HSPUiUri>();

        HSPUiUri webUri = HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.WEBVIEW);
        webUri.setParameter(HSPUiUri.HSPUiUriParameterKey.WEB_URL, "http://www.hangame.com");
        viewMap.put("WebView(Hangame Site)", webUri);

        HSPUiUri marketUri = HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.WEBVIEW);
        marketUri.setParameter(HSPUiUri.HSPUiUriParameterKey.WEB_URL, "market://details?id=jp.naver.sklinebirzzle");
        viewMap.put("WebView(Android Market)", marketUri);

        HSPUiUri testWebUri = HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.WEBVIEW);
        testWebUri.setParameter(HSPUiUri.HSPUiUriParameterKey.WEB_URL, "http://10.64.49.251/default13/gameplus?_locale=global");
        testWebUri.setParameter(HSPUiUri.HSPUiUriParameterKey.TOPBAR_SHOW, "false");
        viewMap.put("WebView(Test Site)", testWebUri);
        
        Float.parseFloat("1");
        
        return viewMap;
    }
}
