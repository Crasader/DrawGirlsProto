
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.ui.HSPUiFactory;
import com.hangame.hsp.ui.HSPUiLauncher;
import com.hangame.hsp.ui.HSPUiUri;
import com.hangame.hsp.ui.HSPUiUri.HSPUiUriParameterKey;
import com.hangame.hsp.ui.HSPUiUri.HSPUiUriParameterValue;

public class HSPUiLauncherAPI extends AbstractModule {

    public HSPUiLauncherAPI() {
        super("HSPUiLauncher");
    }

    public static void launchView(HSPUiUri uri, boolean isShowTopBar, boolean isShowGnb) {
        if (isShowTopBar) {
            uri.setParameter(HSPUiUriParameterKey.TOPBAR_SHOW, HSPUiUriParameterValue.TRUE);
        } else {
            uri.setParameter(HSPUiUriParameterKey.TOPBAR_SHOW, HSPUiUriParameterValue.FALSE);
        }

        if (isShowGnb) {
            uri.setParameter(HSPUiUriParameterKey.GNB_SHOW, HSPUiUriParameterValue.TRUE);
        } else {
            uri.setParameter(HSPUiUriParameterKey.GNB_SHOW, HSPUiUriParameterValue.FALSE);
        }

        HSPUiLauncher.getInstance().launch(uri);
    }

    public void testLaunchWebView() {
        HSPUiUri uri = HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.WEBVIEW);
        uri.setParameter(HSPUiUri.HSPUiUriParameterKey.WEB_URL, "http://www.hangame.com");
        launchView(uri, true, false);
    }

    public void testLaunchGamePlusView() {
        HSPUiUri uri = HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.GAMEPLUS);
        launchView(uri, true, true);
    }
}
