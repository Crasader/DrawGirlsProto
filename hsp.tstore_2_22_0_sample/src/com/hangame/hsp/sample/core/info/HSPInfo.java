
package com.hangame.hsp.sample.core.info;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPUtil;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import android.content.Context;
import android.content.res.Resources;
import android.content.res.XmlResourceParser;
import android.util.Log;

import java.io.IOException;
import java.util.Locale;

public class HSPInfo {
    private static final String TAG = "HSPInfo";
    
    public static String getHSPRuntimeInfo() {
        StringBuffer sb = new StringBuffer();

        sb.append("HSP State = " + HSPCore.getInstance().getState() + "\n");
        sb.append("HSP Service Domain = " + HSPCore.getInstance().getServiceProperties().getValue("serviceDomain")
                + "\n");
        sb.append("HSP LNC URL = " + HSPCore.getInstance().getConfiguration().getAddressLaunching() + "\n");
        sb.append("HSP MemberNo = " + HSPCore.getInstance().getMemberNo() + "\n");
        sb.append("HSP MemberID = " + HSPCore.getInstance().getMemberID() + "\n");
        
        return sb.toString();
    }

    public static String getHSPInfo(Context context, boolean isTest) {
        StringBuffer sb = new StringBuffer();

        sb.append("=== HSP SDK Version ===\n");

        sb.append(HSPUtil.getHSPVersion() + "\n");

        if (isTest == false) {
            sb.append("=== HSP Module Version ===\n");

            printVersion(context, "hspsdk_version", sb);
            printVersion(context, "hspsdk_version_npush", sb);
            printVersion(context, "hspsdk_version_payment", sb);
        }

        sb.append("=== HSPCore ===\n");

        sb.append(HSPCore.getInstance() + "\n");

        sb.append("=== HSPConfiguration ===\n");

        sb.append(HSPCore.getInstance().getConfiguration() + "\n");

        sb.append("=== HSP Util ===\n");

        //sb.append("HSP UDID = " + HSPUtil.getUniqueDeviceID() + "\n");
        
        sb.append("=== Device Locale ===\n");
        
        sb.append("Locale = " + Locale.getDefault() + "\n");

        return sb.toString();
    }

    private static void printVersion(Context context, String versionFileName, StringBuffer sb) {

        Resources resources = context.getResources();

        int id = resources.getIdentifier(versionFileName, "xml", context.getPackageName());

        if (id != 0) {
            XmlResourceParser parser = resources.getXml(id);

            try {

                int eventType = parser.getEventType();

                while (eventType != XmlPullParser.END_DOCUMENT) {
                    if (eventType == XmlPullParser.START_TAG) {
                        String strNode = parser.getName();
                        if (strNode.equalsIgnoreCase("module")) {
                            String name = parser.getAttributeValue(null, "name");
                            String version = parser.getAttributeValue(null, "version");

                            sb.append(name + " : " + version + "\n");
                        }
                    }

                    eventType = parser.next();
                }
            } catch (XmlPullParserException e) {
                Log.e(TAG, e.toString(), e);
            } catch (IOException e) {
                Log.e(TAG, e.toString(), e);
            } catch (Exception e) {
                Log.e(TAG, e.toString(), e);
            } finally {
                if (parser != null) {
                    parser.close();
                }
            }
        }

    }
}
