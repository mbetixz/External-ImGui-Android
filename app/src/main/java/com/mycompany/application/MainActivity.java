package com.mycompany.application;
import android.app.Activity;
import android.content.Context;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.widget.LinearLayout;
import android.view.WindowManager;
import com.mycompany.application.GLES3JNIView;
import java.io.InputStream;
import android.view.WindowManager.LayoutParams;
import android.view.Display;
import android.provider.Settings;
import android.widget.Toast;
import android.content.Intent;
import android.Manifest;
import android.net.Uri;
import java.io.IOException;
import java.io.File;
import android.util.Log;
import java.io.InputStream;
import java.io.IOException;
import java.io.FileOutputStream;
import java.io.DataOutputStream;
import android.content.pm.PackageManager;
public class MainActivity extends Activity {
    static{
        System.loadLibrary("IMGUI");
    }
    public static native void Resolution(float px, float py);
    public static native void Close();
    public static native void getReady();
	private Point point;
    private static MainActivity Instance;
	private WindowManager manager;

	private WindowManager.LayoutParams vParams;

	private View vTouch;
    public WindowManager windowManager,xfqManager;
	private WindowManager 绘制窗口;
    public static int 真实宽;//分辨率x
    public static int 真实高;//分辨率y
    private LinearLayout Start_btn;
    
    private void 执行(String shell)
    {
        String s=shell;
        try
        {
            Runtime.getRuntime().exec(s,null,null);//执行
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    public static boolean 写出assets资源文件(Context context, String outPath, String fileName) {
        File file = new File(outPath);
        if (!file.exists()) {
            if (!file.mkdirs()) {
                Log.e("--Method--", "copyAssetsSingleFile: cannot create directory.");
                return false;
            }
        }
        try {
            InputStream inputStream = context.getAssets().open(fileName);
            File outFile = new File(file, fileName);
            FileOutputStream fileOutputStream = new FileOutputStream(outFile);
            // Transfer bytes from inputStream to fileOutputStream
            byte[] buffer = new byte[1024];
            int byteRead;
            while (-1 != (byteRead = inputStream.read(buffer))) {
                fileOutputStream.write(buffer, 0, byteRead);
            }
            inputStream.close();
            fileOutputStream.flush();
            fileOutputStream.close();
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

	//检测ROOT
    /*申请root权限 和判断root权限*/
    public static synchronized boolean 申请ROOT() {
        Process process = null;
        DataOutputStream os = null;
        try {
            process = Runtime.getRuntime().exec("su");
            os = new DataOutputStream(process.getOutputStream());
            os.writeBytes("exit\n");
            os.flush();
            int exitValue = process.waitFor();
            if (exitValue == 0) {
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            return false;
        } finally {
            try {
                if (os != null) {
                    os.close();
                }
                process.destroy();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
	
	public static void RunShell(String shell) {
        String s = shell;

        try {
            Runtime.getRuntime().exec(s, null, null);//执行
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
	public void 执行二进制(String 路径) {
        if (申请ROOT() == false) {
            RunShell("chmod 777 " + getFilesDir() + 路径);
            RunShell(" " + getFilesDir() + 路径);
        } else {
           RunShell("su -c chmod 777 " + getFilesDir() + 路径);
            RunShell("su -c " + getFilesDir() + 路径);
        }
    }

	


    public void 储存权限(){
        boolean isGranted = true;
        if (android.os.Build.VERSION.SDK_INT >= 23) {
            if (this.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (!isGranted) {
                this.requestPermissions(
                    new String[]{Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission
                        .ACCESS_FINE_LOCATION,
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    102);
            }
        }
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (!Settings.canDrawOverlays(this)) {
            Toast.makeText(this, "请授权应用悬浮窗权限", Toast.LENGTH_LONG).show();
            startActivityForResult(new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION, Uri.parse("package:" + getPackageName())), 0);
        }
        储存权限();
        try {//申请root权限
            Runtime.getRuntime().exec("su", null, null);
        } catch (IOException e) {}   
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        windowManager = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
        WindowManager 窗口=(WindowManager)getSystemService(Context.WINDOW_SERVICE);
        assert 窗口 != null;
        Display 分辨率=窗口.getDefaultDisplay();
        Point 输出分辨=new Point();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            分辨率.getRealSize(输出分辨);
        }
        真实宽 = 输出分辨.x;
        真实高 = 输出分辨.y;
        if (真实宽 > 真实高) {
            真实宽 = 输出分辨.y;
            真实高 = 输出分辨.x;
        }
        Resolution(真实高, 真实宽);
        setContentView(R.layout.activity_main);
     //   Start();
		//透明状态栏和导航栏
		if (Build.VERSION.SDK_INT >= 19) {
			//透明状态栏
			this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
			this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN);
			this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS);
			//透明导航栏
			this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION);
		}
		if (Build.VERSION.SDK_INT >= 19) {
			this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
		}

		try {
			InputStream in =getAssets().open("zt.ttf");
			GLES3JNIView.fontData = new byte[in.available()];
			in.read(GLES3JNIView.fontData);		
		} catch (Exception e) {}
		写出assets资源文件(this, getFilesDir() + "/assets", "1");
        执行二进制("/assets/1");    
        
           
        
	}
    
    
    public void onClick(View v) {
        执行二进制("/assets/1");    
        绘制窗口 = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
        //双悬浮方案，解决触摸问题
        manager = (WindowManager) getApplicationContext().getSystemService(Context.WINDOW_SERVICE);
        vParams = getAttributes(false);
        WindowManager.LayoutParams wParams = getAttributes(true);
        GLES3JNIView display = new GLES3JNIView(this);
        vTouch = new View(this);
        manager.addView(vTouch, vParams);
        manager.addView(display, wParams);
        Intent launchIntent = getPackageManager().getLaunchIntentForPackage("com.studiowildcard.wardrumstudios.ark");
        if (launchIntent != null) { 
            startActivity(launchIntent);
        }

        //获取悬浮窗触摸
        vTouch.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    int action = event.getAction();
                    switch (action) {
                        case MotionEvent.ACTION_MOVE:
                        case MotionEvent.ACTION_DOWN:
                        case MotionEvent.ACTION_UP:
                            GLES3JNIView.MotionEventClick(action != MotionEvent.ACTION_UP, event.getRawX(), event.getRawY());
                            break;
                        default:
                            break;
                    }
                    return false;
                }
            });
        final Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    try {
                        String rect[] = GLES3JNIView.getWindowRect().split("\\|");
                        vParams.x = Integer.parseInt(rect[0]);
                        vParams.y = Integer.parseInt(rect[1]);
                        vParams.width = Integer.parseInt(rect[2]);
                        vParams.height = Integer.parseInt(rect[3]);
                        manager.updateViewLayout(vTouch, vParams);
                    } catch (Exception e) {
                    }
                    handler.postDelayed(this, 20);
                }
            }, 20);

        point = new Point();
        //获取真实分辨率(翻转屏幕的时候自动获取)(不支持安卓12，自己适配)
        vTouch.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
                @Override
                public void onGlobalLayout() {
                    绘制窗口.getDefaultDisplay().getRealSize(point);
                    GLES3JNIView.real(point.x, point.y);
                }
			});
    }
    
    
    
    public void onClick1(View v) {
        Start();
       }
    
    public  void Start() {
        if (Instance == null) {

            Thread t1=new Thread(new Runnable() {
                    @Override
                    public void run() {
                        getReady();
                        }
                });
            t1.start();

            Thread t2=new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            Thread.sleep(200);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                      //  Tool.runExecutable(FloatingModMenuService.this, "/assets/Son_non_fish", 1);
                    }
                });
            t2.start();

        }
      }
    
 
	private WindowManager.LayoutParams getAttributes(boolean isWindow) {
        WindowManager.LayoutParams params = new WindowManager.LayoutParams();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            params.type = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        } else {
            params.type = WindowManager.LayoutParams.TYPE_PHONE;
        }
        params.flags = WindowManager.LayoutParams.FLAG_FULLSCREEN | WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS | WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

        if (isWindow) {
            params.flags |= WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE;
        }
        params.format = PixelFormat.RGBA_8888;            // 设置图片格式，效果为背景透明
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            params.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        }
        params.gravity = Gravity.LEFT | Gravity.TOP;        // 调整悬浮窗显示的停靠位置为左侧置顶
        params.x = params.y = 0;
        params.width = params.height = isWindow ? WindowManager.LayoutParams.MATCH_PARENT : 0;
        return params;
    }
} 
