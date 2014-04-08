
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPMemberDataStorage;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.util.Log;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class HSPMemberDataAPI extends AbstractModule {
    private static final String TAG = "HSPMemberDataAPI";
    
    public HSPMemberDataAPI() {
        super("HSPMemberData");
    }

    public void testSaveMemberData(){
        
        Map<String, String> param = new HashMap<String, String>();
        param.put("key1", "val1");
        param.put("key2", "val2");
        
        HSPMemberDataStorage.saveMemberData(param, new HSPMemberDataStorage.HSPSaveMemberDataCB() {
            
            @Override
            public void onMemberDataSave(HSPResult result) {
                if(result.isSuccess()){
                    Log.d(TAG, "save member data success");
                    log("save member data success");
                }
            }
        });
        
    }
    
    public void testLoadMemberData(){
        
        ArrayList<String> param = new ArrayList<String>();
        param.add("key1");
        param.add("key2");
        
        HSPMemberDataStorage.loadMemberData(param, new HSPMemberDataStorage.HSPLoadMemberDataCB() {

            @Override
            public void onMemberDataLoad(Map<String, String> Map, HSPResult result) {
                if(result.isSuccess()){
                    Log.d(TAG, "load member data success :: " + Map.toString());
                    log("load member data success :: " + Map.toString());
                }
            }
        });
        
    }
    
    public void testLoadMemberDataFromMemberNo(){
        
        long memberNo = HSPCore.getInstance().getMemberNo();
        
        ArrayList<String> param = new ArrayList<String>();
        param.add("key1");
        param.add("key2");
        
        HSPMemberDataStorage.loadMemberDataFromMemberNo(memberNo, param, new HSPMemberDataStorage.HSPLoadMemberDataCB() {

            @Override
            public void onMemberDataLoad(Map<String, String> Map, HSPResult result) {
                if(result.isSuccess()){
                    Log.d(TAG, "load member data success :: " + Map.toString());
                    log("load member data success :: " + Map.toString());
                }
            }
        });
        
    }
    
    
    public void testRemoveMemberData(){
        
        ArrayList<String> param = new ArrayList<String>();
        param.add("key1");
        param.add("key2");
        
        HSPMemberDataStorage.removeMemberData(param, new HSPMemberDataStorage.HSPRemoveMemberDataCB() {

            @Override
            public void onMemberDataRemove(HSPResult result) {
                if(result.isSuccess()){
                    Log.d(TAG, "remove member data success :: ");
                    log("remove member data success :: ");
                }
            }
        });
        
    }
}
