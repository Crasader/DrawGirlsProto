package com.nhnent.SKSUMRAN.NA;

import java.util.ArrayList;
import java.util.List;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.util.Log;
import android.view.InputDevice;
import android.view.InputEvent;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.widget.Toast;

import com.litqoo.lib.KRunnable;
import com.litqoo.lib.hspConnector;

class Dpad {
	final static int UP       = 0;
	final static int LEFT     = 1;
	final static int RIGHT    = 2;
	final static int DOWN     = 3;
	final static int CENTER   = 4;

	int directionPressed = -1; // initialized to -1

	public int getDirectionPressed(InputEvent event) {
		if (!isDpadDevice(event)) {
			return -1;
		}

		// If the input event is a MotionEvent, check its hat axis values.
		if (event instanceof MotionEvent) {

			// Use the hat axis value to find the D-pad direction
			MotionEvent motionEvent = (MotionEvent) event;
			float xaxis = motionEvent.getAxisValue(MotionEvent.AXIS_HAT_X);
			float yaxis = motionEvent.getAxisValue(MotionEvent.AXIS_HAT_Y);

			// Check if the AXIS_HAT_X value is -1 or 1, and set the D-pad
			// LEFT and RIGHT direction accordingly.
			if (Float.compare(xaxis, -1.0f) == 0) {
				directionPressed =  Dpad.LEFT;
			} else if (Float.compare(xaxis, 1.0f) == 0) {
				directionPressed =  Dpad.RIGHT;
			}
			// Check if the AXIS_HAT_Y value is -1 or 1, and set the D-pad
			// UP and DOWN direction accordingly.
			else if (Float.compare(yaxis, -1.0f) == 0) {
				directionPressed =  Dpad.UP;
			} else if (Float.compare(yaxis, 1.0f) == 0) {
				directionPressed =  Dpad.DOWN;
			}
		}

		// If the input event is a KeyEvent, check its key code.
		else if (event instanceof KeyEvent) {

			// Use the key code to find the D-pad direction.
			KeyEvent keyEvent = (KeyEvent) event;
			if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_LEFT) {
				directionPressed = Dpad.LEFT;
			} else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_RIGHT) {
				directionPressed = Dpad.RIGHT;
			} else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_UP) {
				directionPressed = Dpad.UP;
			} else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_DOWN) {
				directionPressed = Dpad.DOWN;
			} else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_CENTER) {
				directionPressed = Dpad.CENTER;
			}
		}
		return directionPressed;
	}

	public static boolean isDpadDevice(InputEvent event) {
		// Check that input comes from a device with directional pads.
		if ((event.getSource() & InputDevice.SOURCE_DPAD)
				!= InputDevice.SOURCE_DPAD) {
			return true;
		} else {
			return false;
		}
	}
}


public class LuaGLSurfaceView extends Cocos2dxGLSurfaceView{

	public LuaGLSurfaceView(Context context){
		super(context);
	}

	int downCount = 0;
	int upCount = 0;
	float directionX = 0.0f;
	float directionY = 0.0f;
	List<Integer> mCallbackKeyList = new ArrayList<Integer>();
	public void setCallbackKey(int key)
	{
		mCallbackKeyList.add(key);
		//		mCallbackKey = key; 
	}
	public void removeCallbackKey(int registeredKey)
	{
		if(mCallbackKeyList.contains(registeredKey))
        { 
			mCallbackKeyList.remove(new Integer(registeredKey));
		}
	}
	Dpad mDpad = new Dpad();
	private static boolean isFireKey(int keyCode) {
		// Here we treat Button_A and DPAD_CENTER as the primary action
		// keys for the game.
		return keyCode == KeyEvent.KEYCODE_DPAD_CENTER
				|| keyCode == KeyEvent.KEYCODE_BUTTON_A;
	}	
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		boolean handled = false;
		if ((event.getSource() & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD) {
			if (event.getRepeatCount() == 0) {
				// keyCode 다운을 보내줌.
				JSONObject r = new JSONObject();

				try {
					r.put("type", "keyup");
					r.put("keycode", event.getKeyCode());
				} catch (JSONException e) {

				}
				for(Integer keys : mCallbackKeyList)
				{
					queueEvent(new KRunnable(keys.intValue(), r.toString()) {
						public void run() {
							hspConnector.SendReaction(this.delekey,
									this.totalSource);
						}
					});						
				}
				//				switch (keyCode) {
				//				default:
				//					if (isFireKey(keyCode)) {
				//						handled = true;
				//					}
				//					break;
				//				}
			}
			if (handled) {
				return true;
			}
		}
		return super.onKeyUp(keyCode, event);
	}
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		boolean handled = false;
		if ((event.getSource() & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD) {
			if (event.getRepeatCount() == 0) {
				// keyCode 다운을 보내줌.
				JSONObject r = new JSONObject();

				try {
					r.put("type", "keydown");
					r.put("keycode", event.getKeyCode());
				} catch (JSONException e) {

				}
				for(Integer keys : mCallbackKeyList)
				{
					queueEvent(new KRunnable(keys.intValue(), r.toString()) {
						public void run() {
							hspConnector.SendReaction(this.delekey,
									this.totalSource);
						}
					});						
				}
				//				switch (keyCode) {
				//				default:
				//					if (isFireKey(keyCode)) {
				//						handled = true;
				//					}
				//					break;
				//				}
			}
			if (handled) {
				return true;
			}
		}

		//		Toast.makeText(this.getContext(),
		//				"DOWN" + String.valueOf(downCount) + " : " + String.valueOf(keyCode), Toast.LENGTH_LONG)
		//				.show();	
		downCount++;
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public boolean onGenericMotionEvent(MotionEvent event) {


		if ((event.getSource() & InputDevice.SOURCE_JOYSTICK)
				== InputDevice.SOURCE_JOYSTICK) {


			if (event instanceof MotionEvent) {

				// Use the hat axis value to find the D-pad direction
				MotionEvent motionEvent = (MotionEvent) event;
				float xaxis = motionEvent.getX();
				float yaxis = motionEvent.getY();
				int xaxis2 = (int) Math.rint(xaxis);
				int yaxis2 = (int) Math.rint(yaxis);

				//				Toast.makeText(this.getContext(),
				//						xaxis + ".." + yaxis, Toast.LENGTH_LONG)
				//						.show();	
				if(Float.compare(xaxis2, directionX) !=0 || Float.compare(yaxis2, directionY) != 0)
				{
					directionX = xaxis2;
					directionY = yaxis2;
					JSONObject r = new JSONObject();

					try {
						r.put("type", "motion");
						r.put("x", directionX);
						r.put("y", directionY);
					} catch (JSONException e) {

					}
					for(Integer keys : mCallbackKeyList)
					{
						queueEvent(new KRunnable(keys.intValue(), r.toString()) {
							public void run() {
								hspConnector.SendReaction(this.delekey,
										this.totalSource);
							}
						});						
					} 
					//					hspConnector.send
					//					Toast.makeText(this.getContext(),
					//					xaxis2 + ".." + yaxis2, Toast.LENGTH_LONG)
					//					.show();		
					//					Toast.makeText(this.getContext(),
					//					event.toString(), Toast.LENGTH_LONG)
					//					.show();		
				}


			}
			//			return true;
		} else {
			//			return false;
		}

		// TODO Auto-generated method stub
		return super.onGenericMotionEvent(event);
	}




}






