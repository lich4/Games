package com.game.lab2048;

import java.util.ArrayList;

import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.SharedPreferences;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity implements OnGestureListener
{
	private static final float FLIP_DISTANCE = 80;
	private static final int DIRECTION_UP=0;
	private static final int DIRECTION_DOWN=1;
	private static final int DIRECTION_LEFT=2;
	private static final int DIRECTION_RIGHT=3;
	private static final int SIZE=5;
	
	private String username=null;//用户名
	private int score=0;//分数
	private int highestscore=0;//历史最高分
	private TextView tvID=null;//用户名显示
	private TextView tvHighest=null;//最高分显示
	private TextView tvTime=null;//时间显示
	private TextView tvMark=null;//分数显示
	private SharedPreferences sp=null;
	
	private Handler mtimer;//定时器
	private int begin=0;//时间起点
	private Runnable runnable=null;//定时器回调
	private int singlewidth=0;
	private int singleheight=0;
	private float barwidth=0;
	private GestureDetector detector=null;
	
	private TextView[][] Pics=new TextView[SIZE][SIZE];
	private int [][] Data=new int[SIZE][SIZE];
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		runnable=new Runnable()
		{
			@Override
			public void run()
			{
				tvTime.setText(begin+"s");
				tvMark.setText(score+"");
				mtimer.postDelayed(this, 1000);
				begin++;
			}
		};
		
		mtimer=new Handler();
		
		tvID=(TextView)findViewById(R.id.ID);
		tvMark=(TextView)findViewById(R.id.mark);
		tvTime=(TextView)findViewById(R.id.time);
		tvHighest=(TextView)findViewById(R.id.highestscore);
		
		sp=getSharedPreferences("LAB2048",Activity.MODE_PRIVATE);
		username=sp.getString("ID","");
		tvTime.setText("0");
		
		highestscore=sp.getInt("HighestScore",0);
		tvHighest.setText(highestscore+"");
		
		if(username.equals(""))
		{
			AlertDialog.Builder builder=new AlertDialog.Builder(this);
			builder.setIcon(R.drawable.ic_launcher);
			builder.setTitle("请输入ID");
			final EditText et=new EditText(this);
			builder.setView(et);
			builder.setPositiveButton("确定", new OnClickListener()
			{
				@Override
				public void onClick(DialogInterface dialog, int which) 
				{
					String input=et.getText().toString();
					sp.edit().putString("ID",input).commit();
					username=input;
					tvID.setText(username);
				}
			});
			builder.show();
		}
		
		tvID.setText(username);
		tvMark.setText("0");
		
		Pics[0][0]=(TextView)findViewById(R.id.t00);
		Pics[0][1]=(TextView)findViewById(R.id.t01);
		Pics[0][2]=(TextView)findViewById(R.id.t02);
		Pics[0][3]=(TextView)findViewById(R.id.t03);
		Pics[0][4]=(TextView)findViewById(R.id.t04);
		Pics[1][0]=(TextView)findViewById(R.id.t10);
		Pics[1][1]=(TextView)findViewById(R.id.t11);
		Pics[1][2]=(TextView)findViewById(R.id.t12);
		Pics[1][3]=(TextView)findViewById(R.id.t13);
		Pics[1][4]=(TextView)findViewById(R.id.t14);
		Pics[2][0]=(TextView)findViewById(R.id.t20);
		Pics[2][1]=(TextView)findViewById(R.id.t21);
		Pics[2][2]=(TextView)findViewById(R.id.t22);
		Pics[2][3]=(TextView)findViewById(R.id.t23);
		Pics[2][4]=(TextView)findViewById(R.id.t24);
		Pics[3][0]=(TextView)findViewById(R.id.t30);
		Pics[3][1]=(TextView)findViewById(R.id.t31);
		Pics[3][2]=(TextView)findViewById(R.id.t32);
		Pics[3][3]=(TextView)findViewById(R.id.t33);
		Pics[3][4]=(TextView)findViewById(R.id.t34);
		Pics[4][0]=(TextView)findViewById(R.id.t40);
		Pics[4][1]=(TextView)findViewById(R.id.t41);
		Pics[4][2]=(TextView)findViewById(R.id.t42);
		Pics[4][3]=(TextView)findViewById(R.id.t43);
		Pics[4][4]=(TextView)findViewById(R.id.t44);
		
		LinearLayout ll=(LinearLayout)findViewById(R.id.l00);
		singlewidth=ll.getMeasuredWidth();
		singleheight=ll.getMeasuredHeight();
		barwidth=(float)(singlewidth-Pics[0][0].getMeasuredWidth())/2;
		
		detector=new GestureDetector(this);
		
		InitGame();
	}

	public int RGB(int r,int g,int b)
	{
		return (0x7f<<24)|((r&0xff)<<16)|((g&0xff)<<8)|(b&0xff);
	}
	
	public void AfxMessageBox(String str)
	{
		Toast.makeText(this, str, Toast.LENGTH_LONG).show();
	}
	
	//提交分数->排名  继续 
	public void ShowInfo(int goal)
	{
		switch(goal)
		{
			case 0x800:
				AfxMessageBox("恭喜，拿下LV1 BOSS");
				break;

			case 0x1000:
				AfxMessageBox("恭喜，拿下LV2 BOSS");
				break;

			case 0x2000:
				AfxMessageBox("恭喜，拿下LV3 BOSS");
				break;

			case 0x4000:
				AfxMessageBox("恭喜，拿下LV4 BOSS");
				break;

			case 0x8000:
				AfxMessageBox("恭喜，拿下LV5 BOSS");
				break;

			case 0x10000:
				AfxMessageBox("恭喜，拿下LV6 BOSS");
				break;
		}
	}
	
	public void InitGame()
	{
		score=0;
		
		mtimer.removeCallbacks(runnable);
		//开启定时器：mtimer.postDelayed(runnable,1000);
		begin=0;
		
		for(int i=0;i<SIZE;i++)
		{
			for(int j=0;j<SIZE;j++)
			{
				Data[i][j]=0;
			}
		}
		GetLeftCellsToRandomPlace();
	}
	
	public boolean GetLeftCellsToRandomPlace()
	{
		ArrayList<Integer> left=new ArrayList<Integer>();
		for(int i=0;i<SIZE;i++)
		{
			for(int j=0;j<SIZE;j++)
			{
				if(Data[i][j] == 0)
					left.add(i*SIZE+j);
			}
		}
		
		if(left.size() == 0)
			return false;
		
		int sel=(int) (Math.random()*left.size());
		int ran=(int) (Math.random()*10);
		int selx=left.get(sel)/SIZE;
		int sely=left.get(sel)%SIZE;
		if(ran != 0)
		{
			Data[selx][sely]=2;
		}
		else
		{
			Data[selx][sely]=4;
		}
		
		StartBirthAnimation(selx,sely);
		
		Update();
		return true;
	}
	
	public Animation StartMoveAnimation(int fromix,int fromiy,int toix,int toiy)
	{
		float fromXDelta=0.0f,toXDelta=0.0f,fromYDelta=0.0f,toYDelta=0.0f;
		fromXDelta=fromix*singlewidth+barwidth;
		fromYDelta=fromiy*singleheight+barwidth;
		toXDelta=toix*singlewidth+barwidth;
		toYDelta=toiy*singleheight+barwidth;
		View v=Pics[fromix][fromiy];
		Animation anim=new TranslateAnimation(fromXDelta,toXDelta,fromYDelta,toYDelta);
		anim.setDuration(200);
		v.startAnimation(anim);
		return anim;
	}
	
	public Animation StartBirthAnimation(int ix,int iy)
	{
		View v=Pics[ix][iy];
		Animation anim=new AlphaAnimation(0,1);
		anim.setDuration(200);
		v.startAnimation(anim);
		return anim;
	}
	
	public Animation StartDieAnimation(int ix,int iy)
	{
		View v=Pics[ix][iy];
		Animation anim=new AlphaAnimation(1,0);
		anim.setDuration(200);
		v.startAnimation(anim);
		return anim;
	}
	
	public int Get(int x,int y)
	{
		return Data[x][y];
	}
	
	public void Set(int x,int y,int val)
	{
		Data[x][y]=val;
	}
	
	public boolean Combine(int direction)//动画
	{
		boolean moved=false;
		switch(direction)
		{
			case DIRECTION_LEFT:
			{
				for(int l=0;l<SIZE;l++)
				{
					//两两向左合并
					int first=0,second;
					boolean over=false;
					while(first<SIZE && !over)
					{
						while(first<SIZE-1 && Get(l,first) == 0)
							first++;
						
						if(first>=SIZE-1)
						{
							over=true;
							break;
						}

						second=first+1;
						while(second<SIZE && Get(l,second) == 0)
							second++;

						if(second>=SIZE)
						{
							over=true;
							break;
						}

						if(Get(l,first) == Get(l,second))
						{
							Set(l,first,Get(l,first)+Get(l,second));
							StartMoveAnimation(l,second,l,first);
							ShowInfo(Get(l,first));
							Set(l,second,0);
							moved=true;
							first=second+1;
						}
						else
						{
							first=second;
						}
					}
					
					//全部向左聚集
					first=0;
					while(first < SIZE && Get(l,first) != 0)
						first++;//找到第一个空白处
					if(first<SIZE)
					{
						second=first+1;
						while(second<SIZE)
						{//找到下一个有值处	
							while(second<SIZE && Get(l,second) == 0)
							{
								second++;
							}
							if(second<SIZE)
							{
								Set(l,first,Get(l,second));
								StartMoveAnimation(l,second,l,first);
								Set(l,second,0);
								first++;
								second++;
								moved=true;
							}
						}
					}
				}
				break;
			}
			
			case DIRECTION_RIGHT:
			{
				for(int l=0;l<SIZE;l++)
				{
					//两两向右合并
					int first=0,second;
					boolean over=false;
					while(first<SIZE && !over)
					{
						while(first<SIZE-1 && Get(l,SIZE-1-first) == 0)
							first++;
						
						if(first>=SIZE-1)
						{
							over=true;
							break;
						}
						
						second=first+1;
						while(second<SIZE && Get(l,SIZE-1-second) == 0)
							second++;
						
						if(second>=SIZE)
						{
							over=true;
							break;
						}
						
						if(Get(l,SIZE-1-first) == Get(l,SIZE-1-second))
						{
							Set(l,SIZE-1-first,Get(l,SIZE-1-first)+Get(l,SIZE-1-second));
							StartMoveAnimation(l,SIZE-1-second,l,SIZE-1-first);
							ShowInfo(Get(l,first));
							Set(l,SIZE-1-second,0);
							moved=true;
							first=second+1;
						}
						else
						{
							first=second;
						}
					}
					
					//全部向右聚集
					first=0;
					while(first < SIZE && Get(l,SIZE-1-first) != 0)
						first++;//找到第一个空白处
					if(first<SIZE)
					{
						second=first+1;
						while(second<SIZE)
						{//找到下一个有值处	
							while(second<SIZE && Get(l,SIZE-1-second) == 0)
							{
								second++;
							}
							if(second<SIZE)
							{
								Set(l,SIZE-1-first,Get(l,SIZE-1-second));
								StartMoveAnimation(l,SIZE-1-second,l,SIZE-1-first);
								Set(l,SIZE-1-second,0);
								first++;
								second++;
								moved=true;
							}
						}
					}
				}
				break;
			}
			
			case DIRECTION_UP:
			{
				for(int c=0;c<SIZE;c++)
				{
					//两两向上合并
					int first=0,second;
					boolean over=false;
					while(first<SIZE && !over)
					{
						while(first<SIZE-1 && Get(first,c) == 0)
							first++;
						
						if(first>=SIZE-1)
						{
							over=true;
							break;
						}
						
						second=first+1;
						while(second<SIZE && Get(second,c) == 0)
							second++;
						
						if(second>=SIZE)
						{
							over=true;
							break;
						}
						
						if(Get(first,c) == Get(second,c))
						{
							Set(first,c,Get(first,c)+Get(second,c));
							StartMoveAnimation(second,c,first,c);
							ShowInfo(Get(first,c));
							Set(second,c,0);
							moved=true;
							first=second+1;
						}
						else
						{
							first=second;
						}
					}
					
					//全部向上聚集
					first=0;
					while(first < SIZE && Get(first,c) != 0)
						first++;//找到第一个空白处
					if(first<SIZE)
					{
						second=first+1;
						while(second<SIZE)
						{//找到下一个有值处	
							while(second<SIZE && Get(second,c) == 0)
							{
								second++;
							}
							if(second<SIZE)
							{
								Set(first,c,Get(second,c));
								StartMoveAnimation(second,c,first,c);
								Set(second,c,0);
								first++;
								second++;
								moved=true;
							}
						}
					}
				}
				break;
			}
			
			case DIRECTION_DOWN:
			{
				for(int c=0;c<SIZE;c++)
				{
					//两两向右合并
					int first=0,second;
					boolean over=false;
					while(first<SIZE && !over)
					{
						while(first<SIZE-1 && Get(SIZE-1-first,c) == 0)
							first++;
						
						if(first>=SIZE-1)
						{
							over=true;
							break;
						}
						
						second=first+1;
						while(second<SIZE && Get(SIZE-1-second,c) == 0)
							second++;
						
						if(second>=SIZE)
						{
							over=true;
							break;
						}
						
						if(Get(SIZE-1-first,c) == Get(SIZE-1-second,c))
						{
							Set(SIZE-1-first,c,Get(SIZE-1-first,c)+Get(SIZE-1-second,c));
							StartMoveAnimation(SIZE-1-second,c,SIZE-1-first,c);
							ShowInfo(Get(first,c));
							Set(SIZE-1-second,c,0);
							moved=true;
							first=second+1;
						}
						else
						{
							first=second;
						}
					}
					
					//全部向右聚集
					first=0;
					while(first < SIZE && Get(SIZE-1-first,c) != 0)
						first++;//找到第一个空白处
					if(first<SIZE)
					{
						second=first+1;
						while(second<SIZE)
						{//找到下一个有值处	
							while(second<SIZE && Get(SIZE-1-second,c) == 0)
							{
								second++;
							}
							if(second<SIZE)
							{
								Set(SIZE-1-first,c,Get(SIZE-1-second,c));
								StartMoveAnimation(SIZE-1-second,c,SIZE-1-first,c);
								Set(SIZE-1-second,c,0);
								first++;
								second++;
								moved=true;
							}
						}
					}
				}
				break;
			}	
		}
		return moved;
	}
	
	public boolean Judge()
	{
		int i,j;
		for(i=0;i<SIZE;i++)
		{
			for(j=0;j<SIZE;j++)
			{
				if(Data[i][j] == 0)
					return true;
				if(i+1 < SIZE && Data[i][j] == Data[i+1][j])
					return true;
				if(j+1 < SIZE && Data[i][j] == Data[i][j+1])
					return true;
			}
		}
		
		return false;
	}
	
	public int getColor(int val)
	{
		switch(val)
		{
			case 0x0:
				return RGB(255,255,255);
			case 0x2:
				return RGB(124,252,0);
			case 0x4:
				return RGB(102,205,170);
			case 0x8:
				return RGB(205,92,92);
			case 0x10:
				return RGB(255,215,0);
			case 0x20:
				return RGB(255,140,0);
			case 0x40:
				return RGB(219,112,147);
			case 0x80:
				return RGB(135,206,215);
			case 0x100:
				return RGB(0,0,255);
			case 0x200:
				return RGB(123,104,235);
			case 0x400:
				return RGB(0,255,0);
			case 0x800:
				return RGB(255,0,0);
			case 0x1000:
				return RGB(0,255,255);
			case 0x2000:
				return RGB(255,0,255);
			case 0x4000:
				return RGB(255,255,0);
			case 0x8000:
				return RGB(128,128,128);
			case 0x10000:
				return RGB(0,0,0);
			default:
				return RGB(0,0,0);
		}
	}
	
	//相对于OnPaint
	public void Update()
	{
		for(int i=0;i<SIZE;i++)
		{
			for(int j=0;j<SIZE;j++)
			{
				int color=getColor(Data[i][j]);
				Pics[i][j].setTextColor(0xFF000000);
				Pics[i][j].setBackgroundColor(color);
				String buffer="";
				switch(Data[i][j])
				{
					case 0x0:
						break;
				
					case 0x2:
						buffer="周志中";
						break;
						
					case 0x4:
						buffer="庞启雄";
						break;
	
					case 0x8:
						buffer="刘柳";
						break;
	
					case 0x10:
						buffer="汪松兴";
						break;
	
					case 0x20:
						buffer="夏航宇";
						break;
	
					case 0x40:
						buffer="李妍";
						break;
	
					case 0x80:
						buffer="徐蕾";
						break;
	
					case 0x100:
						buffer="李超";
						break;
	
					case 0x200:
						buffer="吴云";
						break;
	
					case 0x400:
						buffer="田原";
						break;
	
					case 0x800:
						buffer="卢老师";
						break;
	
					case 0x1000:
						buffer="胡老师";
						break;
	
					case 0x2000:
						buffer="沈老师";
						break;
	
					case 0x4000:
						buffer="孙院长";
						break;
	
					case 0x8000:
						buffer="肖国镇";
	
					default:
						buffer="神";
						break;
				}
				Pics[i][j].setText(buffer);
			}
		}
	}

	@Override
	public boolean onDown(MotionEvent e) 
	{
		return false;
	}

	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,float velocityY)
	{
		int direction=-1;
		if(e1.getX() - e2.getX() > FLIP_DISTANCE)
		{
			direction=DIRECTION_LEFT;
		}
		else if(e2.getX() - e1.getX() > FLIP_DISTANCE)
		{
			direction=DIRECTION_RIGHT;
		}
		
		if(e1.getY() - e2.getY() > FLIP_DISTANCE)
		{
			direction=DIRECTION_UP;
		}
		else if(e2.getY() - e1.getY() > FLIP_DISTANCE)
		{
			direction=DIRECTION_DOWN;
		}
		
		if(direction != -1)
		{
			if(Combine(direction))
			{
				GetLeftCellsToRandomPlace();
				if(score == 0)
				{
					mtimer.postDelayed(runnable,1000);
					begin=0;
				}
				
				score += 100;
				if(!Judge())
				{
					String str1,str2;
					if(MeasureScore())
						str1="成功!";
					else
						str1="失败!";
					str2="最终得分:"+score;
					
						
					if(score > highestscore)
					{
						highestscore=score;
						tvHighest.setText(highestscore+"");
						sp.edit().putInt("HighestScore",highestscore).commit();
					}
					AlertDialog.Builder builder=new AlertDialog.Builder(this);
					builder.setTitle(str1);
					builder.setMessage(str2);
					builder.setPositiveButton("上传成绩",new OnClickListener()
					{
						@Override
						public void onClick(DialogInterface dialog, int which) 
						{
							Intent intent=new Intent(MainActivity.this,UploadMark.class);
							intent.putExtra("id",username);
							intent.putExtra("score",score);
							startActivity(intent);
							mtimer.removeCallbacks(runnable);
							InitGame();
						}
					});
					builder.setNegativeButton("继续游戏",new OnClickListener()
					{
						@Override
						public void onClick(DialogInterface dialog, int which) 
						{
							mtimer.removeCallbacks(runnable);
							InitGame();
						}
					});
					
					builder.setCancelable(false);
					builder.show();
				}
			}
			return true;
		}
		return false;
	}

	public boolean MeasureScore()
	{
		int measuredscore=0;
		for(int i=0;i<SIZE;i++)
		{
			for(int j=0;j<SIZE;j++)
			{
				int val=Get(i,j);
				if(val >= 2048)
				{
					measuredscore += val*100;
				}
			}
		}

		score += measuredscore;

		if(measuredscore != 0)
			return true;
		else
			return false;
	}
	
	@Override
	public void onLongPress(MotionEvent e) 
	{
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX,float distanceY) 
	{
		return false;
	}

	@Override
	public void onShowPress(MotionEvent e) 
	{		
	}

	@Override
	public boolean onSingleTapUp(MotionEvent e)
	{
		return false;
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) 
	{
		return detector.onTouchEvent(event);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) 
	{
	   super.onCreateOptionsMenu(menu);
	   menu.add(0,0,0,"上传分数");
	   menu.add(0,0,1,"更改姓名");
	   menu.add(0,0,2,"新建游戏");
	   menu.add(0,0,3,"退出游戏");
	   return true;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
	   switch (item.getOrder()) 
	   {
		   case 0:
				Intent intent=new Intent(MainActivity.this,UploadMark.class);
				intent.putExtra("id",username);
				intent.putExtra("score",score);
				startActivity(intent);
				return true;
				
		   case 1:
			    AlertDialog.Builder builder=new AlertDialog.Builder(this);
				builder.setIcon(R.drawable.ic_launcher);
				builder.setTitle("请输入ID");
				final EditText et=new EditText(this);
				builder.setView(et);
				builder.setPositiveButton("确定", new OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which) 
					{
						String input=et.getText().toString();
						sp.edit().remove("ID");
						sp.edit().putString("ID",input).commit();
						username=input;
						tvID.setText(username);
					}
				});
				builder.show();
		       	return true;
		       	
		   case 2:
			    mtimer.removeCallbacks(runnable);
				InitGame();
		        return true;
		       
		   case 3:
			    finish();
			    return true;
	   }
	   return false;
	}
}



