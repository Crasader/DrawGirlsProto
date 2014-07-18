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
	 * @param hspResult 처리 결과 result
	 * @param context Caller Context
	 * @param promotionState 프로모션 상태값
	 */
	@Override
	public void onCheckPromotion(
			HSPResult hspResult, Object context,
			HashMap<ShapeType, PromotionState> promotionStateMap)       {
		if (!hspResult.isSuccess()) {
			// checkPromotion 요청 실패, 오류 코드 확인 가능
			return;
		}


		ShapeType shpeTp = ShapeType.NORMAL; // 일반 : 버튼, 배너, 팝업

//		ShapeType shpeTp = ShapeType.ENDING;    // 엔딩
		//ShapeType shpeTp = ShapeType.FREECHARGE;          // 무료충전

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
			// 프로모션 데이터가 없음, 추가적인 기능 처리 없음
			Log.d("promo", "none");
			break;
		}
		case CGP_PROMOTION_EXISTS: {
			// 프로모션 정보 존재           
			List<PromotionItem> promoList = HSPCGP.getPromotionInfo(shpeTp);
			if(promoList != null && promoList.size() !=0 ){
				PromotionItem promoItem = promoList.get(0);
				// 프로모션정보는 하나이기 때문에 첫번째의 것을 얻는다.
				hspConnector.setmPromoItem(promoItem);
				try {
					r.put("typecode", promoItem.getTypeCode());
					switch(promoItem.getTypeCode())
					{
					case 1: // 버튼.
						r.put("eventurl", promoItem.getEventUrl());
						r.put("buttonurl", promoItem.getButtonUrl());
						r.put("bubbletext", promoItem.getBubbleText());
						//					r.put("buttonImg", promoItem.getButtonImg());
						break;
					case 2: // 배너.
						r.put("bannerlandurl", promoItem.getPromotionBannerLandUrl());
						r.put("bannerporturl", promoItem.getPromotionBannerPortUrl());
						break;
					case 3: // 팝업.
						break;
					case 4: // 엔딩.
						break;
					case 5: // 무료 충전.
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
			// 보상 정보 존재            
			List<PromotionItem> promoList = HSPCGP.getPromotionInfo(shpeTp);

			JSONArray jsonArray = new JSONArray();
			if(promoList.size() >= 1)
			{
				PromotionItem onePromo = promoList.get(0);
				JSONObject temp = new JSONObject();
				try {
					temp.put("rewardvalue", onePromo.getRewardValue()); // 보상 값.
					temp.put("rewardcode", onePromo.getRewardCode()); // 보상 코드.
					temp.put("promotiontype", onePromo.getPromotionType());  //프로모션 형태 1:프로모션, 2: 일반보상(설치프로모션,미션프로모션), 3:설치보상(1+1프로모션에서 설치보상)
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
			// 보상 정보 존재            
			// 보상 정보 존재            
			
			List<PromotionItem> promoList = HSPCGP.getPromotionInfo(shpeTp);

			JSONArray jsonArray = new JSONArray();
			if(promoList.size() >= 1)
			{
				PromotionItem onePromo = promoList.get(0);
				JSONObject temp = new JSONObject();
				try {
					temp.put("rewardvalue", onePromo.getRewardValue()); // 보상 값.
					temp.put("rewardcode", onePromo.getRewardCode()); // 보상 코드.
					temp.put("promotiontype", onePromo.getPromotionType());  //프로모션 형태 1:프로모션, 2: 일반보상(설치프로모션,미션프로모션), 3:설치보상(1+1프로모션에서 설치보상)
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


