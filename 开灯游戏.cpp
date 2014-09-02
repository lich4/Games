#include <graphics.h>
#define len1 50 //方块大小
#define len2 5  //方块间隔
#define N 4
void main()
{
	int i,j,data[N+2][N+2],sum=0;//防越界数组
	mouse_msg mouse;
	initgraph(640,480);
	setbkcolor(BLACK);
	for(i=0;i<N+2;i++)
		for(j=0;j<N+2;j++)
			data[i][j]=0;
	do
	{
		sum=0;
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
			{
				if(data[i+1][j+1]) 
				{
					setfillcolor(RED);
					sum++;
				}
				else setfillcolor(WHITE);
				bar(100+j*(len1+len2),100+i*(len1+len2),100+j*(len1+len2)+len1,
					100+i*(len1+len2)+len1);
			}
		}
		if(sum==N*N) 
		{
			setcolor(WHITE);
			outtextxy(20,20,"you win!");
			break;
		}
		if(mousehit()&&((mouse=getmouse()).flags==mouse_flag_left)&&mouse.msg==mouse_msg_down)
		{
			if(mouse.x>=100 && mouse.x<=100+N*(len1+len2) && mouse.y>=100 && 
				mouse.y<=100+N*(len1+len2))
			{
				int tx=(mouse.y-100)/(len1+len2)+1,ty=(mouse.x-100)/(len1+len2)+1;
				data[tx-1][ty]=1-data[tx-1][ty];
				data[tx+1][ty]=1-data[tx+1][ty];
				data[tx][ty-1]=1-data[tx][ty-1];
				data[tx][ty+1]=1-data[tx][ty+1];
				data[tx][ty]=1-data[tx][ty];
			}
		}
		delay(10);
	}while(1);
	getch();
	closegraph();
}