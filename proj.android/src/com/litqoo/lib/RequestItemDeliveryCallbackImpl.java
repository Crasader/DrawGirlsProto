package com.litqoo.lib;

import java.util.ArrayList;
import java.util.List;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import com.hangame.hsp.HSPResult;
import com.hangame.hsp.itemdelivery.HSPItemDelivery;
import com.hangame.hsp.itemdelivery.HSPItemDelivery.RequestItemDeliveryCallback;
import com.hangame.hsp.itemdelivery.model.ItemInfo;

public class RequestItemDeliveryCallbackImpl implements RequestItemDeliveryCallback{
	protected int m_key;
	protected Cocos2dxGLSurfaceView m_glView;
	public RequestItemDeliveryCallbackImpl(final int _key, Cocos2dxGLSurfaceView glView)
	{
		m_key = _key;
		m_glView = glView;
	}
	@Override
	public void onRequestItemDelivery(HSPResult hspResult, long transactionId, List<ItemInfo> itemInfoList, String receipt)
	{
		JSONObject r = new JSONObject();
		if (hspResult.isSuccess()) {
			Log.d("litqoo", "success on RequestItemDeli");
			try {
				r.put("issuccess", 1);
				r.put("iteminfocount", itemInfoList.size());
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if (itemInfoList.size() > 0) {
				Log.d("promo", "받은 아템이 있음.");
				// 받은 아이템이 있음                
				// 아이템 적용법1: Game Server가 있으면 게임 서버에 아이템 적용 요청 (Game Server는 Item 서버에 receipt를 이용해서 검증을 해야함)
				// 아이템 적용법2: Standalone 형 게임은 게임클라이언트 자체적으로 아이템 적용 (ItemInfo의 itemSequence는 자체적으로 중복처리를 해야함)

				//                                                    applyItems(itemInfoList); //게임에서 자체 구현함
				// 아이템 적용 완료후 finishItemDelivery 호출
				ArrayList<Long> itemSeq = new ArrayList<Long>();

				for(ItemInfo i : itemInfoList)
				{
					itemSeq.add(i.getItemSequence());
				}
//				HSPItemDelivery.finishItemDelivery(transactionId, itemSequenceList)
				HSPItemDelivery.finishItemDelivery(transactionId, itemSeq);
//				HSPItemDelivery.finishItemDelivery(transactionId, itemSequenceList)
//				finishItemDelivery(transactionId, itemInfoList);
			}
			else {
				// 배송할 상품이 없음
				try {
					r.put("issuccess", 0);
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		} else {
			Log.d("litqoo", "fail on req");
			try {
				r.put("issuccess", 0);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			// 요청 실패
		}
		m_glView.queueEvent(new KRunnable(m_key, r.toString()) {

			@Override
			public void run() {
				// TODO Auto-generated method stub

				hspConnector.SendResult(this.delekey, this.totalSource);
			}
		});
	}
}