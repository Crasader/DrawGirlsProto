
package com.hangame.hsp.sample.core;

import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.sample.AbstractViewCategory;
import com.hangame.hsp.util.Log;

import dalvik.system.DexFile;

import android.content.Context;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

class ReflectionUtil {

    static List<Method> getAPIMethodList(Class clazz) {
        List<Method> methodList = new ArrayList<Method>();

        Method[] methods = clazz.getMethods();

        for (Method method : methods) {
            if (method.getName().startsWith("test")) {
                methodList.add(method);
            }
        }

        return methodList;
    }

    static List<AbstractModule> getAPIModuleList(Context context) {
        List<AbstractModule> moduleList = new ArrayList<AbstractModule>();

        try {
            String apkName = context.getPackageManager().getApplicationInfo(context.getPackageName(), 0).sourceDir;
            DexFile dexFile = new DexFile(apkName);

            // Print all classes in the DexFile
            for (Enumeration<String> classNames = dexFile.entries(); classNames.hasMoreElements();) {
                String className = classNames.nextElement();

                if (className != null) {
                    if (className.matches("com.hangame.hsp.sample.*") && className.matches(".*API")) {
                        Log.i("getAPIModuleList!!!", "class:: " + className);

                        if (getAPIMethodList(Class.forName(className)).size() > 0) {
                            moduleList.add((AbstractModule)Class.forName(className).newInstance());
                        }
                    }
                }
            }
        } catch (Exception e) {
            Log.w("getAPIModuleList!!!", "Error opening ", e);
        }
        return moduleList;
    }

    static List<AbstractViewCategory> getCategoryList(Context context) {
        List<AbstractViewCategory> categoryList = new ArrayList<AbstractViewCategory>();

        try {
            String apkName = context.getPackageManager().getApplicationInfo(context.getPackageName(), 0).sourceDir;
            DexFile dexFile = new DexFile(apkName);

            // Print all classes in the DexFile
            for (Enumeration<String> classNames = dexFile.entries(); classNames.hasMoreElements();) {
                String className = classNames.nextElement();

                if (className != null) {
                    if (className.matches("com.hangame.hsp.sample.*") && className.matches(".*View")
                            && (className.contains(".ui") || className.contains(".view"))) {
                        Log.i("getCategoryList!!!", "class:: " + className);
                        categoryList.add((AbstractViewCategory)Class.forName(className).newInstance());
                    }
                }
            }
        } catch (Exception e) {
            Log.w("getCategoryList!!!", "Error opening ", e);
        }

        return categoryList;
    }

}
