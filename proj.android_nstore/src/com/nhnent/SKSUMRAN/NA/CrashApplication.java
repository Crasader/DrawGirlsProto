/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.nhnent.SKSUMRAN.NA;

import org.acra.ACRA;
import org.acra.ReportField;
import org.acra.ReportingInteractionMode;
import org.acra.annotation.ReportsCrashes;
import org.acra.sender.HttpSender;

import android.app.Application;

@ReportsCrashes
(
		formUri = "http://hapill.com/wp/acra.php",
	    
	    formKey = "", // This is required for backward compatibility but not used
	    customReportContent = {
	            ReportField.APP_VERSION_CODE,
	            ReportField.APP_VERSION_NAME,
	            ReportField.ANDROID_VERSION,
	            ReportField.PACKAGE_NAME,
	            ReportField.REPORT_ID,
	            ReportField.BUILD,
	            ReportField.STACK_TRACE,
	            ReportField.LOGCAT,
	            
	    },
	    mode = ReportingInteractionMode.SILENT
)

public class CrashApplication extends Application {
	@Override
    public void onCreate() {
        // TODO Auto-generated method stub
        super.onCreate();
        ACRA.init(this);
//        MyReportSender yourSender = new MyReportSender();
//        ACRA.getErrorReporter().setReportSender(yourSender);
    }
}
