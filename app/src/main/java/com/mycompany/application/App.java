package com.mycompany.application;

import android.app.Application;

public class App extends Application {

	private static App sApp;

	@Override
	public void onCreate() {
		super.onCreate();
		sApp = this;
	}

	public static App getApp() {
		return sApp;
	}

}
