package com.game.lab2048;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLEncoder;
import java.util.ArrayList;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.StatusLine;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpResponseException;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.util.Xml;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class UploadMark extends Activity
{
	private static final String TAG = "UploadMark";
	private String id=null;
	private String score=null;
	private ArrayList<data> da=new ArrayList<data>();
	private ListView list=null;
	private BaseAdapter adapter=null;
	
	private final class MyAdapter extends BaseAdapter 
	{
		@Override
		public int getCount() 
		{
			return da.size();
		}

		@Override
		public long getItemId(int position) 
		{
			return position;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent)
		{
			TextView tv=new TextView(UploadMark.this);
			tv.setText(da.get(position).id+":"+da.get(position).score);
			return tv;
		}

		@Override
		public Object getItem(int position)
		{
			// TODO Auto-generated method stub
			return null;
		}
	}

	class data
	{
		private String id;
		private String score;
		public String getId()
		{
			return id;
		}
		
		public void setId(String id) 
		{
			this.id = id;
		}
		
		public String getScore() 
		{
			return score;
		}
		
		public void setScore(String score) 
		{
			this.score = score;
		}
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.query);
		
		list=(ListView)findViewById(R.id.list);
		adapter=new MyAdapter();
		list.setAdapter(adapter);
		
		Intent intent=getIntent();
		id=intent.getStringExtra("id");
		score=intent.getIntExtra("score",0)+"";
	
		final Handler handler=new Handler()
		{
			@Override
			public void handleMessage(Message msg)
			{
				Toast.makeText(UploadMark.this, "上传成功", Toast.LENGTH_LONG).show();
				adapter.notifyDataSetChanged();
			}
		};
		

		new Thread()
		{
			@Override
			public void run()
			{
				try 
				{
					getData(handler);
				} 
				catch (XmlPullParserException e) 
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
				}		
			}
		}.start();
	}

	private void getData(final Handler handler) throws XmlPullParserException
	{
		try
		{
			id=URLEncoder.encode(new String(id.getBytes("UTF-8")),"UTF-8");
			score=URLEncoder.encode(new String(score.getBytes("UTF-8")),"UTF-8");
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
		String url="http://114.212.227.91:8080/game.jsp?id="+id+"&score="+score;
		
		
		try
		{
			URLConnection conn=new URL(url).openConnection();
			InputStream instream = conn.getInputStream();
			data curdata=null;
			
			XmlPullParser parser=Xml.newPullParser();
			parser.setInput(instream,"utf-8");
			int type=parser.getEventType();
			while(type != XmlPullParser.END_DOCUMENT)
			{
				if(type == XmlPullParser.START_TAG)
				{
					String name=parser.getName();
					if("item".equals(name))
					{
						curdata=new data();
					}
					else if("id".equals(name))
					{
						curdata.setId(parser.nextText());
					}
					else if("score".equals(name))
					{
						curdata.setScore(parser.nextText());
					}
				}
				else if(type == XmlPullParser.END_TAG)
				{
					String name=parser.getName();
					if("item".equals(name))
					{
						da.add(curdata);
					}
				}	
				type=parser.next();
			}
			instream.close();
			if(handler != null)
				handler.sendEmptyMessage(0);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
}
