package com.litqoo.lib;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Calendar;

import android.content.Context;
import android.os.Environment;

public class SnowFilelog 
{
	private static String m_strLogFileFolderPath = "";
	private static String m_strLogFileName = "filelog.txt";
	
	public static void initialize(Context context)
	{
		// SD ī�尡 ������ SD ī�� ���, ���� ���� �׳� Root
		boolean bSDCardExist = Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED);
		if (bSDCardExist == true)
		{
			m_strLogFileFolderPath = Environment.getExternalStorageDirectory().getAbsolutePath();
		}
		else
		{
			m_strLogFileFolderPath = "";
		}
	}
	
	public static void setFileName(String strFileName)
	{
		m_strLogFileName = strFileName;
	}
	
	public static void reset()
	{
		File file = new File(m_strLogFileFolderPath + "/" + m_strLogFileName);
		file.delete();
		
		//write("SnowFileLog.reset()");
	}
	
	public static void write(String strMessage)
	{
		String _strMessage = strMessage;
		_strMessage = _strMessage + "\n";
		if ( (strMessage == null) || (strMessage.length() == 0) )
			return;
		
		
		_strMessage = getCurrentTime() + " " + _strMessage + "\n";
		
		String storage = Environment.getExternalStorageDirectory().getAbsolutePath(); //"/data/"; // _thiz.getFilesDir().toString() + "/";
		String file = storage + "/mylog.txt";
		FileOutputStream fos = null;
		try {
			fos = new FileOutputStream(file, true);
			//fos = _thiz.openFileOutput(file, Context.MODE_APPEND);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			return;
			//e.printStackTrace();
			
		}
		try {
			fos.write(_strMessage.getBytes());
			fos.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			//e.printStackTrace();
		}
		
	}
//	public static void write(Context _thiz, String strMessage)
//	{
//		String _strMessage = strMessage;
//		if ( (strMessage == null) || (strMessage.length() == 0) )
//			return;
//		
//		
//		_strMessage = getCurrentTime() + " " + _strMessage + "\n";
//		
//		String storage = _thiz.getFilesDir().toString() + "/";
//		String file = storage + "mylog.txt";
//		FileOutputStream fos = null;
//		try {
//			fos = new FileOutputStream(file, true);
//			//fos = _thiz.openFileOutput(file, Context.MODE_APPEND);
//		} catch (FileNotFoundException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
//		try {
//			fos.write(_strMessage.getBytes());
//			fos.close();
//		} catch (IOException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
//		
//	}
	private static String getCurrentTime()
	{
		Calendar calendar = Calendar.getInstance();
		String strTime = String.format("%d-%d-%d %d:%d:%d", calendar.get(Calendar.YEAR), 
															calendar.get(Calendar.MONTH) + 1,
															calendar.get(Calendar.DAY_OF_MONTH),
															calendar.get(Calendar.HOUR_OF_DAY),
															calendar.get(Calendar.MINUTE),
															calendar.get(Calendar.SECOND));
		return strTime;
	}
}
