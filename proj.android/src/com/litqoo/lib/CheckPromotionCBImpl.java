package com.litqoo.lib;


import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.cgp.HSPCGP;
import com.hangame.hsp.cgp.HSPCGP.CheckPromotionCB;

import com.hangame.hsp.cgp.HSPCGP.CheckPromotionCB;
import com.hangame.hsp.cgp.model.PromotionState;
public class CheckPromotionCBImpl implements CheckPromotionCB {
	
	protected int m_key;
	protected Cocos2dxGLSurfaceView m_glView;
	public CheckPromotionCBImpl(final int _key, Cocos2dxGLSurfaceView glView)
	{
		m_key = _key;
		m_glView = glView;
		
	}
	@Override 
	public void onCheckPromotion(HSPResult hspResult, Object context, PromotionState promotionState)
	{
		if(!hspResult.isSuccess())
		{
			// 요청 실패.
			return;
		}
		JSONObject r = new JSONObject();
		try {
			r.put("promotionstate", promotionState.name());
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
		switch(promotionState)
		{
		case CGP_NONE:
		{
			break;
		}
		case CGP_PROMOTION_EXISTS:
		{
			break;
		}
		case CGP_REWARD_REQUIRED:
		{

			break;
		}
		}
	}
}
