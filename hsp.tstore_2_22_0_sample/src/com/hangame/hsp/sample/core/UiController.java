
package com.hangame.hsp.sample.core;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPState;
import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.sample.AbstractViewCategory;
import com.hangame.hsp.sample.core.info.HSPInfo;
import com.hangame.hsp.sample.core.login.HSPLoginAPI;
import com.hangame.hsp.ui.HSPUiFactory;
import com.hangame.hsp.ui.HSPUiLauncher;
import com.hangame.hsp.ui.HSPUiUri;
import com.hangame.hsp.ui.HSPUiUri.HSPUiUriParameterKey;
import com.hangame.hsp.ui.HSPUiUri.HSPUiUriParameterValue;

import android.R;
import android.app.Activity;
import android.app.ProgressDialog;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class UiController {
    private static final String TAG = "UiContorller";

    private static ScrollView logView;

    private static TextView logTextView;

    private static TextView infoTextView;

    private static boolean isTest;

    private static Activity activity;

    private static ProgressDialog progressDialog;

    private static View mainView;

    public static void init(final Activity activity, boolean isTest) {
        UiController.activity = activity;
        UiController.isTest = isTest;

        int mainLayoutId = activity.getResources().getIdentifier("sample_core", "layout", activity.getPackageName());

        mainView = activity.getLayoutInflater().inflate(mainLayoutId, null);
        final ViewGroup selectView = (ViewGroup)mainView.findViewWithTag("sample.select");
        logView = (ScrollView)mainView.findViewWithTag("sample.log");
        logTextView = (TextView)mainView.findViewWithTag("sample.log.text");

        final View loginView = LoginView.initView(activity);
        final View uiView = UiView.initView(activity);
        final View apiView = APIView.initView(activity);
        final View infoView = getLayoutView(activity, "sample_core_info");

        infoTextView = (TextView)infoView.findViewWithTag("sample.info.content");

        selectView.addView(loginView);
        updateMenuView("sample.login");

        mainView.findViewWithTag("sample.login").setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                selectView.removeAllViews();
                selectView.addView(loginView);
                LoginView.updateView();
                logView.scrollTo(0, logTextView.getHeight());

                toast("HSP State = " + HSPCore.getInstance().getState());

                updateMenuView("sample.login");
            }
        });

        mainView.findViewWithTag("sample.ui").setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                selectView.removeAllViews();
                selectView.addView(uiView);
                logView.scrollTo(0, logTextView.getHeight());

                updateMenuView("sample.ui");
            }
        });

        mainView.findViewWithTag("sample.api").setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                selectView.removeAllViews();
                selectView.addView(apiView);
                logView.scrollTo(0, logTextView.getHeight());

                updateMenuView("sample.api");
            }
        });

        mainView.findViewWithTag("sample.info").setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                selectView.removeAllViews();
                selectView.addView(infoView);
                logView.fullScroll(ScrollView.FOCUS_UP);

                infoTextView.setText(HSPInfo.getHSPRuntimeInfo());

                updateMenuView("sample.info");
            }
        });

        activity.setContentView(mainView);
    }

    public static void printHSPInfo() {
        log(HSPInfo.getHSPInfo(activity, isTest));

        log("\n=== Start Test ===\n");
    }

    public static void updateView() {
        LoginView.updateView();
    }

    private static void updateMenuView(String menu) {
        ((Button)mainView.findViewWithTag("sample.login")).setEnabled(true);
        ((Button)mainView.findViewWithTag("sample.ui")).setEnabled(true);
        ((Button)mainView.findViewWithTag("sample.api")).setEnabled(true);
        ((Button)mainView.findViewWithTag("sample.info")).setEnabled(true);

        if ("sample.login".equals(menu)) {
            ((Button)mainView.findViewWithTag("sample.login")).setEnabled(false);
        } else if ("sample.ui".equals(menu)) {
            ((Button)mainView.findViewWithTag("sample.ui")).setEnabled(false);
        } else if ("sample.api".equals(menu)) {
            ((Button)mainView.findViewWithTag("sample.api")).setEnabled(false);
        } else if ("sample.info".equals(menu)) {
            ((Button)mainView.findViewWithTag("sample.info")).setEnabled(false);
        }
    }

    private static View getLayoutView(Activity activity, String layoutName) {
        int viewLayoutId = activity.getResources().getIdentifier(layoutName, "layout", activity.getPackageName());
        return activity.getLayoutInflater().inflate(viewLayoutId, null);
    }

    public static void toast(String message) {
        if (activity != null) {
            Toast.makeText(activity, message, Toast.LENGTH_SHORT).show();
        }
    }

    public static void log(String message) {
        log(null, message);
    }

    public static void log(final String tag, final String message) {
        if (tag != null) {
            logTextView.setText(logTextView.getText().toString() + "\n[" + tag + "] " + message);
            Log.d(tag, message);
        } else {
            logTextView.setText(logTextView.getText().toString() + "\n" + message);
            Log.d("HSPSample", message);
        }

        logView.post(new Runnable() {
            @Override
            public void run() {
                logView.fullScroll(View.FOCUS_DOWN);
            }
        });
    }

    public static synchronized void showProgressDialog() {
        activity.runOnUiThread(new Runnable() {
            public void run() {
                if (progressDialog == null) {
                    progressDialog = new ProgressDialog(activity);
                    progressDialog.setMessage("Please wait a moment.");
                    progressDialog.show();
                }
            }
        });
    }

    public static synchronized void hideProgressDialog() {
        activity.runOnUiThread(new Runnable() {
            public void run() {
                if (progressDialog != null) {
                    progressDialog.dismiss();
                    progressDialog = null;
                }
            }
        });
    }

    public static Activity getActivity() {
        return activity;
    }

    public static Drawable getDrawable(String name) {
        String type = "drawable";
        String packageName = activity.getPackageName();
        int id = activity.getResources().getIdentifier(name, type, packageName);

        if (id == 0) {
            return null;
        } else {
            return activity.getResources().getDrawable(id);
        }
    }

    private static class LoginView {
        private static View manualLogin;

        private static View autoLogin;

        private static View logout;

        private static View reset;

        private static View mapping;

        private static View withdraw;

        private static View initView(final Activity activity) {

            View mainView = getLayoutView(activity, "sample_core_login");

            final HSPLoginAPI loginAPI = new HSPLoginAPI();

            manualLogin = mainView.findViewWithTag("sample.login.manual.login");
            manualLogin.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    loginAPI.login(activity, true);
                }
            });

            autoLogin = mainView.findViewWithTag("sample.login.auto.login");
            autoLogin.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    loginAPI.login(activity, false);
                }
            });

            logout = mainView.findViewWithTag("sample.login.logout");
            logout.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    loginAPI.logout();
                }
            });

            reset = mainView.findViewWithTag("sample.login.reset");
            reset.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    loginAPI.resetAccount();
                }
            });

            mapping = mainView.findViewWithTag("sample.login.mapping");
            mapping.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    loginAPI.requestMappingToAccount();
                }
            });

            withdraw = mainView.findViewWithTag("sample.login.withdraw");
            withdraw.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    loginAPI.withdrawAccount();
                }
            });

            return mainView;
        }

        private static void updateView() {
            if (HSPCore.getInstance().getState() == HSPState.HSP_STATE_ONLINE) {
                manualLogin.setEnabled(true);
                autoLogin.setEnabled(true);
                logout.setEnabled(true);
                reset.setEnabled(true);
                mapping.setEnabled(true);
                withdraw.setEnabled(true);
            } else {
                manualLogin.setEnabled(true);
                autoLogin.setEnabled(true);
                logout.setEnabled(false);
                reset.setEnabled(false);
                mapping.setEnabled(false);
                withdraw.setEnabled(false);
            }
        }
    }

    private static class UiView {
        private static HSPUiUri viewUri;

        private static View initView(final Activity activity) {
            View mainView = getLayoutView(activity, "sample_core_ui");

            final CheckBox gnbShow = (CheckBox)mainView.findViewWithTag("sample.ui.gnb");

            final CheckBox topbarShow = (CheckBox)mainView.findViewWithTag("sample.ui.topbar");

            final Spinner categorySpinner = (Spinner)mainView.findViewWithTag("sample.ui.category");

            final Spinner viewSpinner = (Spinner)mainView.findViewWithTag("sample.ui.view");

            final ArrayAdapter<String> categoryAdapter = new ArrayAdapter<String>(activity,
                    R.layout.simple_spinner_item);

            categoryAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

            categorySpinner.setAdapter(categoryAdapter);

            final ArrayAdapter<String> viewAdapter = new ArrayAdapter<String>(activity, R.layout.simple_spinner_item);

            viewAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

            viewSpinner.setAdapter(viewAdapter);

            final List<AbstractViewCategory> categoryList = ReflectionUtil.getCategoryList(activity);

            categoryAdapter.clear();

            for (AbstractViewCategory viewCategory : categoryList) {
                categoryAdapter.add(viewCategory.getCategoryName());
            }

            categorySpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                @Override
                public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                    final Map<String, HSPUiUri> viewMap = categoryList.get(position).getViewMap();

                    viewAdapter.clear();

                    for (String viewKey : viewMap.keySet()) {
                        viewAdapter.add(viewKey);
                    }

                    viewUri = new ArrayList<HSPUiUri>(viewMap.values()).get(0);

                    viewSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {

                        @Override
                        public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                            String viewKey = (String)viewSpinner.getItemAtPosition(position);

                            android.util.Log.d(TAG, "select view: " + viewKey);

                            viewUri = viewMap.get(viewKey);
                        }

                        @Override
                        public void onNothingSelected(AdapterView<?> arg0) {
                        }
                    });
                }

                @Override
                public void onNothingSelected(AdapterView<?> arg0) {
                }
            });

            mainView.findViewWithTag("sample.ui.launch").setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {

                    android.util.Log.d(TAG, "Launch View: " + viewUri);

                    if (viewUri != null) {
                        HSPUiUri uri = HSPUiFactory.getUiUri(viewUri.getAction());

                        uri.setParameter(viewUri.getParameters());

                        if (topbarShow.isChecked()) {
                            uri.setParameter(HSPUiUriParameterKey.TOPBAR_SHOW, HSPUiUriParameterValue.TRUE);
                        } else {
                            uri.setParameter(HSPUiUriParameterKey.TOPBAR_SHOW, HSPUiUriParameterValue.FALSE);
                        }

                        if (gnbShow.isChecked()) {
                            uri.setParameter(HSPUiUriParameterKey.GNB_SHOW, HSPUiUriParameterValue.TRUE);
                        } else {
                            uri.setParameter(HSPUiUriParameterKey.GNB_SHOW, HSPUiUriParameterValue.FALSE);
                        }

                        HSPUiLauncher.getInstance().launch(uri);
                    }

                }
            });

            return mainView;
        }
    }

    private static class APIView {
        private static AbstractModule module;

        private static Method method;

        private static View initView(final Activity activity) {
            View mainView = getLayoutView(activity, "sample_core_api");

            final Spinner moduleSpinner = (Spinner)mainView.findViewWithTag("sample.api.module");

            final Spinner methodSpinner = (Spinner)mainView.findViewWithTag("sample.api.method");

            final ArrayAdapter<String> moduleAdapter = new ArrayAdapter<String>(activity, R.layout.simple_spinner_item);

            moduleAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

            moduleSpinner.setAdapter(moduleAdapter);

            final ArrayAdapter<String> methodAdapter = new ArrayAdapter<String>(activity, R.layout.simple_spinner_item);

            methodAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

            methodSpinner.setAdapter(methodAdapter);

            final List<AbstractModule> moduleList = ReflectionUtil.getAPIModuleList(activity);

            moduleAdapter.clear();

            for (AbstractModule module : moduleList) {
                moduleAdapter.add(module.getModuleName());
            }

            moduleSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                @Override
                public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                    module = moduleList.get(position);

                    final List<Method> methodList = ReflectionUtil.getAPIMethodList(module.getClass());

                    methodAdapter.clear();

                    for (Method method : methodList) {
                        methodAdapter.add(method.getName().substring(4));
                    }

                    method = methodList.get(0);

                    methodSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {

                        @Override
                        public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                            method = methodList.get(position);
                        }

                        @Override
                        public void onNothingSelected(AdapterView<?> arg0) {
                        }
                    });
                }

                @Override
                public void onNothingSelected(AdapterView<?> arg0) {
                }
            });

            mainView.findViewWithTag("sample.api.call").setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    if (method != null) {
                        try {
                            log("Call " + module.getModuleName() + "." + method.getName() + "()");
                            method.invoke(module, null);
                        } catch (InvocationTargetException e) {
                            log(e.getCause().toString());
                            e.getCause().printStackTrace();
                        } catch (Exception e) {
                            log(e.toString());
                            e.printStackTrace();
                        }
                    }

                }
            });

            return mainView;
        }
    }
}
