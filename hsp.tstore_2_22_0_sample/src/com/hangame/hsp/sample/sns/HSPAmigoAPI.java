package com.hangame.hsp.sample.sns;

import com.hangame.hsp.HSPCore;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.sns.HSPAmigo;
import com.hangame.hsp.sns.HSPAmigo.HSPAmigoFollowingType;
import com.hangame.hsp.sns.HSPAmigo.HSPAmigoProposedType;
import com.hangame.hsp.sns.HSPAmigo.HSPAmigoRelationCode;
import com.hangame.hsp.sns.HSPAmigo.HSPAmigoRelationResultType;
import com.hangame.hsp.sns.HSPAmigo.HSPAmigoSocialPointType;
import com.hangame.hsp.sns.data.HSPAmigoRelationInfo;
import com.hangame.hsp.sns.data.HSPAmigoRelationResult;


import com.hangame.hsp.util.Log;

import java.util.ArrayList;
import java.util.List;

public class HSPAmigoAPI extends AbstractModule {
	private static final String TAG = "HSPAmigoAPI";

	private List<Long> mMemberList = null;

	private long mJeawoo = 4105000000000065L; // 정재우, 카톡 platformQA존
	
	private long mHspTest = 4105000000001336L; // hsptest , 카톡 platformQA존

	private long mTargetMemberNo = 0L; // 정재우, 카톡 platformQA존
	
	private long mSocialPoint = 98L;		// Social point test, Add
	
	private long mSocialPointUpdate = 102L;		// Social point test, Update
	
	private String mExtraTestData = "extradata test..";
	
	private long mMyMemberNo = -1;

	public HSPAmigoAPI() {
		super("HSPAmigo");
	}

	public void changeMemberNo() {
		mMyMemberNo = HSPCore.getInstance().getMemberNo();

		if (mMyMemberNo == mJeawoo ) {
			mTargetMemberNo = mHspTest;
		} else {
			mTargetMemberNo = mJeawoo;
		}

		log("This MemberNo is " + mMyMemberNo + ", TargetMemberNo is " + mTargetMemberNo);

		if ( mMemberList != null ) 
		{
			mMemberList.clear();
		}
		mMemberList = new ArrayList<Long>();
		mMemberList.add(mTargetMemberNo);
	}
	
	
	public long getTargetMemberNo() {
		changeMemberNo();
		
		return mTargetMemberNo;
	}
	
	public List<Long> getTargetMemberList() {
		changeMemberNo();
		
		mMemberList = new ArrayList<Long>();
		mMemberList.add(mTargetMemberNo);
		
		return mMemberList;
	}
	
	

	///////////////////////////////////////////////////////////////////////////////////////////////
	// 1. command method
	public void testAcceptMate(){
		HSPAmigo.acceptMate(getTargetMemberList(), new HSPAmigo.HSPActionMateCB() {
			@Override
			public void onReceive(List<HSPAmigoRelationResult> proposeResultList,
					HSPResult result) {

				if(result.isSuccess()){
					Log.d(TAG, "acceptMate is successed");
					log("Query result size is " + proposeResultList.size());
					for (final HSPAmigoRelationResult proposeResult : proposeResultList) {
						log("Member Number: " + proposeResult.getMemberNo());
						log("OpponentMember Number: " + proposeResult.getOpponentMemberNo());
						
						HSPAmigoRelationResultType resultType = proposeResult.getResult();
						if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_SUCCESS)) {
							log("요청 성공");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_ACCEPTED)) {
							log("요청을 이미 수락한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_REQUESTED)) {
							log("이미 요청을 보낸 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_BLOCKED )) {
							log("차단을 한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_MATE )) {
							log("쌍방 요청을 수락하여 MATE가 맺어진 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_NO_MATE  )) {
							log("MATE가 아닌 상태");
						}
					}
				}	else {
					log( "acceptMate is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}

	public void testCancelBlockMate(){
		HSPAmigo.cancelBlockMate(getTargetMemberList(), new HSPAmigo.HSPActionMateCB() {
			@Override
			public void onReceive(List<HSPAmigoRelationResult> proposeResultList,
					HSPResult result) {

				if(result.isSuccess()){
					Log.d(TAG, "cancelBlockMate is successed");
					log("Query result size is " + proposeResultList.size());
					for (final HSPAmigoRelationResult proposeResult : proposeResultList) {
						log("Member Number: " + proposeResult.getMemberNo());
						log("OpponentMember Number: " + proposeResult.getOpponentMemberNo());
						HSPAmigoRelationResultType resultType = proposeResult.getResult();
						
						if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_SUCCESS)) {
							log("요청 성공");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_ACCEPTED)) {
							log("요청을 이미 수락한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_REQUESTED)) {
							log("이미 요청을 보낸 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_BLOCKED )) {
							log("차단을 한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_MATE )) {
							log("쌍방 요청을 수락하여 MATE가 맺어진 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_NO_MATE  )) {
							log("MATE가 아닌 상태");
						}
					}
				}	else {
					log( "cancelBlockMate is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );

				}
			}
		});
	}

	public void testBlockProposalMates(){
		HSPAmigo.blockProposalMates(getTargetMemberList(), new HSPAmigo.HSPActionMateCB() {
			@Override
			public void onReceive(List<HSPAmigoRelationResult> proposeResultList,
					HSPResult result) {

				if(result.isSuccess()){
					Log.d(TAG, "blockProposalMates is successed");
					log("Query result size is " + proposeResultList.size());
					for (final HSPAmigoRelationResult proposeResult : proposeResultList) {
						log("Member Number: " + proposeResult.getMemberNo());
						log("OpponentMember Number: " + proposeResult.getOpponentMemberNo());
						
						HSPAmigoRelationResultType resultType = proposeResult.getResult();
						if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_SUCCESS)) {
							log("요청 성공");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_ACCEPTED)) {
							log("요청을 이미 수락한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_REQUESTED)) {
							log("이미 요청을 보낸 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_BLOCKED )) {
							log("차단을 한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_MATE )) {
							log("쌍방 요청을 수락하여 MATE가 맺어진 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_NO_MATE  )) {
							log("MATE가 아닌 상태");
						}
					}
				}	else {
					log( "blockProposalMates is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}

	public void testDeleteMatePropsal(){
		HSPAmigo.deleteMatePropsal(getTargetMemberList(), new HSPAmigo.HSPActionMateCB() {
			@Override
			public void onReceive(List<HSPAmigoRelationResult> proposeResultList,
					HSPResult result) {

				if(result.isSuccess()){
					Log.d(TAG, "deleteMatePropsal is successed");
					log("Query result size is " + proposeResultList.size());
					for (final HSPAmigoRelationResult proposeResult : proposeResultList) {
						log("Member Number: " + proposeResult.getMemberNo());
						log("OpponentMember Number: " + proposeResult.getOpponentMemberNo());
						
						HSPAmigoRelationResultType resultType = proposeResult.getResult();
						if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_SUCCESS)) {
							log("요청 성공");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_ACCEPTED)) {
							log("요청을 이미 수락한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_REQUESTED)) {
							log("이미 요청을 보낸 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_BLOCKED )) {
							log("차단을 한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_MATE )) {
							log("쌍방 요청을 수락하여 MATE가 맺어진 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_NO_MATE  )) {
							log("MATE가 아닌 상태");
						}
					}
				}else {
					log( "deleteMatePropsal is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}

			}
		});
	}    

	public void testDismissFollowers(){
		HSPAmigo.dismissFollowers(getTargetMemberList(), new HSPAmigo.HSPActionMateCB() {
			@Override
			public void onReceive(List<HSPAmigoRelationResult> proposeResultList,
					HSPResult result) {

				if(result.isSuccess()){
					Log.d(TAG, "dismissFollowers is successed");
					log("Query result size is " + proposeResultList.size());
					for (final HSPAmigoRelationResult proposeResult : proposeResultList) {
						log("Member Number: " + proposeResult.getMemberNo());
						log("OpponentMember Number: " + proposeResult.getOpponentMemberNo());
						
						HSPAmigoRelationResultType resultType = proposeResult.getResult();
						if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_SUCCESS)) {
							log("요청 성공");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_ACCEPTED)) {
							log("요청을 이미 수락한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_REQUESTED)) {
							log("이미 요청을 보낸 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_BLOCKED )) {
							log("차단을 한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_MATE )) {
							log("쌍방 요청을 수락하여 MATE가 맺어진 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_NO_MATE  )) {
							log("MATE가 아닌 상태");
						}
					}
				}else {
					log( "dismissFollowers is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}

			}
		});
	}    

	public void testDismissMate(){
		HSPAmigo.dismissMate(getTargetMemberList(), new HSPAmigo.HSPActionMateCB() {
			@Override
			public void onReceive(List<HSPAmigoRelationResult> proposeResultList,
					HSPResult result) {

				if(result.isSuccess()){
					Log.d(TAG, "dismissMate is successed");
					log("Query result size is " + proposeResultList.size());
					for (final HSPAmigoRelationResult proposeResult : proposeResultList) {
						log("Member Number: " + proposeResult.getMemberNo());
						log("OpponentMember Number: " + proposeResult.getOpponentMemberNo());
						
						HSPAmigoRelationResultType resultType = proposeResult.getResult();
						if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_SUCCESS)) {
							log("요청 성공");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_ACCEPTED)) {
							log("요청을 이미 수락한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_REQUESTED)) {
							log("이미 요청을 보낸 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_BLOCKED )) {
							log("차단을 한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_MATE )) {
							log("쌍방 요청을 수락하여 MATE가 맺어진 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_NO_MATE  )) {
							log("MATE가 아닌 상태");
						}
					}
				}else {
					log("dismissMate is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}

			}
		});
	}    

	public void testProposeMate(){
		HSPAmigo.proposeMate( getTargetMemberList(), new HSPAmigo.HSPActionMateCB() {
			@Override
			public void onReceive(List<HSPAmigoRelationResult> proposeResultList,
					HSPResult result) {

				if(result.isSuccess()){
					Log.d(TAG, "proposeMate is successed");
					log("Query result size is " + proposeResultList.size());
					for (final HSPAmigoRelationResult proposeResult : proposeResultList) {
						log("Member Number: " + proposeResult.getMemberNo());
						log("OpponentMember Number: " + proposeResult.getOpponentMemberNo());
						
						HSPAmigoRelationResultType resultType = proposeResult.getResult();
						if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_SUCCESS)) {
							log("요청 성공");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_ACCEPTED)) {
							log("요청을 이미 수락한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_REQUESTED)) {
							log("이미 요청을 보낸 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_BLOCKED )) {
							log("차단을 한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_MATE )) {
							log("쌍방 요청을 수락하여 MATE가 맺어진 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_NO_MATE  )) {
							log("MATE가 아닌 상태");
						}
					}
				}else {
					log("proposeMate is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}

			}
		});
	}    
	
	public void testBlockMate(){
		HSPAmigo.blockMate( getTargetMemberList(), new HSPAmigo.HSPActionMateCB() {
			@Override
			public void onReceive(List<HSPAmigoRelationResult> proposeResultList,
					HSPResult result) {
				
				if(result.isSuccess()){
					Log.d(TAG, "blockMate is successed");
					log("Query result size is " + proposeResultList.size());
					for (final HSPAmigoRelationResult proposeResult : proposeResultList) {
						log("Member Number: " + proposeResult.getMemberNo());
						log("OpponentMember Number: " + proposeResult.getOpponentMemberNo());
						
						HSPAmigoRelationResultType resultType = proposeResult.getResult();
						if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_SUCCESS)) {
							log("요청 성공");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_ACCEPTED)) {
							log("요청을 이미 수락한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_REQUESTED)) {
							log("이미 요청을 보낸 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_BLOCKED )) {
							log("차단을 한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_MATE )) {
							log("쌍방 요청을 수락하여 MATE가 맺어진 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_NO_MATE  )) {
							log("MATE가 아닌 상태");
						}
					}
				}else {
					log("blockMate is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}    

	public void testRejectMate(){
		HSPAmigo.rejectMate( getTargetMemberList(), new HSPAmigo.HSPActionMateCB() {
			@Override
			public void onReceive(List<HSPAmigoRelationResult> proposeResultList,
					HSPResult result) {

				if(result.isSuccess()){
					Log.d(TAG, "rejectMate is successed");
					for (final HSPAmigoRelationResult proposeResult : proposeResultList) {
						log("Member Number: " + proposeResult.getMemberNo());
						log("OpponentMember Number: " + proposeResult.getOpponentMemberNo());
						
						HSPAmigoRelationResultType resultType = proposeResult.getResult();
						if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_SUCCESS)) {
							log("요청 성공");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_ACCEPTED)) {
							log("요청을 이미 수락한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_REQUESTED)) {
							log("이미 요청을 보낸 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_BLOCKED )) {
							log("차단을 한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_MATE )) {
							log("쌍방 요청을 수락하여 MATE가 맺어진 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_NO_MATE  )) {
							log("MATE가 아닌 상태");
						}
					}
				}else {
					log("rejectMate is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}    
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 2. Query method
	public void testQueryAmigoMates(){
		HSPAmigo.queryAmigoMates( 0, 10, new HSPAmigo.HSPQueryMateCB() {
			@Override
			public void onReceive(List<Long> opponentNoList, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryAmigoMates mate is successed");
					log("Query result size is " + opponentNoList.size());
					for (final Long opponentNo : opponentNoList) {
						log("OpponentMember Number: " + opponentNo);
					}
				}else {
					log("queryAmigoMates is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
				
			}
		});
	}    
	public void testQueryAmigoBlockingMembers(){
		HSPAmigo.queryAmigoBlockingMembers( 0, 10, new HSPAmigo.HSPQueryMateCB() {
			@Override
			public void onReceive(List<Long> opponentNoList, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryAmigoBlockingMembers is successed.");
					log("Query result size is " + opponentNoList.size());
					for (final Long opponentNo : opponentNoList) {
						log("OpponentMember Number: " + opponentNo);
					}
				}else {
					log("queryAmigoBlockingMembers is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}    

	public void testQueryFollowingList_Follower(){
		HSPAmigo.queryFollowingList(HSPAmigoFollowingType.HSP_AMIGO_FOLLOWER, 0, 10, new HSPAmigo.HSPQueryMateCB() {
			@Override
			public void onReceive(List<Long> opponentNoList, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryFollowingList is successed");
					log("Query result size is " + opponentNoList.size());
					for (final Long opponentNo : opponentNoList) {
						log("OpponentMember Number: " + opponentNo);
					}
				}else {
					log("queryFollowingList is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}    
	public void testQueryFollowingList_Followee(){
		HSPAmigo.queryFollowingList(HSPAmigoFollowingType.HSP_AMIGO_FOLLOWEE, 0, 10, new HSPAmigo.HSPQueryMateCB() {
			@Override
			public void onReceive(List<Long> opponentNoList, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryFollowingList is successed");
					log("Query result size is " + opponentNoList.size());
					for (final Long opponentNo : opponentNoList) {
						log("OpponentMember Number: " + opponentNo);
					}
				}else {
					log("queryFollowingList is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}    
	public void testQueryFollowingList_MutualFollow(){
		HSPAmigo.queryFollowingList(HSPAmigoFollowingType.HSP_AMIGO_MUTUAL_FOLLOW, 0, 10, new HSPAmigo.HSPQueryMateCB() {
			@Override
			public void onReceive(List<Long> opponentNoList, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryFollowingList is successed");
					log("Query result size is " + opponentNoList.size());
					for (final Long opponentNo : opponentNoList) {
						log("OpponentMember Number: " + opponentNo);
					}
				}else {
					log("queryFollowingList is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}    

	public void testQueryProposedMates_FromMe(){
		HSPAmigo.queryProposedMates(HSPAmigoProposedType.HSP_AMIGO_PROPOSED_FROM_ME, 0, 10, new HSPAmigo.HSPQueryMateCB() {
			@Override
			public void onReceive(List<Long> opponentNoList, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryProposedMates is successed");
					log("Query result size is " + opponentNoList.size());
					for (final Long opponentNo : opponentNoList) {
						log("OpponentMember Number: " + opponentNo);
					}
				}else {
					log("queryProposedMates is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}    
	public void testQueryProposedMates_ToMe(){
		HSPAmigo.queryProposedMates(HSPAmigoProposedType.HSP_AMIGO_PROPOSED_TO_ME, 0, 10, new HSPAmigo.HSPQueryMateCB() {
			@Override
			public void onReceive(List<Long> opponentNoList, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryProposedMates is successed");
					log("Query result size is " + opponentNoList.size());
					for (final Long opponentNo : opponentNoList) {
						log("OpponentMember Number: " + opponentNo);
					}
				}else {
					log("queryProposedMates is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}    

	/*
	 * Only used to query the RelationCode.  
	 */
	public void testQueryRelations(){
		HSPAmigo.queryRelations( getTargetMemberList(), new HSPAmigo.HSPAmigoRelationInfoCB() {
			@Override
			public void onReceive(List<HSPAmigoRelationInfo> relationInfoList,
					HSPResult result) {

				if(result.isSuccess()){
					Log.d(TAG, "queryRelations is successed");
					log("Query result size is " + relationInfoList.size());
					for (final HSPAmigoRelationInfo relationInfo : relationInfoList) {
						log("Group Id : " + relationInfo.getGroupId());
						log("MemberNo : " + relationInfo.getMemberNo());
						log("Opponent: " + relationInfo.getOpponentMemberNo());

						if ( relationInfo.getRelationCode() == HSPAmigoRelationCode.HSP_AMIGO_RELATION_MATE ) {
							log("MATE 인 관계");
						} else if ( relationInfo.getRelationCode() == HSPAmigoRelationCode.HSP_AMIGO_RELATION_MATE_PROPOSED ) {
							log("MATE 요청한 상태");
						} else if ( relationInfo.getRelationCode() == HSPAmigoRelationCode.HSP_AMIGO_RELATION_MATE_PROPOSAL_BLOCK ) {
							log("MATE 요청 차단한 상태");
						} else if ( relationInfo.getRelationCode() == HSPAmigoRelationCode.HSP_AMIGO_RELATION_MATE_BLOCK ) {
							log("차단한 관계");
						} else if ( relationInfo.getRelationCode() == HSPAmigoRelationCode.HSP_AMIGO_RELATION_NONE ) {
							log("아무 관계 아님");
						}
						
						log("RegDate: " + relationInfo.getRegDate());
					}
				}else {
					log("queryRelations is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	} 

	/*
	 * used to query the RelationCode, SocialPoint, ExtraData  
	 */
	public void testQueryRelationsAll(){
		HSPAmigo.queryRelationsAll( getTargetMemberList(), new HSPAmigo.HSPAmigoRelationInfoCB() {
			
			@Override
			public void onReceive(List<HSPAmigoRelationInfo> relationInfoList,
					HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryRelationsAll is successed");
					log("Query result size is " + relationInfoList.size());
					for (final HSPAmigoRelationInfo relationInfo : relationInfoList) {
						log("Group Id : " + relationInfo.getGroupId());
						log("MemberNo : " + relationInfo.getMemberNo());
						log("Opponent: " + relationInfo.getOpponentMemberNo());
						
						if ( relationInfo.getRelationCode() == HSPAmigoRelationCode.HSP_AMIGO_RELATION_MATE ) {
							log("MATE 인 관계");
						} else if ( relationInfo.getRelationCode() == HSPAmigoRelationCode.HSP_AMIGO_RELATION_MATE_PROPOSED ) {
							log("MATE 요청한 상태");
						} else if ( relationInfo.getRelationCode() == HSPAmigoRelationCode.HSP_AMIGO_RELATION_MATE_PROPOSAL_BLOCK ) {
							log("MATE 요청 차단한 상태");
						} else if ( relationInfo.getRelationCode() == HSPAmigoRelationCode.HSP_AMIGO_RELATION_MATE_BLOCK ) {
							log("차단한 관계");
						} else if ( relationInfo.getRelationCode() == HSPAmigoRelationCode.HSP_AMIGO_RELATION_NONE ) {
							log("아무 관계 아님");
						}
						
						log("SocialPoint: " + relationInfo.getSocialPoint());
						log("ExtraData: " + relationInfo.getExtraData());
						log("RegDate: " + relationInfo.getRegDate());
					}
				}else {
					log("queryRelationsAll is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	} 

	public void testQueryAmigoMatesCount(){
		HSPAmigo.queryAmigoMatesCount( new HSPAmigo.HSPQueryCountCB() {
			@Override
			public void onReceive(long count, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryAmigoMatesCount is successed");
					log("Query result size is " + count);

				}else {
					log("queryAmigoMatesCount is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}

			}
		});
	}   
	public void testQueryAmigoBlockingMembersCount(){
		HSPAmigo.queryAmigoBlockingMembersCount( new HSPAmigo.HSPQueryCountCB() {
			@Override
			public void onReceive(long count, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryAmigoBlockingMembersCount is successed");
					log("Query result size is " + count);
					
				}else {
					log("queryAmigoBlockingMembersCount is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
				
			}
		});
	}   

	public void testQueryFollowingCount_Folower(){
		HSPAmigo.queryFollowingCount(HSPAmigoFollowingType.HSP_AMIGO_FOLLOWER, new HSPAmigo.HSPQueryCountCB() {
			@Override
			public void onReceive(long count, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryFollowingCount is successed");
					log("Query result size is " + count);

				}else {
					log("queryFollowingCount is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}

			}
		});
	}   
	public void testQueryFollowingCount_Followee(){
		HSPAmigo.queryFollowingCount(HSPAmigoFollowingType.HSP_AMIGO_FOLLOWEE, new HSPAmigo.HSPQueryCountCB() {
			@Override
			public void onReceive(long count, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryFollowingCount is successed");
					log("Query result size is " + count);
					
				}else {
					log("queryFollowingCount is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
				
			}
		});
	}   
	public void testQueryFollowingCount_MutualFollow(){
		HSPAmigo.queryFollowingCount(HSPAmigoFollowingType.HSP_AMIGO_MUTUAL_FOLLOW, new HSPAmigo.HSPQueryCountCB() {
			@Override
			public void onReceive(long count, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryFollowingCount is successed");
					log("Query result size is " + count);
					
				}else {
					log("queryFollowingCount is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
				
			}
		});
	}   

	public void testQueryProposedMatesCount_FromMe(){
		HSPAmigo.queryProposedMatesCount(HSPAmigoProposedType.HSP_AMIGO_PROPOSED_FROM_ME, new HSPAmigo.HSPQueryCountCB() {
			@Override
			public void onReceive(long count, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryProposedMatesCount is successed");
					log("Query result size is " + count);

				}else {
					log("queryProposedMatesCount is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}

			}
		});
	}   
	public void testQueryProposedMatesCount_ToMe(){
		HSPAmigo.queryProposedMatesCount(HSPAmigoProposedType.HSP_AMIGO_PROPOSED_TO_ME, new HSPAmigo.HSPQueryCountCB() {
			@Override
			public void onReceive(long count, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryProposedMatesCount is successed");
					log("Query result size is " + count);
					
				}else {
					log("queryProposedMatesCount is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
				
			}
		});
	}   

	public void testQueryExtraData(){
		HSPAmigo.queryExtraData(getTargetMemberNo(), new HSPAmigo.HSPQueryExtraDataCB() {
			@Override
			public void onReceive(String extraData, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryExtraData is successed");
					log(getTargetMemberNo() + " ExtraData: " + extraData);

				}else {
					log("queryExtraData is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}

			}
		});
	}   
	public void testQueryMatesLimitCount(){
		//changeMemberNo();
		HSPAmigo.queryMatesLimitCount(new HSPAmigo.HSPQueryMateLimitCountCB() {
			@Override
			public void onReceive(long limitCount, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "queryMatesLimitCount is successed");
					log("queryMatesLimitCount is successed.");
					log("Query result size is " + limitCount);

				}else {
					log("requestSetExtraData is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}   
	public void testQuerySocialPoint(){
		HSPAmigo.querySocialPoint(getTargetMemberNo(), new HSPAmigo.HSPQuerySocialPointCB() {
			@Override
			public void onReceive(long socialPoint, HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "querySocialPoint is successed");
					log("querySocialPoint is successed.");

					log(getTargetMemberNo() + " SocialPoint: " + socialPoint);

				}else {
					log("querySocialPoint is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}

			}
		});
	}   

	///////////////////////////////////////////////////////////////////////////////////////////////
	// 3. Request method

	public void testRequestAutoAddMate(){
		HSPAmigo.requestAutoAddMate(getTargetMemberList(), new HSPAmigo.HSPSetSocialMateCB() {
			@Override
			public void onResult(HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "requestAutoAddMate is successed");
					log("requestAutoAddMate is successed." + "to " + getTargetMemberNo());
				} else {
					log("requestAutoAddMate is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}

			}
		});
	}

	public void testRequestSetFollowers(){
		HSPAmigo.requestSetFollowers(getTargetMemberList(), new HSPAmigo.HSPActionMateCB() {
			@Override
			public void onReceive(List<HSPAmigoRelationResult> proposeResultList,
					HSPResult result) {

				if(result.isSuccess()){
					Log.d(TAG, "requestSetFollowers is successed");
					log("requestSetFollowers is successed.");
					log("Query result size is " + proposeResultList.size());
					for (final HSPAmigoRelationResult proposeResult : proposeResultList) {
						log("Member Number: " + proposeResult.getMemberNo());
						log("OpponentMember Number: " + proposeResult.getOpponentMemberNo());
						
						HSPAmigoRelationResultType resultType = proposeResult.getResult();
						if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_SUCCESS)) {
							log("요청 성공");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_ACCEPTED)) {
							log("요청을 이미 수락한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_REQUESTED)) {
							log("이미 요청을 보낸 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_BLOCKED )) {
							log("차단을 한 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_ALREADY_MATE )) {
							log("쌍방 요청을 수락하여 MATE가 맺어진 상태");
						} else if ( resultType.equals(HSPAmigoRelationResultType.HSP_AMIGO_RELATION_RESULT_NO_MATE  )) {
							log("MATE가 아닌 상태");
						}
					}
				} else {
					log("requestSetFollowers is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}

	public void testRequestSetExtraData(){
		HSPAmigo.requestSetExtraData(getTargetMemberNo(), mExtraTestData, new HSPAmigo.HSPSetSocialMateCB() {

			@Override
			public void onResult(HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "requestSetExtraData is successed");
					Log.d(TAG, "mMemberNo : " + getTargetMemberNo());
					
					log("requestSetExtraData is successed. set " + "'"+mExtraTestData+"'" + " ExtraData to " + getTargetMemberNo());
					
				} else {
					log("requestSetExtraData is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
			}
		});
	}

	public void testRequestSetSocialPoint_Add(){
		HSPAmigo.requestSetSocialPoint(HSPAmigoSocialPointType.HSP_AMIGO_SOCIAL_POINT_ADD, 
				getTargetMemberNo(), mSocialPoint, 
				new HSPAmigo.HSPSetSocialMateCB() {
			@Override
			public void onResult(HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "requestSetSocialPoint is successed. set " + mSocialPoint + " point to " + getTargetMemberNo());
					log("requestSetSocialPoint is successed. set " + mSocialPoint + " point to " + getTargetMemberNo());
				} else {
					log("requestSetExtraData is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}

			}
		});
	}
	
	public void testRequestSetSocialPoint_Update(){
		HSPAmigo.requestSetSocialPoint(HSPAmigoSocialPointType.HSP_AMIGO_SOCIAL_POINT_UPDATE, 
				getTargetMemberNo(), mSocialPointUpdate, 
				new HSPAmigo.HSPSetSocialMateCB() {
			@Override
			public void onResult(HSPResult result) {
				if(result.isSuccess()){
					Log.d(TAG, "requestSetSocialPoint is successed. set " + mSocialPointUpdate + " point to " + getTargetMemberNo());
					log("requestSetSocialPoint is successed. set " + mSocialPointUpdate + " point to " + getTargetMemberNo());
				} else {
					log("requestSetExtraData is failed. " + 
							"resultCode(" + result.getCode() + "), "+
							"resultDetailCode(" + result.getDetailCode()+ "), "+ 
							"resultDetail(" + result.getDetail() );
				}
				
			}
		});
	}

}