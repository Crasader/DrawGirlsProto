package com.litqoo.lib;

public abstract class KRunnable implements Runnable {
	protected final String totalSource;
	protected final int delekey;

	public KRunnable(final int key, String str) {
		totalSource = str;
		delekey = key;
	}
}
