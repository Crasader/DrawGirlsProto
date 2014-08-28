<?php

class TimeCheck
{
	static public $lastTime=0;
	static public $cnt=0;
	static public $log=[];

	static public function get_time() {
	    list($usec, $sec) = explode(" ", microtime());
	    return ((float)$usec + (float)$sec);
	}
	static public function start(){
		self::$lastTime = self::get_time();
	}
	static public function check($print){
		self::$cnt++;
		$oldtime = self::$lastTime;
		self::$lastTime = self::get_time();
		$time = self::$lastTime-$oldtime;

		self::$log[]=array("tag"=>$print,"cnt"=>self::$cnt,"time"=>$time);
	}
	static public function printLog(){
		foreach (self::$log as $key => $value) {
			echo $value["tag"]."/".$value["cnt"]."/".$value["time"]."<br>";
		}
	}
}

TimeCheck::start();
// include "../command/cmd2.php";
// TimeCheck::check("cmd2 include");
include "../DBManagerLib.php";
TimeCheck::check("DBManagerLib include");
// include "../DBManagerLib2.php";
// TimeCheck::check("DBManagerLib2 include");

TimeCheck::printLog();

?>