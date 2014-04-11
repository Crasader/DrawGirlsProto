
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPConfiguration;
import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.sample.core.UiController;

public class HSPConfigurationAPI extends AbstractModule {

    public HSPConfigurationAPI() {
        super("HSPConfiguration");
    }

    public void testGetConfigurationData() {
        HSPConfiguration configuration = HSPConfiguration.getInstance(UiController.getActivity());
        log("Launching Zone: " + configuration.getLaunchingZone());
        log("Launching Address: " + configuration.getAddressLaunching());
        log("HSP Locale: " + configuration.getLocale());
        log("Launching Refresh Time: " + configuration.getLncRefreshTimeInterval());
        log("heartbeat interval: " + configuration.getHeartBeatTimeInterval());
        log("Debug Level: " + configuration.getDebugLevel());
        log("Market code: " + configuration.getMarketCode());
        log("TCP Timeout: " + configuration.getTimeoutTCP());
        log("HTTP Timeout: " + configuration.getTimeoutHTTP());
        log("WelcomeView display: " + configuration.isShowUiWelcomeMessage());
        log("New version download UI display: " + configuration.isShowUiVersionCheck());
        log("Maintenance UI display: " + configuration.isShowUiMaintenance());
        log("Use push: " + configuration.isUsePush());
        log("ResendFailedData: " + configuration.isResendFailedData());
        log("Suspend Delay Time: " + configuration.getSuspendDelayTime());
        log("Enforced Device Login: " + configuration.isEnforcedDeviceLogin());
    }
}
