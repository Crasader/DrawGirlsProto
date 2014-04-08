
package com.hangame.hsp.sample.view_toast_new;

import com.hangame.hsp.HSPConfiguration;
import com.hangame.hsp.HSPCore;
import com.hangame.hsp.sample.AbstractViewCategory;
import com.hangame.hsp.ui.HSPUiFactory;
import com.hangame.hsp.ui.HSPUiUri;

import java.util.LinkedHashMap;
import java.util.Map;

public class ProfileView extends AbstractViewCategory {

    public ProfileView() {
        super("Profile");
    }

    public Map<String, HSPUiUri> getViewMap() {
        Map<String, HSPUiUri> viewMap = new LinkedHashMap<String, HSPUiUri>();

        viewMap.put("My Profile", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.PROFILE_MYPROFILE));
        viewMap.put("My Detail Profile", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.PROFILE_MYPROFILE_DETAIL));

        viewMap.put("User Detail Profile", HSPUiFactory.getUiUri(HSPUiUri.HSPUiUriAction.PROFILE_MYPROFILE_DETAIL));

        return viewMap;
    }
}
