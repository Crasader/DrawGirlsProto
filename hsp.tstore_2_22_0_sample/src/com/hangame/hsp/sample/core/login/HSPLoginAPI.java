
package com.hangame.hsp.sample.core.login;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPLoginProfile;
import com.hangame.hsp.HSPOAuthData;
import com.hangame.hsp.HSPOAuthProvider;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.sample.core.UiController;

import android.app.Activity;
import android.content.Context;

public class HSPLoginAPI extends AbstractModule {
    public HSPLoginAPI() {
        super("HSPLoginAPI");
    }

    public boolean initialize(Context context, int gameNo, String gameId, String gameVersion) {
        log("HSPCore.createInstance(gameNo=" + gameNo + ", gameId=" + gameId + ", gameVersion=" + gameVersion + ")");

        boolean initialize = HSPCore.createInstance(context, gameNo, gameId, gameVersion);

        return initialize;
    }

    public void login(Activity activity, boolean manualLogin) {
        log("HSPCore.login(manualLogin=" + manualLogin + ")");

        showProgressDialog();

        HSPCore.getInstance().login(activity, manualLogin, new HSPCore.HSPLoginCB() {

            @Override
            public void onLogin(HSPResult result, boolean isPlayable) {
                log("@@@@@@ HSPCore.login callback => " + result);
                handleResult("HSPCore.login()", result);
                hideProgressDialog();
            }
        });
    }

    public void loginKakao(Activity activity, boolean manualLogin) {
        log("HSPCore.login(manualLogin=" + manualLogin + ")");

        showProgressDialog();

        HSPCore.getInstance().setLoginProfileHandler(new HSPCore.HSPSetLoginProfileCB() {

            @Override
            public void onChange(HSPLoginProfile loginProfile) {
                log("@@@@@@ HSPCore.setLoginProfileListener callback => " + loginProfile);
                loginProfile.setNickname("miwol");
                loginProfile.setProfileImageUrl("miwol_profile_image");
            }
            
        });
        
        HSPCore.getInstance().login(activity, manualLogin, new HSPCore.HSPLoginCB() {

            @Override
            public void onLogin(HSPResult result, boolean isPlayable) {
                log("@@@@@@ HSPCore.login callback => " + result);
                handleResult("HSPCore.login()", result);
                hideProgressDialog();
            }
        });
    }

    public void loginByOauth(Activity activity) {
//        String channelId = "E7zE3cdnQd";
        String accessToken = "AAAA14D+k4/5sZ/3hsb6i6enVzYXtagOS7TtBoT6mLpBKR1IpDsB5jxEkTaGeS/kNVSrvFSxLv5AGWglMBTyVShpZ1nAmN4l4DNBRmsnzmsVpBhykU5fH6xf+Xl3+mH3OV15bq0TDgbmB4LBcOEddqKvKT8qtQTDD1qjh3FUaGVjvRs5wGlfn1iO4EjJwVLA2RPsrKQlSrPq5Tn81+IHSMQOVipGzvR6M505BE/M6lj9Of875r6xcXKyFBr3/tWXUp7HL8xtCdXXmCogss17aoKcFSqJs9btD4q9htsFURJqobBa";
        String accessTokenSecret = "El6GQB5wVsJexUdtrnAzAVcIS1W_06";
        long expireTime = 0;

        HSPOAuthData oauth = new HSPOAuthData();
        oauth.setAccessToken(accessToken);
        oauth.setAccessTokenSecret(accessTokenSecret);
        oauth.setTokenExpireTime(expireTime);
        HSPCore.getInstance().login(activity, HSPOAuthProvider.POKETROKET, oauth, new HSPCore.HSPLoginCB() {
            @Override
            public void onLogin(HSPResult result, boolean isPlayable) {
                log("@@@@@@ HSPCore.login callback => " + result);
                handleResult("HSPCore.login()", result);
                hideProgressDialog();
            }
        });
    }

    public void suspend() {
        log("HSPCore.suspend()");

        HSPCore.getInstance().suspend(new HSPCore.HSPSuspendCB() {

            @Override
            public void onSuspend(HSPResult result) {
                handleResult("HSPCore.suspend()", result);
            }
        });
    }

    public void logout() {
        log("HSPCore.logout()");

        showProgressDialog();

        HSPCore.getInstance().logout(new HSPCore.HSPLogoutCB() {
            @Override
            public void onLogout(HSPResult result) {
                handleResult("HSPCore.logout()", result);
                hideProgressDialog();
            }
        });
    }

    public void resetAccount() {
        log("HSPCore.resetAccount()");

        showProgressDialog();

        HSPCore.getInstance().resetAccount(new HSPCore.HSPResetAccountCB() {

            @Override
            public void onAccountReset(HSPResult result) {
                handleResult("HSPCore.resetAccount()", result);
                hideProgressDialog();
            }
        });
    }

    public void withdrawAccount() {
        log("HSPCore.withdrawAccount()");

        showProgressDialog();

        HSPCore.getInstance().withdrawAccount(new HSPCore.HSPWithdrawAccountCB() {

            @Override
            public void onAccountWithdraw(HSPResult result) {
                handleResult("HSPCore.withdrawAccount()", result);
                hideProgressDialog();
            }
        });
    }

    public void requestMappingToAccount() {
        UiController.log("HSPCore.requestMappingToAccount()");

        HSPCore.getInstance().requestMappingToAccount(new HSPCore.HSPRequestMappingToAccountCB() {

            @Override
            public void onIdpIDMap(HSPResult result) {
                handleResult("HSPCore.requestMappingToAccount()", result);
            }
        });
    }

    private void handleResult(String method, HSPResult result) {
        log(method + ": " + result);
        updateState();

        showToast(method + (result.isSuccess() ? " success" : " failed: 0x" + Integer.toHexString(result.getCode())));
    }
}
