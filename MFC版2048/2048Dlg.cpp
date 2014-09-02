// 2048Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "2048.h"
#include "2048Dlg.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CMy2048Dlg dialog

CMy2048Dlg::CMy2048Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy2048Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMy2048Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy2048Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMy2048Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMy2048Dlg, CDialog)
	//{{AFX_MSG_MAP(CMy2048Dlg)
	ON_WM_PAINT()
	ON_WM_KEYUP()
	ON_COMMAND(ID_SIZE44, OnSize44)
	ON_COMMAND(ID_SIZE55, OnSize55)
	ON_COMMAND(ID_SIZE66, OnSize66)
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy2048Dlg message handlers

BOOL CMy2048Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SIZE=5;
	Data=NULL;
	mtimer=0;
	srand(time(NULL));
	InitGame();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

#define RRGB(x,y,z) (RGB(0xFF-x,0xFF-y,0xFF-z))

COLORREF getColor(int val)
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
			return RRGB(0xFF,0xFF,0xFF);
	}
}

void CMy2048Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		RECT rt;
		GetClientRect(&rt);

		int celllen=rt.right/SIZE;
		int cellhei=rt.bottom/SIZE;
		for(int i=0;i<SIZE;i++)
		{
			for(int j=0;j<SIZE;j++)
			{
				CPen pen(PS_SOLID,1,RGB(0,0,0));
				dc.SelectObject(pen);
				CBrush brush(getColor(Get(i,j)));
				dc.SelectObject(brush);
				RECT rts={j*celllen,i*cellhei,j*celllen+celllen,i*cellhei+cellhei};
				dc.Rectangle(&rts);

				if(Get(i,j))
				{
					CString buffer;
					switch(Get(i,j))
					{
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
//					itoa(Get(i,j),buffer,10);
					
					CFont font;
					font.CreateFont(30,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,"Arial");
					dc.SelectObject(font);
					dc.SetTextColor(RGB(0,0,0));
					dc.DrawText(buffer,&rts,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				}
			}
		}
		
	}
}

void CMy2048Dlg::InitGame()
{
	if(mtimer)
		KillTimer(mtimer);
	mtimer=SetTimer(0,100,NULL);
	begin=time(NULL);
	scroll=0;
	if(Data)
		delete []Data;
	Data=new int[SIZE*SIZE];

	int* curData=(int*)Data;
	for(int i=0;i<SIZE*SIZE;i++)
	{
		curData[i]=0;
	}
	GetLeftCellsToRandomPlace();
}

BOOL CMy2048Dlg::GetLeftCellsToRandomPlace()
{
	int* curData=(int*)Data;
	vector<int> left;
	for(int i=0;i<SIZE*SIZE;i++)
	{
		if(!curData[i])
			left.push_back(i);
	}
	if(left.size() == 0)
		return FALSE;

	int sel=rand()%left.size();
	int ran=rand()%6;
	if(ran<3)
		curData[left[sel]]=2;
	else if(ran<5)
		curData[left[sel]]=4;
	else
		curData[left[sel]]=8;

	RECT rt;
	GetClientRect(&rt);
	Invalidate(FALSE);

	return TRUE;
}

BOOL CMy2048Dlg::Judge()
{
	int i,j;
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			if(Get(i,j) == 0)
				return TRUE;
			if(i+1<SIZE && Get(i,j) == Get(i+1,j))
				return TRUE;
			if(j+1<SIZE && Get(i,j) == Get(i,j+1))
				return TRUE;
		}
	}
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			if(Get(i,j) == 0)
				return TRUE;
		}
	}
	return FALSE;
}

void CMy2048Dlg::ShowInfo(int goal)
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

BOOL CMy2048Dlg::Combine(UINT direction)
{
	BOOL moved=FALSE;
	switch(direction)
	{
		case VK_LEFT:
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
							ShowInfo(Get(l,first));
							Set(l,second,0);
							moved=TRUE;
							first=second+1;
						}
						else
						{
							first=second;
						}
					}
					
					//全部向左聚集
					first=0;
					while(Get(l,first))
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
								Set(l,second,0);
								first++;
								second++;
								moved=TRUE;
							}
						}
					}
				}
			}
			break;

		case VK_RIGHT:
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
							Set(l,SIZE-1-second,0);
							moved=TRUE;
							first=second+1;
						}
						else
						{
							first=second;
						}
					}
					
					//全部向右聚集
					first=0;
					while(Get(l,SIZE-1-first))
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
								Set(l,SIZE-1-second,0);
								first++;
								second++;
								moved=TRUE;
							}
						}
					}
				}
			}
			break;

		case VK_UP:
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
							Set(second,c,0);
							moved=TRUE;
							first=second+1;
						}
						else
						{
							first=second;
						}
					}
					
					//全部向上聚集
					first=0;
					while(Get(first,c))
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
								Set(second,c,0);
								first++;
								second++;
								moved=TRUE;
							}
						}
					}
				}
			}
			break;

		case VK_DOWN:
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
							Set(SIZE-1-second,c,0);
							moved=TRUE;
							first=second+1;
						}
						else
						{
							first=second;
						}
					}
					
					//全部向右聚集
					first=0;
					while(Get(SIZE-1-first,c))
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
								Set(SIZE-1-second,c,0);
								first++;
								second++;
								moved=TRUE;
							}
						}
					}
				}
			}
			break;
	}
	return moved;
}

void CMy2048Dlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar >= VK_LEFT && nChar <= VK_DOWN)
	{
		if(Combine(nChar))
		{
			GetLeftCellsToRandomPlace();
			scroll+=100;
			
			if(!Judge())
			{
				CString str;
				if(scroll < 2048)
					str.Format("失败! 最终得分：%d",scroll);
				else
					str.Format("恭喜! 最终得分：%d",scroll);
				AfxMessageBox(str);
				InitGame();
			}
		}
	}
}


void CMy2048Dlg::OnSize44() 
{
	SIZE=4;
	InitGame();
}

void CMy2048Dlg::OnSize55() 
{
	SIZE=5;
	InitGame();
}

void CMy2048Dlg::OnSize66() 
{
	SIZE=6;
	InitGame();
}

void CMy2048Dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(Data)
		delete []Data;
	CDialog::OnClose();
}

int CMy2048Dlg::Get(int x,int y)
{
	return Data[x*SIZE+y];
}

void CMy2048Dlg::Set(int x,int y,int val)
{
	Data[x*SIZE+y]=val;
}

void CMy2048Dlg::OnTimer(UINT nIDEvent) 
{
	CString curscroll;
	curscroll.Format("2048(IGDI实验室终极BOSS版)    当前得分：%d    用时：%ds",scroll,time(NULL)-begin);
			SetWindowText(curscroll);

	CDialog::OnTimer(nIDEvent);
}
