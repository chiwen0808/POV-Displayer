package lincp.wendypov;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.Arrays;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.GridLayout;
import android.view.ViewGroup.LayoutParams;

/**
 * Created by GoldenGrandson on 2014/12/12.
 * Implemented by Wendy Huang on 2015/01/11.
 */
public class Customize extends Activity {
    /** Called when the activity is first created. */

    GridLayout gl;
    ImageView[] text;
    int rowCount = 8;
    static int columnCount = 24;
    int totalPix = rowCount*columnCount;
    int item;
    private Button btn_back;
    private Button btn_reset;
    private Button btn_send;
    private Button btn_smile;
    private Button btn_nctu;
    private int[] noDraw = {};
    private byte[] pixVal = new byte[columnCount];
    
  
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.customize_layout);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        btn_back = (Button)findViewById(R.id.btn_back);
        btn_reset = (Button)findViewById(R.id.btn_reset);
        btn_send = (Button)findViewById(R.id.btn_send);
        btn_smile = (Button)findViewById(R.id.btn_smile);
        btn_nctu = (Button)findViewById(R.id.btn_nctu);
        gl = (GridLayout)findViewById(R.id.gird_customize);
        Arrays.fill(pixVal, (byte) 0);

        text = new ImageView[totalPix];


        btn_back.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
            // TODO Auto-generated method stub
                Intent intent = new Intent();
                intent.setClass(Customize.this, MainActivity.class);
                startActivity(intent);
                Customize.this.finish();
            }
        });


        btn_reset.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
            	for(int i=0;i<columnCount;++i)
            	{
            		pixVal[i] = 0x00;
            	}
                for(item=0;item<totalPix;item++)
                {
                	text[item].setImageResource(R.drawable.led_off);
                }
            }
        });
        
        btn_send.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
            	Log.d("Wendy debug: line1", String.valueOf(pixVal[0]));
            	Log.d("Wendy debug: line2", String.valueOf(pixVal[1]));
            	Log.d("Wendy debug: line3", String.valueOf(pixVal[2]));
            	Log.d("Wendy debug: line4", String.valueOf(pixVal[3]));
            	Log.d("Wendy debug: line5", String.valueOf(pixVal[4]));
            	
     
            	
            	Intent resultIntent=new Intent();
            	Bundle b=new Bundle();
            	b.putByteArray("B", pixVal);
            	resultIntent.putExtras(b);
            	setResult(RESULT_OK,resultIntent);
            	finish();        	

            }
        });
        
        btn_smile.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
            	//f80442e945050545
            	//e94204f800000000
            	//0000000000000000
            	//000030787c3e1f3e

            	//byte smile_pic[] = new byte[]{
            	//		(byte)0xf8, (byte)0x04, (byte)0x42, (byte)0xe9, (byte)0x45, (byte)0x05, (byte)0x05, (byte)0x45,
            	//		(byte)0xe9, (byte)0x42, (byte)0x04, (byte)0xf8, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00,
            	//		(byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00
            	//};
            	//byte smile_pic[] = new byte[]{
            	//		(byte)0xf8, (byte)0x04, (byte)0x42, (byte)0xe9, (byte)0x45, (byte)0x05, (byte)0x05, (byte)0x45,
            	//		(byte)0xe9, (byte)0x42, (byte)0x04, (byte)0xf8, (byte)0x00, (byte)0x00, (byte)0x30, (byte)0x78,
            	//		(byte)0x7c, (byte)0x3e, (byte)0x1f, (byte)0x3e, (byte)0x7c, (byte)0x78, (byte)0x30, (byte)0x00
            	//};
            	//byte smile_pic[] = new byte[]{
            	//		(byte)0x00, (byte)0x44, (byte)0x28, (byte)0x12, (byte)0x01, (byte)0x01, (byte)0x12, (byte)0x28,
            	//		(byte)0x44, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x30, (byte)0x78,
            	//		(byte)0x7c, (byte)0x3e, (byte)0x1f, (byte)0x3e, (byte)0x7c, (byte)0x78, (byte)0x30, (byte)0x00
            	//};
            	byte smile_pic[] = new byte[]{
            			(byte)0x00, (byte)0x44, (byte)0x28, (byte)0x12, (byte)0x01, (byte)0x01, (byte)0x12, (byte)0x28,
            			(byte)0x44, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x30, (byte)0x78,
            			(byte)0x7c, (byte)0x3e, (byte)0x1f, (byte)0x3e, (byte)0x7c, (byte)0x78, (byte)0x30, (byte)0x00
            	};
                for(int i=0;i<columnCount;i++)
                {
                	for(int j=0;j<rowCount;j++)
                	{
                		int k=((byte)smile_pic[i]) & ((byte)1 << (7-j));
                		if(k!=0)
                			text[columnCount*j+i].setImageResource(R.drawable.led_on);
                			
                	
                	}
                	pixVal[i]=smile_pic[i];
                	
                }
            }
        });
        
        btn_nctu.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
            	byte nctu_pic[] = new byte[]{
            			(byte)0x00, (byte)0x00, (byte)0x00, (byte)0x28, (byte)0x29, (byte)0xb5, (byte)0x62, (byte)0x35,
            			(byte)0x29, (byte)0x28, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x09, (byte)0x0a, (byte)0x0c,
            			(byte)0x78, (byte)0x0c, (byte)0x0a, (byte)0x09, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00
            	};
                for(int i=0;i<columnCount;i++)
                {
                	for(int j=0;j<rowCount;j++)
                	{
                		int k=((byte)nctu_pic[i]) & ((byte)1 << (7-j));
                		if(k!=0)
                			text[columnCount*j+i].setImageResource(R.drawable.led_on);
                			
                	
                	}
                	pixVal[i]=nctu_pic[i];
                	
                }
            }
        });
        

        for(int i=0;i<totalPix;i++)
        {

            text[i] = new ImageView(Customize.this);
            text[i].setLayoutParams(new LayoutParams
                    (LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
            text[i].setImageResource(R.drawable.led_off);

            gl.addView(text[i]);
        }

        boolean flag = false;
        for(item=0;item<totalPix;item++)
        {
            text[item].setOnClickListener(new View.OnClickListener() {

                int pos = item;
                int count = 0;
                int quat = item/columnCount;
                int rmd = item%columnCount;
                int power = 7-quat;

                public void onClick(View v) {
                    // TODO Auto-generated method stub
                    if (count == 0) {
                        text[pos].setImageResource(R.drawable.led_on);
                        //OR
                        byte mask = (byte) (0x01 << power);
                        //pixVal[rmd] += 0x01<<quat;
                        pixVal[rmd] = (byte) (pixVal[rmd]|mask);
                        Log.d("Wendy debug: ", String.valueOf(pos)+" On");
                        count++;
                    } else if (count == 1) {
                        text[pos].setImageResource(R.drawable.led_off);
                        //AND
                        byte mask = (byte) (0x01 << power);
                        mask = (byte) ~mask;
                        pixVal[rmd] = (byte) (pixVal[rmd]&mask);
                        Log.d("Wendy debug: ", String.valueOf(pos)+" Off");
                        count--;
                    }
                }
            });
        }


    }
}
