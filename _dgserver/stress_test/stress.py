# -*- coding: utf-8 -*-
from Crypto.Cipher import AES
import base64

import urllib
import sys
import threading
import random
import json
import datetime
import time
#obj = DES.new("12345678", DES.MODE_ECB)

#encrypted = obj.encrypt("Please  ")
#print base64.b64encode(encrypted)

#ret = base64.b64decode("YVcIYD8l/yg=")
#print "%r" % obj.decrypt(ret)
exeCount = 0
failCount = 0
serverUrl = ""
threads = 0
BS = 16
pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS)
unpad = lambda s : s[0:-ord(s[-1])]

key = "qrqhyrlgprghedvh"

def encodeCmd(codestr):
  global key
  #iv = Random.new().read(AES.block_size)
  cipher = AES.new(key,AES.MODE_ECB)
  fillCount = 16 - len(codestr) % 16;
  for i in range(0, fillCount):
		codestr = codestr + " ";
  encrypted = cipher.encrypt(codestr)
  paramStr = base64.b64encode(encrypted)
  return paramStr

def decodeCmd(codestr):
  global key
  #iv = Random.new().read(AES.block_size)
  cipher = AES.new(key,AES.MODE_ECB)
  rstring = base64.b64decode(codestr)
  rstring = cipher.decrypt(rstring)
  return rstring

def urlreq(sUrl,jsonstring):
	
	paramStr = encodeCmd(jsonstring)
	# print "=====================start==================="
	# print jsonstring
	# print "to"
	# print paramStr
	# print "=====================end==================="
	#for temp in range(0, 10) :
	try:
		global exeCount
		global failCount
		d = urllib.urlencode({"gid":"drawgirls_tstore","version":2,"command":paramStr})
		f = urllib.urlopen("http://"+sUrl+"/command.php",d);
		resultstr = f.read()
		if resultstr[len(resultstr) - 1] != '#':
			failCount += 1
		
		exeCount += 1
		f.close()
		
		rstring = decodeCmd(resultstr[:-1])
		#print rstring
		
		return rstring
	except IOError, e:
		print e

def stress1(thNo,cycle):
	global exeCount
	global failCount
	global serverUrl
	global threads
	
	count = 1
	memNo = (thNo)+1000000
	nick = "dum"+str(thNo);
	allexec = threads*cycle
	while True:
		if cycle < count:
			break
		
		startDatetime = datetime.datetime.now()

		#join
		senddata1 = {"0":{"a":"join","p":{"memberID":memNo,"nick":nick}},"memberID": memNo,"dontcheck":True}
		jsonString = json.dumps(senddata1)
		urlreq(serverUrl,jsonString)
		
		#login
		senddata1 = {"0":{"a":"login","p":{"memberID":memNo}},"memberID": memNo,"dontcheck":True}
		jsonString = json.dumps(senddata1)
		t = urlreq(serverUrl,jsonString)
        #print t
        #mm = mm.replace("\\","")
		#mm = mm.replace("'","")
		#print mm
		#jsonResult = json.loads(mm);
		
		#start
		senddata1 = {"0":{"a":"getcommonsetting","p":{}},"1":{"a":"getarchivementlist","p":{"version":276}},"10":{"a":"getarchivementhistory","p":{"memberID":memNo}},"11":{"a":"getCardHistory","p":{"memberID":memNo}},"12":{"a":"getPuzzleHistory","p":{"memberID":memNo}},"13":{"a":"getPieceHistory","p":{"memberID":memNo}},"14":{"a":"getcharacterhistory","p":{"memberID":memNo}},"15":{"a":"gettodaymission","p":{"memberID":memNo}},"16":{"a":"getuserproperties","p":{"memberID":memNo}},"17":{"a":"getUserData","p":{"memberID":memNo}},"18":{"a":"getheart","p":{"memberID":memNo}},"19":{"a":"gettimeinfo","p":{}},"2":{"a":"getshoplist","p":{"version":733}},"3":{"a":"getpuzzlelist","p":{"version":6026}},"4":{"a":"getcharacterlist","p":{"version":175}},"5":{"a":"getmonsterlist","p":{"version":517}},"6":{"a":"getnoticelist","p":{}},"7":{"a":"gettimeevent","p":{}},"8":{"a":"checkattendenceevent","p":{"memberID":memNo}},"9":{"a":"checkweeklyreward","p":{"memberID":memNo}},"memberID":memNo,"dontcheck":True}
		jsonString = json.dumps(senddata1)
		urlreq(serverUrl,jsonString)
		
		#getpuzzleinfo
		puzzleNo = random.randrange(1,5)
		senddata1 = {"0":{"a":"getpuzzleinfo","p":{"order":puzzleNo}},"memberID":memNo,"dontcheck":True}
		jsonString = json.dumps(senddata1)
		urlreq(serverUrl,jsonString)
		
		#useheart
		senddata1 = {"1":{"a":"updatePieceHistory","p":{"clearCount":1,"clearDateList":[True,False,False,False],"memberID":memNo,"openDate":True,"openType":"무료(첫스테이지)","pieceNo":1,"tryCount":2}},"2":{"a":"getheart","p":{"memberID":memNo,"use":True}},"3":{"a":"checkLoginEvent","p":{"memberID":memNo}},"4":{"a":"gettimeevent","p":{}},"dontcheck":True}
		jsonString = json.dumps(senddata1)
		urlreq(serverUrl,jsonString)
		
		#getcard
		cardNo = random.randrange(1,25)
		senddata1 = {"0":{"a":"starttransaction","p":{"memberID":memNo}},"1":{"a":"updateCardHistory","p":{"addCount":1,"cardNo":cardNo,"memberID":memNo}},"memberID":memNo,"dontcheck":True}
		jsonString = json.dumps(senddata1)
		urlreq(serverUrl,jsonString)
		
		#ok
		senddata1 = {"0":{"a":"starttransaction","p":{"memberID":memNo}},"1":{"a":"exchange","p":{"exchangeID":"clearHeartUp","memberID":memNo}},"2":{"a":"exchange","p":{"exchangeID":"stageGold","list":[{"count":195,"statsID":"stage","statsValue":"1","type":"g"}],"memberID":memNo}},"3":{"a":"addweeklyscore","p":{"data":"{\"allhighscore\":196872.0,\"flag\":\"kr\",\"highstage\":-1,\"nick\":\"리트쿠우\",\"selectedcard\":0}\n","memberID":memNo,"score":196872}},"4":{"a":"getstagerankbyalluser","p":{"data":"{\"allhighscore\":196872.0,\"flag\":\"kr\",\"highstage\":-1,\"nick\":\"리트쿠우\",\"selectedcard\":0}\n","memberID":memNo,"myScore":196872,"stageNo":1}},"5":{"a":"gettimeinfo","p":{}},"6":{"a":"updatearchivementhistory","p":{"archiveID":49,"count":2,"memberID":memNo}},"7":{"a":"updateuserdata","p":{"aNoFail":8,"memberID":memNo}},"8":{"a":"updatetodaymission","p":{"count":196872,"date":20140822,"memberID":memNo}},"memberID":memNo,"dontcheck":True}
		jsonString = json.dumps(senddata1)
		
		urlreq(serverUrl,jsonString)
		
		endDatetime = datetime.datetime.now()

		deltaDatetime = endDatetime - startDatetime

		millisec = int(deltaDatetime.total_seconds() * 1000)
		
		#print t
		print "%d msec / cycle : %d / memberNo : %d  ...  %d / %d  ...  failCount : %d" % (millisec, count, memNo ,exeCount, allexec*7 ,failCount) #jsonResult["result"]["code"]
		count = count + 1

	print "stop thread %d"%(thNo)
#result = urlreq(random.choice(jsonString))

def main():
	#maxOpenNumbers = resource.getrlimit(resource.RLIMIT_NOFILE)[0] # 열 수 있는 파일 개수.
	global serverUrl
	
	print "select Server"
	print "1. 182.162.201.147:10010 (Dev)";
	print "2. 182.162.196.182:10080 (Alpha)";
	print "3. 222.122.203.7:10080 (Real)";
	
	serverUrl = raw_input(">")
	
	if serverUrl == "1":
		serverUrl = "182.162.201.147:10010"
	elif serverUrl == "2":
		serverUrl = "182.162.196.182:10080"
	elif serverUrl == "2":
		serverUrl = "222.122.203.7:10080"
	
	
	# print "select test set"
	# print "1. default set";
	# print "2. none";
	# print "3. none";
	# print "4. none";
	
	# a = input(">");
	
	a=1
	
	if a == 1:
		global exeCount
		global failCount
		global threads
		print "유저수? (thread)"
		threads = input(">")
		print "유저당 몇회 호출? (cycle)"
		cycle = input(">")
		exeCount = 0
		failCount = 0
		
		print "connect to "+serverUrl
		for temp in range(1,  threads+1):
			time.sleep(.200)
			print "create thread %d"%(temp)
			th = threading.Thread(target = stress1, args=(temp,cycle,))
			th.start()
		pass
	elif a == 2:
		pass
	elif a == 3:
		pass
	elif a == 4:
		pass
if __name__ == '__main__':
	main()
	sys.exit(0)



