#ifndef JSON_JSON_H_INCLUDED
# define JSON_JSON_H_INCLUDED

# include "autolink.h"
# include "value.h"
# include "reader.h"
# include "writer.h"
# include "features.h"

#endif // JSON_JSON_H_INCLUDED



/*
 //json string 을 객체로 변환시켜주는놈
 Json::Reader reader;
 
 //사용할 json객체
 Json::Value root;
 
 //리더로 json string 파싱, root에 집어넣기
 reader.parse("{\"aa\":\"abc\", \"bb\":{\"bb1\":\"bb1value\"},\"cc\":[11,22,33,44]}", root);
 
 //마음대로 사용!!
 CCLog("json1 %s",root["aa"].asCString());
 
 CCLog("json2 %s",root["bb"]["bb1"].asCString());
 
 //배열 출력시, index 설정할때 unsigned int 타입으로
 CCLog("json3 %d",root["cc"][(unsigned int)0].asInt());
 
 //바로 값도 바꿀수있다~
 root["bb"]["bb1"]="bb1value chage";
 
 root["cc"][(unsigned int)0]=99;
 
 //cc 는 숫자만 있는 array이지만 문자열도 바로추가~!
 root["cc"].append("new value apped");
 
 //type check도~!
 if(root["cc"].isArray()) CCLog("rootcc is array");
 
 //읽어올때 안전하게 get메소드를 이용하여 값이 없으면 기본값이 리턴되게 할수도있음.
 CCLog("json4 %s",root.get("dd", "defalut value").asCString());
 
 //배열 요소 출력
 if(root["cc"].isArray()){
 for(unsigned int i=0;i<root["cc"].size();i++){
 if(root["cc"][i].isString()){
 CCLog("cc value.. %s",root["cc"][i].asCString());
 }else if(root["cc"][i].isInt()){
 CCLog("cc value.. %d",root["cc"][i].asInt());
 }
 }
 }
 
 //writer선언 StyledWriter=예쁘게 출력하는놈, FastWriter=그냥 막 출력하는놈
 Json::StyledWriter writer; // or Json::FastWriter writer;
 
 //json string 으로 변환
 std::string jsonstr = writer.write(root);
 CCLog("json5 %s",jsonstr.c_str());
 
 */