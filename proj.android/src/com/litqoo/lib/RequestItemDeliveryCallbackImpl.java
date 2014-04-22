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
				Log.d("promo", "���� ������ ����.");
				// ���� �������� ����                
				// ������ �����1: Game Server�� ������ ���� ������ ������ ���� ��û (Game Server�� Item ������ receipt�� �̿��ؼ� ������ �ؾ���)
				// ������ �����2: Standalone �� ������ ����Ŭ���̾�Ʈ ��ü������ ������ ���� (ItemInfo�� itemSequence�� ��ü������ �ߺ�ó���� �ؾ���)

				//                                                    applyItems(itemInfoList); //���ӿ��� ��ü ������
				// ������ ���� �Ϸ��� finishItemDelivery ȣ��
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
				// ����� ��ǰ�� ����
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
			// ��û ����
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