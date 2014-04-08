
package com.hangame.hsp.sample;

import com.hangame.hsp.sample.core.UiController;

import android.app.Activity;
import android.util.Log;

public abstract class AbstractModule {

    private final String moduleName;

    private static final long DEFAULT_TIMEOUT = 20 * 1000;

    private final Object lock = new Object[0];

    private boolean lockFlag = false;

    protected AbstractModule(String moduleName) {
        this.moduleName = moduleName;
    }

    public String getModuleName() {
        return moduleName;
    }

    protected void log(String message) {
        UiController.log(moduleName + " Module", message);
    }

    protected void lock() {
        Log.d(moduleName, "lock IN");

        synchronized (lock) {
            if (lockFlag == false) {
                lockFlag = true;

                try {
                    showProgressDialog();
                    lock.wait(DEFAULT_TIMEOUT);
                    hideProgressDialog();
                } catch (InterruptedException e) {
                    Log.e(moduleName, e.toString(), e);
                }
            } else {
                Log.e(moduleName, "Lock is aleardy unlocked");
            }
        }

        Log.d(moduleName, "lock OUT");
    }

    protected void unlock() {
        Log.d(moduleName, "unlock IN");

        synchronized (lock) {
            if (lockFlag == true) {
                lockFlag = false;
                lock.notifyAll();
            } else {
                Log.e(moduleName, "Lock is aleardy unlocked");
            }
        }

        Log.d(moduleName, "unlock OUT");
    }

    protected void showProgressDialog() {
        UiController.showProgressDialog();
    }

    protected void hideProgressDialog() {
        UiController.hideProgressDialog();
    }

    protected void updateState() {
        UiController.updateView();
    }

    protected void showToast(String message) {
        UiController.toast(message);
    }

    protected Activity getActivity() {
        return UiController.getActivity();
    }
}
