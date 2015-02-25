package lincp.wendypov;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

import lincp.wendypov.R;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import java.text.SimpleDateFormat;

public class MainActivity extends Activity {

	// TODO: adjust ARDUINO_IP_ADDRESS
	public final String ARDUINO_IP_ADDRESS = "192.168.240.1";
	public final String TAG = "lincp";
	
	private ArrayBlockingQueue<String> mQueue = new ArrayBlockingQueue<String>(100);
	private AtomicBoolean mStop = new AtomicBoolean(false);
	private static Thread sNetworkThreadSend = null;
	
	private Button btnTextSend;
	//private Button btnRe;
	//private Button btnMi;
	//private Button btnFa;
	//private Button btnSo;
	//private Button btnLa;
	private Button btnDraw;
	private Button btnClock;
	private Button btnDelayControl;
	private EditText textToShow;
	
	final int wrequestCode =100;//w++
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		
		btnTextSend = (Button) findViewById(R.id.button1);
		//btnRe = (Button) findViewById(R.id.button2);
		//btnMi = (Button) findViewById(R.id.button3);
		//btnFa = (Button) findViewById(R.id.button4);
		//btnSo = (Button) findViewById(R.id.button5);
		//btnLa = (Button) findViewById(R.id.button6);
		btnDraw = (Button) findViewById(R.id.button7);
		btnClock = (Button) findViewById(R.id.button8);
		btnDelayControl = (Button) findViewById(R.id.button9);
		textToShow = (EditText) findViewById(R.id.editText1);
		
		btnTextSend.setOnClickListener(new Button.OnClickListener() {
			@Override
			public void onClick(View arg0) {
				mQueue.offer("TEXT/"+textToShow.getText().toString());
			}
		});
        btnDraw.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View arg0) {
                //mQueue.offer(4);
                Intent intent = new Intent();
                intent.setClass(MainActivity.this, Customize.class);
                //startActivity(intent);
                //MainActivity.this.finish();
               
                startActivityForResult(intent,wrequestCode);
            }
        });
        btnClock.setOnClickListener(new Button.OnClickListener() {
			@Override
			public void onClick(View arg0) {
				SimpleDateFormat sDateFormat = new SimpleDateFormat("HHmmssddMMyyyy");
				String date = sDateFormat.format(new java.util.Date());  
				Log.d("Wendy debug: date",date);
				
				mQueue.offer("CLOCK/"+date);
			}
		});
        btnDelayControl.setOnClickListener(new Button.OnClickListener() {
			@Override
			public void onClick(View arg0) {
				mQueue.offer("DELAY/"+textToShow.getText().toString());
			}
		});
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	           super.onActivityResult(requestCode, resultCode, data);

	           switch(requestCode){
	           case wrequestCode:
	        	          
	        	          log("returning from value sending");
	        	          //Log.d("Wendy debug: ", data.getExtras().getString("B", ""));
	        	          byte recv[] = data.getByteArrayExtra("B");
	        	          Log.d("Wendy debug: recv",String.valueOf(recv[0]));
	        	          Log.d("Wendy debug: recv",String.valueOf(recv[1]));
	        	          Log.d("Wendy debug: recv",String.valueOf(recv[2]));
	        	          Log.d("Wendy debug: recv",String.valueOf(recv[3]));
	        	          Log.d("Wendy debug: recv",String.valueOf(recv[4]));
	        	          String str = "";
	        	          
	        	          //String.format("%02x", recv[0]);
	        	          for (int i=0;i<Customize.columnCount;i++)
	        	          {
	        	        	  str+=String.format("%02x", recv[i]);
	        	        	  
	        	          }
	        	          //Log.d("Wendy debug: recv",String.format("%02x", recv[0]));
	        	          Log.d("Wendy debug: recv",str);
	        	          mQueue.offer("DRAW/"+str);
	          }
	}
	
	
	
	@Override
	protected void onStart() {
		mStop.set(false);
		if(sNetworkThreadSend == null){
			sNetworkThreadSend = new Thread(mNetworkRunnableSend);
			sNetworkThreadSend.start();
		}
		super.onStart();
	}

	@Override
	protected void onStop() {
		mStop.set(true);
		mQueue.clear();
		mQueue.offer("");
		if(sNetworkThreadSend != null) sNetworkThreadSend.interrupt();
		super.onStop();
	}

	private final Runnable mNetworkRunnableSend = new Runnable() {
		@Override
		public void run() {
			log("starting network thread for sending");
			String urlBase = "http://"+ARDUINO_IP_ADDRESS+"/arduino/";
			String url;

			try {
				while(!mStop.get()){
					String val = mQueue.take();
					if(val.length() > 0){
						HttpClient httpClient = new DefaultHttpClient();
						url = urlBase.concat(String.valueOf(val));
						Log.d("Wendy Debug:", url);
						log("executing httpClient request");
						try {
							URI urlToSend = new URI("http://", ARDUINO_IP_ADDRESS+"/arduino/"+val, null);
							Log.d("Wendy Debug:", urlToSend.getRawPath());
							HttpResponse response = httpClient.execute(new HttpGet("http://"+ARDUINO_IP_ADDRESS+urlToSend.getRawPath()));
						} catch (URISyntaxException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
						//HttpResponse response = httpClient.execute(new HttpGet(url));
					}
				}
			} catch (ClientProtocolException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}

			log("returning from network thread for sending");
			sNetworkThreadSend = null;
		}
	};
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	public void log(String s){
		Log.d(">==< "+TAG+" >==<", s);
	}

}
