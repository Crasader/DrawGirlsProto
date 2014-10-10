package com.litqoo.lib;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.itemdelivery.HSPItemDelivery;
import com.hangame.hsp.payment.HSPPayment.PurchaseCB;
import com.hangame.hsp.payment.core.constant.PaymentErrorCode;

public class PurchaseCBImpl implements PurchaseCB {       
	protected int m_key;
	protected Cocos2dxGLSurfaceView m_glView;
	public PurchaseCBImpl(final int _key, Cocos2dxGLSurfaceView glView)
	{
		m_key = _key;
		m_glView = glView;
		
	}
    public void onPurchase(HSPResult hspResult, Object context) {

    	Log.i("litqoo.sumran",hspResult.toString());
    	
    	if (hspResult == null) {
        	Log.i("litqoo.sumran","return null");
            return;
        }

        JSONObject r = new JSONObject();
        

        if (hspResult.getCode() == PaymentErrorCode.PURCHASE_SUCCESS) {
        	Log.i("litqoo.sumran","it's success");
            // ���� ����
//        	Log.d("litqoo", "���� ����");
//        	hspResult.
        	try {
				r.put("issuccess", 1);
			} catch (JSONException e) {
	        	Log.i("litqoo.sumran","it's success2");
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        	m_glView.queueEvent(new KRunnable(m_key, r.toString()) {
        		public void run() {
        			hspConnector.SendResult(this.delekey,this.totalSource);
        		}
        	});
//        	HSPItemDelivery.requestItemDelivery(new RequestItemDeliveryCallbackImpl(m_key, m_glView));
        } else {
        	Log.i("litqoo.sumran","it's fail");
        	// ���� ����
//        	Log.d("litqoo", "���� ����" + Integer.toString(hspResult.getCode()));
        	try {
				r.put("issuccess", 0);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
	        	Log.i("litqoo.sumran","it's fail2");
				e.printStackTrace();
			}
        	
            
            m_glView.queueEvent(new KRunnable(m_key, r.toString()) {
            	public void run() {
            		hspConnector.SendResult(this.delekey,this.totalSource);
            	}
            });
        }
        

    }
}