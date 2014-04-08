
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.HSPServiceProperties;
import com.hangame.hsp.HSPServiceProperties.HSPLaunchingState;
import com.hangame.hsp.HSPServiceProperties.HSPServerName;
import com.hangame.hsp.HSPServiceProperties.HSPWebName;
import com.hangame.hsp.HSPState;
import com.hangame.hsp.sample.AbstractModule;

public class HSPServicePropertiesAPI extends AbstractModule {

    public HSPServicePropertiesAPI() {
        super("HSPServiceProperties");
    }

    public void testGetServicePropertiesAfterLogin() {
        if (HSPCore.getInstance().getState() == HSPState.HSP_STATE_ONLINE) {
            // Handle service properties Information
            HSPServiceProperties properties = HSPCore.getInstance().getServiceProperties();

            // Check if HSPServiceProperties information load
            if (properties.getServicePropertiesMap() != null) {
                log("HSPServiceProperties Load");
            } else {
                log("HSPServiceProperties Load Failed");
            }
        } else {
            log("HSP is not logged in");
        }
    }

    public void testGetServicePropertiesBeforeLogin() {
        final HSPServiceProperties properties = HSPCore.getInstance().getServiceProperties();

        properties.loadServiceProperties(new HSPServiceProperties.HSPLoadServicePropertiesCB() {
            @Override
            public void onServicePropertiesLoad(HSPResult result) {
                if (result.isSuccess()) {
                    log("HSPServiceProperties Load");
                } else {
                    log("HSPServiceProperties Load Failed: " + result);
                }
            }
        });
    }

    public void testGetLaunchingState() {
        HSPServiceProperties properties = HSPCore.getInstance().getServiceProperties();

        HSPLaunchingState state = properties.getLaunchingState();

        switch (state) {
            case HSP_LAUNCHINGSTATE_OK:
                log("HSPLaunchingState : OK");
                break;
            case HSP_LAUNCHINGSTATE_GAME_INFO_FAIL:
                log("HSPLaunchingState : HSP_LAUNCHINGSTATE_GAME_INFO_FAIL");
                break;
            case HSP_LAUNCHINGSTATE_CLIENT_VERSION_FAIL:
                log("HSPLaunchingState : HSP_LAUNCHINGSTATE_CLIENT_VERSION_FAIL");
                break;
            case HSP_LAUNCHINGSTATE_CLIENT_LATESET_VERSION_EXIST:
                log("HSPLaunchingState : HSP_LAUNCHINGSTATE_CLIENT_LATESET_VERSION_EXIST");
                break;
            case HSP_LAUNCHINGSTATE_HANGAME_INSPECTION:
                log("HSPLaunchingState : HSP_LAUNCHINGSTATE_HANGAME_INSPECTION");
                break;
            case HSP_LAUNCHINGSTATE_GAME_INSPECTION:
                log("HSPLaunchingState : HSP_LAUNCHINGSTATE_GAME_INSPECTION");
                break;
            case HSP_LAUNCHINGSTATE_PLATFORM_INSPECTION:
                log("HSPLaunchingState : HSP_LAUNCHINGSTATE_PLATFORM_INSPECTION");
                break;
            case HSP_LAUNCHINGSTATE_RESOURCE_UPDATE_FAIL:
                log("HSPLaunchingState : HSP_LAUNCHINGSTATE_RESOURCE_UPDATE_FAIL");
                break;
            case HSP_LAUNCHINGSTATE_BLOCKED_DEVICE:
                log("HSPLaunchingState : HSP_LAUNCHINGSTATE_BLOCKED_DEVICE");
                break;
            case HSP_LAUNCHINGSTATE_NOT_USABLE_DEVICE:
                log("HSPLaunchingState : HSP_LAUNCHINGSTATE_NOT_USABLE_DEVICE");
                break;
            case HSP_LAUNCHINGSTATE_SERVICE_CLOSED:
                log("HSPLaunchingState : HSP_LAUNCHINGSTATE_SERVICE_CLOSED");
                break;
            case HSP_LAUNCHINGSTATE_INTERNAL_ERROR:
                log("HSPLaunchingState : HSP_LAUNCHINGSTATE_INTERNAL_ERROR");
                break;
            case HSP_LAUNCHINGSTATE_ACCESS_DENIED:
            default:
                log("HSPLaunchingState : HSP_LAUNCHINGSTATE_ACCESS_DENIED");
                break;
        }
    }

    public void testGetGameServerUrl() {
        if (HSPCore.getInstance().getState() == HSPState.HSP_STATE_ONLINE) {
            // HSP Service Property of the load when you log in, so if HSP loadServiceProperties online immediately,
            // without executing the method by
            // invoking getServerAddress Game Server Url can come get you.
            final HSPServiceProperties serviceProperties = HSPCore.getInstance().getServiceProperties();

            String gameServerUrl = serviceProperties.getServerAddress(HSPServerName.HSP_SERVERNAME_GAMESVR);

            // If server address is null, check if the property is registered in admin.
            log("Game Server Address : " + gameServerUrl);
        } else {
            // HSP Login to load before the serviceProperty Game Server Game server address for the key value can be
            // determined.
            final HSPServiceProperties serviceProperties = HSPCore.getInstance().getServiceProperties();
            serviceProperties.loadServiceProperties(new HSPServiceProperties.HSPLoadServicePropertiesCB() {
                @Override
                public void onServicePropertiesLoad(HSPResult result) {
                    if (result.isSuccess()) {
                        String gameServerUrl = serviceProperties.getServerAddress(HSPServerName.HSP_SERVERNAME_GAMESVR);

                        // If server address is null, check if the property is registered in admin.
                        log("Game Server Address : " + gameServerUrl);
                    } else {
                        // error handling
                        log("HSPServiceProperties Load Failed: " + result);
                    }
                }
            });
        }
    }

    public void testGetMaintenanceUrl() {
        if (HSPCore.getInstance().getState() == HSPState.HSP_STATE_ONLINE) {
            // HSP Service Property of the load when you log in, so if HSP loadServiceProperties online immediately,
            // without executing the method by
            // invoking getServerAddress Game Server Url can come get you.
            final HSPServiceProperties serviceProperties = HSPCore.getInstance().getServiceProperties();

            String maintenanceUrl = serviceProperties.getWebURL(HSPWebName.HSP_WEBNAME_MAINTENANCE_INFO_URL);

            // If server address is null, check if the property is registered in admin.
            log("Maintenance URL : " + maintenanceUrl);
        } else {
            // HSP Login to load before the serviceProperty Game Server Game server address for the key value can be
            // determined.
            final HSPServiceProperties serviceProperties = HSPCore.getInstance().getServiceProperties();
            serviceProperties.loadServiceProperties(new HSPServiceProperties.HSPLoadServicePropertiesCB() {
                @Override
                public void onServicePropertiesLoad(HSPResult result) {
                    if (result.isSuccess()) {
                        String maintenanceUrl = serviceProperties
                                .getWebURL(HSPWebName.HSP_WEBNAME_MAINTENANCE_INFO_URL);

                        // If server address is null, check if the property is registered in admin.
                        log("Maintenance URL : " + maintenanceUrl);
                    } else {
                        // error handling
                        log("HSPServiceProperties Load Failed: " + result);
                    }
                }
            });
        }
    }
}
