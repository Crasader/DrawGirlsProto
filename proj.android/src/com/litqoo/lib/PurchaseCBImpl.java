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
        if (hspResult == null) {
            return;
        }
        if (hspResult.getCode() == PaymentErrorCode.PURCHASE_SUCCESS) {
            // 결제 성공
        	Log.d("litqoo", "결제 성공");
        	HSPItemDelivery.requestItemDelivery(new RequestItemDeliveryCallbackImpl(m_key, m_glView));
        } else {
        	
        	JSONObject r = new JSONObject();
            // 결제 실패
        	Log.d("litqoo", "결제 실패" + Integer.toString(hspResult.getCode()));
        	try {
				r.put("issuccess", 0);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        	m_glView.queueEvent(new KRunnable(m_key,r.toString()) {
        		public void run() {

        			Log.d("litqoo", "login id:"+HSPCore.getInstance().getMemberID()+"/no:"+HSPCore.getInstance().getMemberNo());
        			Log.d("litqoo", "SendResult"+this.totalSource);
        			hspConnector.SendResult(this.delekey,this.totalSource);
        		}
        	});
        }
    }
}