
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPCore.HSPRequestServerUTCCB;
import com.hangame.hsp.HSPMappingType;
import com.hangame.hsp.HSPOAuthProvider;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.HSPState;
import com.hangame.hsp.auth.login.HSPLoginService;
import com.hangame.hsp.auth.login.HSPSilosService;
import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.ui.ResourceUtil;
import com.hangame.hsp.util.NetworkUtil;

import android.content.Context;
import android.telephony.TelephonyManager;

import java.util.Date;
import java.util.UUID;

public class HSPCoreAPI extends AbstractModule {
    // auth event listener
    private HSPCore.HSPBeforeLoginListener mBeforeLoginListener;

    private HSPCore.HSPAfterLoginListener mAfterLoginListener;

    private HSPCore.HSPBeforeLogoutListener mBeforeLogoutListener;

    private HSPCore.HSPAfterLogoutListener mAfterLogoutListener;

    private HSPCore.HSPBeforeResetAccountListener mBeforeResetAccountListener;

    private HSPCore.HSPAfterResetAccountListener mAfterResetAccountListener;

    public HSPCoreAPI() {
        super("HSPCore");
    }

    public void testGetUUID() {
        final TelephonyManager tm = (TelephonyManager) ResourceUtil.getContext().getSystemService(Context.TELEPHONY_SERVICE);
        
        final String tmDevice, tmSerial, androidId;
        tmDevice = "" + tm.getDeviceId();
        tmSerial = "" + tm.getSimSerialNumber();
        androidId = "" + android.provider.Settings.Secure.getString(ResourceUtil.getContext().getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
      
        UUID deviceUuid = new UUID(androidId.hashCode(), ((long)tmDevice.hashCode() << 32) | tmSerial.hashCode());
        String deviceId = deviceUuid.toString();
        log("HSPCore.testGetUUID(): " + deviceId);
        log("HSPCore.testGetUUID() random: " + UUID.randomUUID().toString());
    }

    public void testGetGameID() {
        log("HSPCore.getGameID(): " + HSPCore.getInstance().getGameID());
    }

    public void testGetGameNo() {
        log("HSPCore.getGameNo(): " + HSPCore.getInstance().getGameNo());
    }

    public void testGetGameVersion() {
        log("HSPCore.getGameVersion(): " + HSPCore.getInstance().getGameVersion());
    }

    public void testGetMemberAge() {
        log("HSPCore.getMemberAge(): " + HSPCore.getInstance().getMemberAge());
    }

    public void testGetMemberID() {
        log("HSPCore.getMemberID(): " + HSPCore.getInstance().getMemberID());
    }

    public void testGetMemberNo() {
        log("HSPCore.getMemberNo(): " + HSPLoginService.getMemberNo());
    }

    public void testGetMemberNickname() {
        log("HSPCore.getMemberNo(): " + HSPLoginService.getMemberNickname());
    }

    public void testGetTicket() {
        log("HSPCore.getTicket(): " + HSPCore.getInstance().getTicket());
    }

    public void testGetLocalIpAddress() {
        log("NetworkUtil.getLocalIpAddress(): " + NetworkUtil.getLocalIpAddress());
    }

    public void testGetState() {
        HSPState state = HSPCore.getInstance().getState();
        switch (state) {
            case HSP_STATE_INIT:
                log("HSP State = INIT");
                break;
            case HSP_STATE_ONLINE:
                log("HSP State = ONLINE");
                break;
            case HSP_STATE_OFFLINE:
                log("HSP State = OFFLINE");
                break;
            case HSP_STATE_UNAVAILABLE:
                log("HSP State = UNAVAILABLE");
                break;
            default:
                log("HSP State = UNKNOWN");
                break;
        }
    }

    public void testRequestServerUTC() {
        HSPCore.getInstance().requestServerUTC(new HSPRequestServerUTCCB() {

            @Override
            public void onServerUTCReceive(Date date, HSPResult result) {
                if (result.isSuccess()) {
                    log("ServerUTC to locale: " + date.toLocaleString());
                    log("ServerUTC to GMT: " + date.toGMTString());
                    log("ServerUTC: " + date);
                } else {
                    log("request ServerUTC is Failed: " + result);
                }
            }
        });
    }

    public void testGetLoginType() {
        log("LoginType(): " + HSPCore.getInstance().loginType());
    }

    public void testGetOAuthAccessToken() {
        log("GetOAuthAccessToken(): " + HSPCore.getInstance().getOAuthAccessToken());
    }

    public void testGetOAuthInfo() {
        log("GetOAuthInfo(): " + HSPSilosService.getOAuthInfo());
    }

    public void testGetIdType() {
        log("IdType(): " + HSPSilosService.getMemberIDType());
    }

    public void testRegisterListener() {
        // Event before performing login
        mBeforeLoginListener = new HSPCore.HSPBeforeLoginListener() {
            @Override
            public void onBeforeLogin() {
                // Handle required action before performing login
                log("onBeforeLogin");
            }
        };

        HSPCore.getInstance().addBeforeLoginListener(mBeforeLoginListener);

        // Event after performing login
        mAfterLoginListener = new HSPCore.HSPAfterLoginListener() {
            @Override
            public void onAfterLogin() {
                // Handle required action after performing login
                log("onAfterLogin");
            }
        };

        HSPCore.getInstance().addAfterLoginListener(mAfterLoginListener);

        // Event before performing logout
        mBeforeLogoutListener = new HSPCore.HSPBeforeLogoutListener() {
            @Override
            public void onBeforeLogout() {
                // Handle required action before performing logout
                log("onBeforeLogout");
            }
        };

        HSPCore.getInstance().addBeforeLogoutListener(mBeforeLogoutListener);

        // Event after performing logout
        mAfterLogoutListener = new HSPCore.HSPAfterLogoutListener() {
            @Override
            public void onAfterLogout() {
                // Handle required action after performing logout
                log("onAfterLogout");
            }
        };

        HSPCore.getInstance().addAfterLogoutListener(mAfterLogoutListener);

        // Event before performing device reset
        mBeforeResetAccountListener = new HSPCore.HSPBeforeResetAccountListener() {
            @Override
            public void onBeforeResetAccount() {
                // Handle required action before performing device reset
                log("onBeforeResetAccount");
            }
        };

        HSPCore.getInstance().addBeforeResetAccountListener(mBeforeResetAccountListener);

        // Event after performing device reset
        mAfterResetAccountListener = new HSPCore.HSPAfterResetAccountListener() {
            @Override
            public void onAfterResetAccount() {
                // Handle required action after performing device reset
                log("onAfterResetAccount");
            }
        };

        HSPCore.getInstance().addAfterResetAccountListener(mAfterResetAccountListener);
    }

    public void testGuestMappingHangame() {
        HSPCore.getInstance().requestMappingToAccount(HSPMappingType.ID, new HSPCore.HSPRequestMappingToAccountCB() {
            @Override
            public void onIdpIDMap(HSPResult result) {
                log("HSPCore.requestMappingToAccount() : " + result);
            }
        });
    }
    
    public void testGuestMappingPayco() {
        HSPCore.getInstance().requestMappingToAccount(HSPMappingType.EMAIL, new HSPCore.HSPRequestMappingToAccountCB() {
            @Override
            public void onIdpIDMap(HSPResult result) {
                log("HSPCore.requestMappingToAccount() : " + result);
            }
        });
    }

    public void testGuestMappingFacebook() {
        HSPCore.getInstance().requestMappingToAccount(HSPMappingType.FACEBOOK, new HSPCore.HSPRequestMappingToAccountCB() {
            @Override
            public void onIdpIDMap(HSPResult result) {
                log("HSPCore.requestMappingToAccount() : " + result);
            }
        });
    }

    public void testLoginNaver() {
        HSPCore.getInstance().login(HSPCore.getInstance().getGameActivity(), HSPOAuthProvider.NAVER, new HSPCore.HSPLoginCB() {

            @Override
            public void onLogin(HSPResult result, boolean isPlayable) {
                log("@@@@@@ HSPCore.login callback => " + result);
            }
        });
    }

    public void testLoginFacebook() {
        HSPCore.getInstance().login(HSPCore.getInstance().getGameActivity(), HSPOAuthProvider.FACEBOOK, new HSPCore.HSPLoginCB() {

            @Override
            public void onLogin(HSPResult result, boolean isPlayable) {
                log("@@@@@@ HSPCore.login callback => " + result);
            }
        });
    }

    public void testLoginPayco() {
        HSPCore.getInstance().login(HSPCore.getInstance().getGameActivity(), HSPOAuthProvider.NEID, new HSPCore.HSPLoginCB() {

            @Override
            public void onLogin(HSPResult result, boolean isPlayable) {
                log("@@@@@@ HSPCore.login callback => " + result);
            }
        });
    }

    public void testLoginHangame() {
        HSPCore.getInstance().login(HSPCore.getInstance().getGameActivity(), HSPOAuthProvider.HANGAME, new HSPCore.HSPLoginCB() {

            @Override
            public void onLogin(HSPResult result, boolean isPlayable) {
                log("@@@@@@ HSPCore.login callback => " + result);
            }
        });
    }

    public void testLoginGuest() {
        HSPCore.getInstance().login(HSPCore.getInstance().getGameActivity(), HSPOAuthProvider.GUEST, new HSPCore.HSPLoginCB() {

            @Override
            public void onLogin(HSPResult result, boolean isPlayable) {
                log("@@@@@@ HSPCore.login callback => " + result);
            }
        });
    }

    public void testGetDisplayName() {
        log("HSPSilosService.getMemberDisplayName() : " + HSPSilosService.getMemberDisplayName());
    }

    public void testUnregisterListener() {
        HSPCore.getInstance().removeBeforeLoginListener(mBeforeLoginListener);
        HSPCore.getInstance().removeAfterLoginListener(mAfterLoginListener);
        HSPCore.getInstance().removeBeforeLogoutListener(mBeforeLogoutListener);
        HSPCore.getInstance().removeAfterLogoutListener(mAfterLogoutListener);
        HSPCore.getInstance().removeBeforeResetAccountListener(mBeforeResetAccountListener);
        HSPCore.getInstance().removeAfterResetAccountListener(mAfterResetAccountListener);
    }
}
