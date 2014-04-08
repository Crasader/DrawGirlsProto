
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPProfile;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.HSPUtil;
import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.sample.core.UiController;

import android.graphics.Bitmap;

public class HSPUtilAPI extends AbstractModule {

    public HSPUtilAPI() {
        super("HSPUtil");
    }

    public void testGetHspVersion() {
        // HSP Version API
        String hspVersion = HSPUtil.getHSPVersion();

        log("HSP Version = " + hspVersion);
    }

    public void testGetUDID() {
        // Device UDID API
        String udid = HSPUtil.getUniqueDeviceID();

        log("Device UDID = " + udid);
    }

    public void testCheckDeviceRooting() {
        // Device rooting API
        HSPUtil.checkCrackedDevice(new HSPUtil.HSPCheckCrackedDeviceCB() {
            @Override
            public void onCrackedDeviceCheck(boolean isCracked, HSPResult result) {
                if (result.isSuccess()) {
                    if (isCracked) {
                        log("This device is rooting device.");
                    } else {
                        log("This device is not rooting device.");
                    }
                } else {
                    log("Failed to check Device rooting: " + result);
                }
            }
        });
    }

    public void testCheckDeviceCheating() {
        // Device cheating API
        HSPUtil.checkCheating(new HSPUtil.HSPCheckCheatingCB() {
            @Override
            public void onCheatingCheck(boolean isCheated, HSPResult result) {
                if (result.isSuccess()) {
                    if (isCheated) {
                        log("This device is cheating device.");
                    } else {
                        log("This device is not cheating device.");
                    }
                } else {
                    log("Failed to check Device cheating: " + result);
                }
            }
        });
    }

    public void testCheckHackingTool() {
        // Set hacking tool types to check
        int hackingToolTypes = HSPUtil.HSPHackingToolType.ROOTING | HSPUtil.HSPHackingToolType.CHEATING
                | HSPUtil.HSPHackingToolType.MOBIZEN | HSPUtil.HSPHackingToolType.BLUESTACKS
                | HSPUtil.HSPHackingToolType.INTEGRITY | HSPUtil.HSPHackingToolType.AVD
                | HSPUtil.HSPHackingToolType.ADB;

        // Check device hacking API
        HSPUtil.checkHackingTool(hackingToolTypes, new HSPUtil.HSPCheckHackingToolCB() {
            @Override
            public void onHackingToolCheck(int hackingToolType, HSPResult result) {
                if (result.isSuccess()) {
                    if (hackingToolType != HSPUtil.HSPHackingToolType.NONE) {
                        log("This device is hacked device:" + hackingToolType);
                    } else {
                        log("This device is not hacked device.");
                    }
                } else {
                    log("Failed to check Device hacked: " + result);
                }
            }
        });
    }

    public void testDownloadImage() {
        // image download API
        long memberNo = HSPCore.getInstance().getMemberNo();
        String imageUrl = HSPProfile.getProfileImageURL(memberNo, true);

        log("Image Url: " + imageUrl);
        HSPUtil.downloadImage(imageUrl, new HSPUtil.HSPDownloadImageCB() {

            @Override
            public void onImageDownload(Bitmap image, HSPResult result) {
                if (result.isSuccess()) {
                    log("Image object: " + image);
                } else {
                    log("Failed to download image: " + result);
                }
            }
        });
    }

    public void testGetSelectedImageFromGallery() {

        // Check GetSelectedImageFromGallery API
        HSPUtil.getSelectedImageFromGallery(new HSPUtil.HSPDownloadImageCB() {

            @Override
            public void onImageDownload(Bitmap image, HSPResult result) {
                // TODO Auto-generated method stub
                if (result.isSuccess()) {
                    log("result Success : onImageDownload : ");
                }
            }
        });
    }

    public void testGetCountryCode() {
        String countryCode = HSPUtil.getCountryCode();

        log("Country Code = " + countryCode);
    }

    public void testGetCarrierCode() {
        String carrierCode = HSPUtil.getCarrierCode();

        log("Carrier Code = " + carrierCode);
    }

    public void testGetCarrierName() {
        String operatorName = HSPUtil.getCarrierName();

        log("Carrier Name = " + operatorName);
    }

    public void testAlertViewWithAgeRequirement() {

        HSPUtil.alertViewWithAgeRequirement(UiController.getActivity(), new HSPUtil.HSPAlertViewWithAgeRequirementCB() {

            @Override
            public void onCheckResult(HSPUtil.HSPAgeRequirement resultStatus) {
                // TODO Auto-generated method stub
                log("Result : " + resultStatus);
            }
        });
    }
}
