package com.litqoo.lib;


import java.util.HashMap;
import java.util.List;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.util.Log;

import com.hangame.hsp.HSPResult;
import com.hangame.hsp.cgp.HSPCGP;
import com.hangame.hsp.cgp.HSPCGP.CheckPromotionMapCB;
import com.hangame.hsp.cgp.constant.CGPType.ShapeType;
import com.hangame.hsp.cgp.model.PromotionItem;
import com.hangame.hsp.cgp.model.PromotionState;
public class CheckPromotionMapCBImpl implements CheckPromotionMapCB {
	protected int m_key;
	protected Cocos2dxGLSurfaceView m_glView;
	protected Context m_context;
	public CheckPromotionMapCBImpl(final int _key, Cocos2dxGLSurfaceView glView, Context context)
	{
		m_key = _key;
		m_glView = glView;	
		m_context = context;
	}
	/**
	 * On check promotion.
	 *
	 * @param hspResult ó�� ��� result
	 * @param context Caller Context
	 * @param promotionState ���θ�� ���°�
	 */
	@Override
	public void onCheckPromotion(
			HSPResult hspResult, Object context,
			HashMap<ShapeType, PromotionState> promotionStateMap)       {
		if (!hspResult.isSuccess()) {
			
			Log.i("litqoo.sumran","cgp error");
			
			JSONObject r = new JSONObject();
			try {
				r.put("promotionstate","CGP_NONE");
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			m_glView.queueEvent(new KRunnable(m_key, r.toString()) {

				@Override
				public void run() {
					// TODO Auto-generated method stub

					hspConnector.SendResult(this.delekey, this.totalSource);
				}
			});
			return;
		}


		ShapeType shpeTp = ShapeType.NORMAL; // �Ϲ� : ��ư, ���, �˾�

//		ShapeType shpeTp = ShapeType.ENDING;    // ����
		//ShapeType shpeTp = ShapeType.FREECHARGE;          // ��������

//		PromotionState ps1 = promotionStateMap.get(ShapeType.FREECHARGE);
//		PromotionState ps2 = promotionStateMap.get(ShapeType.NORMAL);
//		PromotionState ps3 = promotionStateMap.get(ShapeType.ENDING);
		
		JSONObject r = new JSONObject();
		try {
			r.put("promotionstate", promotionStateMap.get(shpeTp).name());
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		switch (promotionStateMap.get(shpeTp)) {
		case CGP_NONE: {
			// ���θ�� �����Ͱ� ����, �߰����� ��� ó�� ����
			Log.d("promo", "none");
			break;
		}
		case CGP_PROMOTION_EXISTS: {
			// ���θ�� ���� ����           
			List<PromotionItem> promoList = HSPCGP.getPromotionInfo(shpeTp);
			if(promoList != null && promoList.size() !=0 ){
				PromotionItem promoItem = promoList.get(0);
				// ���θ�������� �ϳ��̱� ������ ù��°�� ���� ��´�.
				hspConnector.setmPromoItem(promoItem);
				try {
					r.put("typecode", promoItem.getTypeCode());
					switch(promoItem.getTypeCode())
					{
					case 1: // ��ư.
						r.put("eventurl", promoItem.getEventUrl());
						r.put("buttonurl", promoItem.getButtonUrl());
						r.put("bubbletext", promoItem.getBubbleText());
						//					r.put("buttonImg", promoItem.getButtonImg());
						break;
					case 2: // ���.
						r.put("bannerlandurl", promoItem.getPromotionBannerLandUrl());
						r.put("bannerporturl", promoItem.getPromotionBannerPortUrl());
						break;
					case 3: // �˾�.
						break;
					case 4: // ����.
						break;
					case 5: // ���� ����.
						break;

					}
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				}
				Log.d("promo", "123");
			}

			break;
		
		case CGP_REWARD_REQUIRED: {
			// ���� ���� ����            
			List<PromotionItem> promoList = HSPCGP.getPromotionInfo(shpeTp);

			JSONArray jsonArray = new JSONArray();
			if(promoList.size() >= 1)
			{
				PromotionItem onePromo = promoList.get(0);
				JSONObject temp = new JSONObject();
				try {
					temp.put("rewardvalue", onePromo.getRewardValue()); // ���� ��.
					temp.put("rewardcode", onePromo.getRewardCode()); // ���� �ڵ�.
					temp.put("promotiontype", onePromo.getPromotionType());  //���θ�� ���� 1:���θ��, 2: �Ϲݺ���(��ġ���θ��,�̼����θ��), 3:��ġ����(1+1���θ�ǿ��� ��ġ����)
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				jsonArray.put(temp);
			}
			try {
				r.put("rewards", jsonArray);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			break;
		}
		case CGP_PROMOTION_REWARD_EXISTS: {
			// ���� ���� ����            
			// ���� ���� ����            
			
			List<PromotionItem> promoList = HSPCGP.getPromotionInfo(shpeTp);

			JSONArray jsonArray = new JSONArray();
			if(promoList.size() >= 1)
			{
				PromotionItem onePromo = promoList.get(0);
				JSONObject temp = new JSONObject();
				try {
					temp.put("rewardvalue", onePromo.getRewardValue()); // ���� ��.
					temp.put("rewardcode", onePromo.getRewardCode()); // ���� �ڵ�.
					temp.put("promotiontype", onePromo.getPromotionType());  //���θ�� ���� 1:���θ��, 2: �Ϲݺ���(��ġ���θ��,�̼����θ��), 3:��ġ����(1+1���θ�ǿ��� ��ġ����)
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				jsonArray.put(temp);
			}
//			for (int i = 0; i < promoList.size(); i++) {
//			}
			try {
				r.put("rewards", jsonArray);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			break;
		}

		}
//		HSPCGP.launchPromotion((Activity)m_context);
		m_glView.queueEvent(new KRunnable(m_key, r.toString()) {

			@Override
			public void run() {
				// TODO Auto-generated method stub

				hspConnector.SendResult(this.delekey, this.totalSource);
			}
		});
	}
}


