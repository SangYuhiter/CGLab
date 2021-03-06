
// CGLabView.cpp: CCGLabView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CGLab.h"
#endif

#include "CGLabDoc.h"
#include "CGLabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//颜色集
COLORREF colors[3] = {
	RGB(255,0,0),RGB(0,255,0),RGB(0,0,255),
};
int colorselect = 0;

// CCGLabView

IMPLEMENT_DYNCREATE(CCGLabView, CView)

BEGIN_MESSAGE_MAP(CCGLabView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DRAWPOLY, &CCGLabView::OnDrawpoly)
	ON_COMMAND(ID_SEEDFILL4, &CCGLabView::OnSeedfill4)
	ON_COMMAND(ID_TRANSLATION, &CCGLabView::OnTranslation)
	ON_COMMAND(ID_SEMMETRY_X, &CCGLabView::OnSemmetryX)
	ON_COMMAND(ID_SEMMETRY_Y, &CCGLabView::OnSemmetryY)
	ON_COMMAND(ID_SEMMETRY_POINT, &CCGLabView::OnSemmetryPoint)
	ON_COMMAND(ID_ROTATION, &CCGLabView::OnRotation)
	ON_COMMAND(ID_ZOOM, &CCGLabView::OnZoom)
	ON_COMMAND(ID_COHEN_CUT, &CCGLabView::OnCohenCut)
	ON_COMMAND(ID_CARTOON1, &CCGLabView::OnCartoon1)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_INITALL, &CCGLabView::OnInitall)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCGLabView 构造/析构

CCGLabView::CCGLabView()
{
	// TODO: 在此处添加构造代码
	//多边形绘制
	m_pointNum = 0;
	graphtype = 0;
	m_point.clear();
	//多边形填充
	graphfilltype = 0;
	pSeed = 0;
	m_stk.empty();
	//平移和对称
	graphtranstype = 0;
	//初始化方向操作
	key_up[0] = FALSE; key_up[1] = FALSE;
	up_distance = 0;
	key_down[0] = FALSE; key_down[1] = FALSE;
	down_distance = 0;
	key_left[0] = FALSE; key_left[1] = FALSE;
	left_distance = 0;
	key_right[0] = FALSE; key_right[1] = FALSE;
	right_distance = 0;
	graphsemmtryflag = FALSE;
	pSemmetry = 0;
	//旋转
	graphrotationflag = FALSE;
	pRotation = 0;
	rotationangle = 0;
	//缩放
	graphzoomflag = FALSE;
	pZoom = 0;
	zoomproportionx = zoomproportiony = 1;
	//坐标原点
	pXYSystem = 0;

	cut_point.clear();
	cut_pointNum = 0;
	graphcutflag = FALSE;
	graphcuttype = 0;

	cartoonflag = FALSE;
	cartoontype = 0;
	cartoon_ball.clear();
	cartoon_ballNum = 0;
	xadd.clear();
	yadd.clear();
}

CCGLabView::~CCGLabView()
{
}

BOOL CCGLabView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCGLabView 绘图

void CCGLabView::OnDraw(CDC* /*pDC*/)
{
	CCGLabDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	DrawBall();
}


// CCGLabView 打印

BOOL CCGLabView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCGLabView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCGLabView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CCGLabView 诊断

#ifdef _DEBUG
void CCGLabView::AssertValid() const
{
	CView::AssertValid();
}

void CCGLabView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGLabDoc* CCGLabView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGLabDoc)));
	return (CCGLabDoc*)m_pDocument;
}
#endif //_DEBUG


// CCGLabView 消息处理程序

//画多边形
void CCGLabView::OnDrawpoly()
{
	// TODO: 在此添加命令处理程序代码
	//绘制图形类型
	Initallthing();
	graphtype = 1;
}


void CCGLabView::OnSeedfill4()
{
	// TODO: 在此添加命令处理程序代码
	graphfilltype = 1;
}


void CCGLabView::OnTranslation()
{
	// TODO: 在此添加命令处理程序代码
	graphtranstype = 1;
}


void CCGLabView::OnSemmetryX()
{
	// TODO: 在此添加命令处理程序代码
	graphsemmtryflag = TRUE;
	graphtranstype = 2;
}


void CCGLabView::OnSemmetryY()
{
	// TODO: 在此添加命令处理程序代码
	graphsemmtryflag = TRUE;
	graphtranstype = 3;
}


void CCGLabView::OnSemmetryPoint()
{
	// TODO: 在此添加命令处理程序代码
	graphsemmtryflag = TRUE;
	graphtranstype = 4;
}


void CCGLabView::OnRotation()
{
	// TODO: 在此添加命令处理程序代码
	graphrotationflag = TRUE;
	graphtranstype = 5;
}


void CCGLabView::OnZoom()
{
	// TODO: 在此添加命令处理程序代码
	graphzoomflag = TRUE;
	graphtranstype = 6;
}


void CCGLabView::OnCohenCut()
{
	// TODO: 在此添加命令处理程序代码
	graphcutflag = TRUE;
	graphcuttype = 1;
}


void CCGLabView::OnCartoon1()
{
	// TODO: 在此添加命令处理程序代码
	cartoonflag = TRUE;
	cartoontype = 1;
	SetTimer(ID_CARTOON1, 10, NULL);
}

void CCGLabView::OnInitall()
{
	// TODO: 在此添加命令处理程序代码
	Initallthing();
}

//鼠标左键收集点信息
void CCGLabView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//非对称，旋转，缩放时，将鼠标左点加入图节点
	if (!graphsemmtryflag && !graphrotationflag && !graphzoomflag && !graphcutflag)
	{
		m_point.push_back(point);
		m_pointNum++;
	}
	if (graphsemmtryflag) pSemmetry = point;
	if (graphrotationflag)pRotation = point;
	if (graphzoomflag)pZoom = point;
	if (graphcutflag)
	{
		cut_point.push_back(point);
		cut_pointNum++;
	}
	if (cartoonflag && cartoontype == 1)
	{
		cartoon_ball.push_back(point);
		cartoon_ballNum++;

		xadd.push_back(5);
		yadd.push_back(5);
	}
	CView::OnLButtonDown(nFlags, point);
}


void CCGLabView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//种子填充点
	pSeed = point;
	//绘制多边形
	if (m_pointNum >= 2 && graphtype == 1)
	{
		DrawPoly();
		//m_pointNum = 0;
		//m_point.clear();
	}
	if (graphfilltype == 1)
	{
		PolyFill();
	}
	if (graphtranstype != 0)
	{
		switch (graphtranstype)
		{
		case 1:
			MatrixTrans();
			DrawPoly();
			key_up[0] = FALSE; key_up[1] = FALSE;
			up_distance = 0;
			key_down[0] = FALSE; key_down[1] = FALSE;
			down_distance = 0;
			key_left[0] = FALSE; key_left[1] = FALSE;
			left_distance = 0;
			key_right[0] = FALSE; key_right[1] = FALSE;
			right_distance = 0;
			break;
		case 2:
		case 3:
		case 4:
			pXYSystem = pSemmetry;
			DrawXYSystem();
			MatrixTrans();
			graphsemmtryflag = FALSE;
			DrawPoly();
			break;
		case 5:
			pXYSystem = pRotation;
			DrawXYSystem();
			MatrixTrans();
			graphrotationflag = FALSE;
			DrawPoly();
			break;
		case 6:
			pXYSystem = pZoom;
			DrawXYSystem();
			MatrixTrans();
			graphzoomflag = FALSE;
			DrawPoly();
			break;
		default:
			break;
		}
	}
	if (graphcuttype != 0)
	{
		DrawCutEdge();
		DrawCutLine();
	}
	CView::OnRButtonDown(nFlags, point);
}


//绘制多边形
void CCGLabView::DrawPoly()
{
	// TODO: 在此处添加实现代码.
	int i;
	if (m_pointNum > 1)
	{
		for (i = 0; i < m_pointNum; i++)
		{
			if (i == m_pointNum - 1)
			{
				DrawLine(m_point[m_pointNum - 1], m_point[0]);
			}
			else
			{
				DrawLine(m_point[i], m_point[i + 1]);
			}
		}
	}
	return;
}


// 绘制直线
void CCGLabView::DrawLine(CPoint p1, CPoint p2)
{
	// TODO: 在此处添加实现代码.
	int color = RGB(255, 0, 0);
	CDC *pDC;
	pDC = GetWindowDC();

	int xs = p1.x, ys = p1.y;
	int xe = p2.x, ye = p2.y;

	//规格化调整，x小的为起点
	if (xs > xe)
	{
		int t;
		t = xs; xs = xe; xe = t;
		t = ys; ys = ye; ye = t;
	}
	int x = xs, y = ys;

	//线段垂直
	if (xs == xe)
	{
		//规格化,y小的为起点
		if (ys > ye)
		{
			int t;
			t = ys; ys = ye; ye = t;
		}
		y = ys;
		while (y <= ye)
		{
			pDC->SetPixel(x, y, color);
			y++;
		}
		return;
	}

	//线段水平
	if (ys == ye)
	{
		//已规格化
		while (x <= xe)
		{
			pDC->SetPixel(x, y, color);
			x++;
		}
		return;
	}

	int dx = xe - xs, dy = ye - ys;
	int twoDx = 2 * dx, twoDy = 2 * dy;
	int twoDyMinusDx = 2 * (dy - dx);
	int twoDxMinusDy = 2 * (dx - dy);
	int twoDxAddDy = 2 * (dx + dy);
	int d;
	double k = (double)(ye - ys) / (double)(xe - xs);
	pDC->SetPixel(x, y, color);	//绘制第一个点
								//当线段斜率0<k<=1时。
	if (0 < k && k <= 1)
	{
		d = 2 * dy - dx;
		while (x < xe)
		{
			x++;
			if (d < 0)
			{
				d += twoDy;
			}
			else
			{
				y++;
				d += twoDyMinusDx;
			}
			pDC->SetPixel(x, y, color);
		}
		return;
	}
	//当线段斜率-1<=k<0时。
	if (k < 0 && k >= -1)
	{
		d = 2 * dy + dx;
		while (x < xe)
		{
			x++;
			if (d >= 0)
				d += twoDy;
			else
			{
				y--;
				d += twoDxAddDy;
			}
			pDC->SetPixel(x, y, color);
		}
		return;
	}
	//当线段斜率k<-1时。
	if (k < -1)
	{
		d = 2 * dx - dy;
		while (y > ye)
		{
			y--;
			if (d >= 0)
				d -= twoDx;
			else
			{
				x++;
				d -= twoDxAddDy;
			}
			pDC->SetPixel(x, y, color);
		}
		return;
	}
	//当线段斜率k>1时。
	if (k > 1)
	{
		d = dy;
		while (y < ye)
		{
			y++;
			if (d < 0)
			{
				d += twoDx;
			}
			else
			{
				x++;
				d -= twoDyMinusDx;
			}
			pDC->SetPixel(x, y, color);
		}
		return;
	}
}


// 初始化+清屏
void CCGLabView::Initallthing()
{
	// TODO: 在此处添加实现代码.
	//多边形绘制
	m_pointNum = 0;
	graphtype = 0;
	m_point.clear();
	//多边形填充
	graphfilltype = 0;
	pSeed = 0;
	m_stk.empty();
	//平移和对称
	graphtranstype = 0;
	//初始化方向操作
	key_up[0] = FALSE; key_up[1] = FALSE;
	up_distance = 0;
	key_down[0] = FALSE; key_down[1] = FALSE;
	down_distance = 0;
	key_left[0] = FALSE; key_left[1] = FALSE;
	left_distance = 0;
	key_right[0] = FALSE; key_right[1] = FALSE;
	right_distance = 0;
	graphsemmtryflag = FALSE;
	pSemmetry = 0;
	//旋转
	graphrotationflag = FALSE;
	pRotation = 0;
	rotationangle = 0;
	//缩放
	graphzoomflag = FALSE;
	pZoom = 0;
	zoomproportionx = zoomproportiony = 1;
	//坐标原点
	pXYSystem = 0;

	//直线裁剪
	cut_point.clear();
	cut_pointNum = 0;
	graphcutflag = FALSE;
	graphcuttype = 0;

	//动画
	cartoonflag = FALSE;
	cartoontype = 0;
	cartoon_ball.clear();
	cartoon_ballNum = 0;
	xadd.clear();
	yadd.clear();
	//清屏
	Invalidate();
	return;
}


// 多边形填充
void CCGLabView::PolyFill()
{
	// TODO: 在此处添加实现代码.
	int back_color = RGB(255, 255, 255),
		bound_color = RGB(255, 0, 0),
		fill_color = RGB(0, 0, 255);
	//ScanFill();
	SeedFill4(pSeed, fill_color, bound_color);
	return;
}


void CCGLabView::SeedFill4(CPoint pSeed, int fill_color, int bound_color)
{
	// TODO: 在此处添加实现代码.
	CDC *pDC;
	pDC = GetWindowDC();

	CPoint px, pa, pb, pc, pd;
	m_stk.push(pSeed);
	while (m_stk.size() != 0)
	{
		CPoint px = m_stk.top();
		m_stk.pop();
		pDC->SetPixel(px.x, px.y, fill_color);
		int a = pDC->GetPixel(px.x, px.y + 1),
			b = pDC->GetPixel(px.x, px.y - 1),
			c = pDC->GetPixel(px.x + 1, px.y),
			d = pDC->GetPixel(px.x - 1, px.y);

		if (c != fill_color && c != bound_color)
		{
			pc.x = px.x + 1; pc.y = px.y;
			m_stk.push(pc);
		}
		if (a != fill_color && a != bound_color)
		{
			pa.x = px.x; pa.y = px.y + 1;
			m_stk.push(pa);
		}
		if (d != fill_color && d != bound_color)
		{
			pd.x = px.x - 1; pd.y = px.y;
			m_stk.push(pd);
		}
		if (b != fill_color && b != bound_color)
		{
			pb.x = px.x; pb.y = px.y - 1;
			m_stk.push(pb);
		}

	}
	return;
}


void CCGLabView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nChar)
	{
		//记录方向键上压下并开启计数
	case VK_UP:
		if (graphtranstype == 1)
		{
			key_up[0] = TRUE;
			SetTimer(VK_UP, 5, NULL);	//5ms记一次数
		}
		break;
	case VK_DOWN:
		if (graphtranstype == 1)
		{
			key_down[0] = TRUE;
			SetTimer(VK_DOWN, 5, NULL);	//5ms记一次数
		}
		break;
	case VK_LEFT:
		if (graphtranstype == 1)
		{
			key_left[0] = TRUE;
			SetTimer(VK_LEFT, 5, NULL);	//5ms记一次数
		}
		break;
	case VK_RIGHT:
		if (graphtranstype == 1)
		{
			key_right[0] = TRUE;
			SetTimer(VK_RIGHT, 5, NULL);	//5ms记一次数
		}
		break;
	default:
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CCGLabView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nChar)
	{
		//记录方向键上抬起并关闭计数
	case VK_UP:
		if (graphtranstype == 1)
		{
			key_up[1] = TRUE;
			KillTimer(VK_UP);
		}
		break;
	case VK_DOWN:
		if (graphtranstype == 1)
		{
			key_down[1] = TRUE;
			KillTimer(VK_DOWN);
		}
		break;
	case VK_LEFT:
		if (graphtranstype == 1)
		{
			key_left[1] = TRUE;
			KillTimer(VK_LEFT);
		}
		break;
	case VK_RIGHT:
		if (graphtranstype == 1)
		{
			key_right[1] = TRUE;
			KillTimer(VK_RIGHT);
		}
		break;
	default:
		break;
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CCGLabView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case VK_UP:
		//向上移动
		up_distance += 1;
		break;
	case VK_DOWN:
		down_distance += 1;
		break;
	case VK_LEFT:
		left_distance += 1;
		break;
	case VK_RIGHT:
		right_distance += 1;
		break;
	case ID_CARTOON1:
		for (int i = 0; i < cartoon_ballNum; i++)
		{
			cartoon_ball[i].x += xadd[i];
			cartoon_ball[i].y += yadd[i];
		}
		int cx = GetSystemMetrics(SM_CXFULLSCREEN);
		int cy = GetSystemMetrics(SM_CYFULLSCREEN);
		//球遇到侧壁
		for (int i = 0; i < cartoon_ballNum; i++)
		{
			if (cartoon_ball[i].x > cx - 50 || cartoon_ball[i].x < 50)
			{
				xadd[i] = -xadd[i];
				colorselect++;
			}

			if (cartoon_ball[i].y > cy - 100 || cartoon_ball[i].y < 100)
			{
				yadd[i] = -yadd[i];
				colorselect++;
			}
		}
		Invalidate();
		break;
	}
	CView::OnTimer(nIDEvent);
}


void CCGLabView::DrawXYSystem()
{
	// TODO: 在此处添加实现代码.
	int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	CPoint px, py;
	px.x = 0; px.y = pXYSystem.y;
	py.x = cx; py.y = pXYSystem.y;
	DrawLine(px, py);
	px.x = pXYSystem.x; px.y = 0;
	py.x = pXYSystem.x; py.y = cy;
	DrawLine(px, py);
	return;
}


// 矩阵变换
void CCGLabView::MatrixTrans()
{
	// TODO: 在此处添加实现代码.
	//初始化变换矩阵
	float trans[3][3] = { { 1,0,0 },{ 0,1,0 },{ 0,0,1 } };
	float point[3] = { 0,0,1 };

	//根据标志位设置变换矩阵
	//平移变换
	//[1  0  0
	// 0  1  0
	// tx ty 1]
	//向上平移:ty=-up_distance
	if (graphtranstype == 1 && key_up[0] == TRUE && key_up[1] == TRUE)
	{
		trans[2][1] = -up_distance;
	}

	//向下平移:ty=down_distance
	if (graphtranstype == 1 && key_down[0] == TRUE && key_down[1] == TRUE)
	{
		trans[2][1] = down_distance;
	}

	//向左平移:tx=-left_distance
	if (graphtranstype == 1 && key_left[0] == TRUE && key_left[1] == TRUE)
	{
		trans[2][0] = -left_distance;
	}

	//向右平移:tx=right_distance
	if (graphtranstype == 1 && key_right[0] == TRUE && key_right[1] == TRUE)
	{
		trans[2][0] = right_distance;
	}

	//对称变换
	//[Y 0 0
	// 0 X 0
	// 0 0 1]
	//X轴对称：X=-1
	if (graphsemmtryflag == TRUE && graphtranstype == 2)
	{
		trans[1][1] = -1;
		trans[2][1] = (float)(2 * pSemmetry.y);
	}
	//Y轴对称：Y=-1
	if (graphsemmtryflag == TRUE && graphtranstype == 3)
	{
		trans[0][0] = -1;
		trans[2][0] = (float)(2 * pSemmetry.x);
	}
	//原点对称：X=-1,Y=-1
	if (graphsemmtryflag == TRUE && graphtranstype == 4)
	{
		trans[0][0] = -1;
		trans[1][1] = -1;
		trans[2][0] = (float)(2 * pSemmetry.x);
		trans[2][1] = (float)(2 * pSemmetry.y);
	}

	//旋转变换
	rotationangle = PI / 12.0;	//旋转角度，顺时针为正
								//[ cosA sinA 0
								// -sinA cosA 0
								//   0    0   1]
	if (graphrotationflag == TRUE)
	{
		trans[0][0] = cos(rotationangle);
		trans[0][1] = sin(rotationangle);
		trans[1][0] = -sin(rotationangle);
		trans[1][1] = cos(rotationangle);
	}

	//比例缩放
	zoomproportionx = zoomproportiony = 1 / 2.0;	//大于1放大；小于1缩小
													//[SX 0  0
													// 0  SY 0
													// 0  0  1]
	if (graphzoomflag)
	{
		trans[0][0] = zoomproportionx;
		trans[1][1] = zoomproportiony;
	}

	for (int i = 0; i < m_pointNum; i++)
	{
		float point[3] = { 0,0,1 };
		point[0] = (float)m_point[i].x;
		point[1] = (float)m_point[i].y;
		if (graphrotationflag)
		{
			point[0] = point[0] - (float)pRotation.x;
			point[1] = point[1] - (float)pRotation.y;
		}
		if (graphzoomflag)
		{
			point[0] = point[0] - (float)pZoom.x;
			point[1] = point[1] - (float)pZoom.y;
		}
		point[0] = point[0] * trans[0][0] + point[1] * trans[1][0] + point[2] * trans[2][0];
		point[1] = point[0] * trans[0][1] + point[1] * trans[1][1] + point[2] * trans[2][1];
		point[2] = point[0] * trans[0][2] + point[1] * trans[1][2] + point[2] * trans[2][2];
		if (graphrotationflag)
		{
			point[0] = point[0] + (float)pRotation.x;
			point[1] = point[1] + (float)pRotation.y;
		}
		if (graphzoomflag)
		{
			point[0] = point[0] + (float)pZoom.x;
			point[1] = point[1] + (float)pZoom.y;
		}
		m_point[i].x = (int)point[0];
		m_point[i].y = (int)point[1];
	}
	return;
}


// 绘制裁剪边框
void CCGLabView::DrawCutEdge()
{
	// TODO: 在此处添加实现代码.
	switch (graphcuttype)
	{
	case 1:
		DrawCutRec();
		break;
	default:
		break;
	}
	return;
}


// 绘制矩形裁剪边框
void CCGLabView::DrawCutRec()
{
	// TODO: 在此处添加实现代码.
	CPoint pa=cut_point[0], pb=cut_point[0],
		   pc=cut_point[1], pd=cut_point[1];
	pb.y = pd.y;
	pd.y = pa.y;
	DrawLine(pa, pb);
	DrawLine(pb, pc);
	DrawLine(pc, pd);
	DrawLine(pd, pa);
	return;
}


// 绘制裁剪线段
void CCGLabView::DrawCutLine()
{
	//获得裁剪框边界
	int xmin, xmax, ymin, ymax;
	if (cut_point[0].x < cut_point[1].x)
	{
		xmin = cut_point[0].x;
		xmax = cut_point[1].x;
	}
	else
	{
		xmin = cut_point[1].x;
		xmax = cut_point[0].x;
	}
	if (cut_point[0].y < cut_point[1].y)
	{
		ymin = cut_point[0].y;
		ymax = cut_point[1].y;
	}
	else
	{
		ymin = cut_point[1].y;
		ymax = cut_point[0].y;
	}

	// TODO: 在此处添加实现代码.
	for (int i = 2; i < cut_pointNum; i += 2)
	{
		CPoint startpoint, endpoint;
		//横坐标小的为起点
		if (cut_point[i].x < cut_point[i + 1].x)
		{
			startpoint = cut_point[i];
			endpoint = cut_point[i + 1];
		}
		else
		{
			startpoint = cut_point[i+1];
			endpoint = cut_point[i];
		}
		//线段编码
		linecode startpointcode,endpointcode;
		startpointcode = LineEncode(xmax, xmin, ymax, ymin, startpoint);
		endpointcode = LineEncode(xmax, xmin, ymax, ymin, endpoint);

		//线段完全可见
		if ((startpointcode.Ct == 0 && startpointcode.Cb == 0 && startpointcode.Cl == 0 && startpointcode.Cr == 0) &&
			(endpointcode.Ct == 0 && endpointcode.Cb == 0 && endpointcode.Cl == 0 && endpointcode.Cr == 0))
		{
			DrawLine(startpoint,endpoint);
			continue;
		}

		//线段完全不可见
		//1、线段在边框外同侧（编码相与不为0）
		if (((startpointcode.Ct & endpointcode.Ct) == 1) || ((startpointcode.Cb & endpointcode.Cb) == 1) ||
			((startpointcode.Cl & endpointcode.Cl) == 1) || ((startpointcode.Cr & endpointcode.Cr) == 1))
		{
			continue;
		}

		//线段与边框延长线求交
		CPoint pleft, pup, pright, pdown;
		//左边界
		//若起点在左边界以左
		if (startpointcode.Cl == 1)
		{
			pleft.x = xmin;
			pleft.y = CalculatePositionY(startpoint, endpoint, xmin);
			//交点在左边框延长线上,线段不可见
			if (pleft.y > ymax || pleft.y < ymin)
			{
				continue;
			}
			else	//更新结点
			{
				startpoint = pleft;
			}
		}
		//上边界
		//若起点在上边界以上
		if (startpointcode.Ct == 1)
		{
			pup.x = CalculatePositionX(startpoint, endpoint, ymax);
			pup.y = ymax;
			
			//交点在上边框延长线上,线段不可见
			if (pup.x > xmax || pup.x < xmin)
			{
				continue;
			}
			else	//更新结点
			{
				startpoint = pup;
			}
		}
		//若终点在上边界以上
		if (endpointcode.Ct == 1)
		{
			pup.x = CalculatePositionX(startpoint, endpoint, ymax);
			pup.y = ymax;

			//交点在上边框延长线上,线段不可见
			if (pup.x > xmax || pup.x < xmin)
			{
				continue;
			}
			else	//更新结点
			{
				endpoint = pup;
			}
		}
		//右边界
		//若终点在右边界以右
		if (endpointcode.Cr == 1)
		{
			pright.x = xmax;
			pright.y = CalculatePositionY(startpoint, endpoint, xmax);
			//交点在右边框延长线上,线段不可见
			if (pright.y > ymax || pright.y < ymin)
			{
				continue;
			}
			else	//更新结点
			{
				endpoint = pright;
			}
		}
		//下边界
		//若起点在下边界以下
		if (startpointcode.Cb == 1)
		{
			pdown.x = CalculatePositionX(startpoint, endpoint, ymin);
			pdown.y = ymin;

			//交点在上边框延长线上,线段不可见
			if (pdown.x > xmax || pdown.x < xmin)
			{
				continue;
			}
			else	//更新结点
			{
				startpoint = pdown;
			}
		}
		//若终点在下边界以下
		if (endpointcode.Cb == 1)
		{
			pdown.x = CalculatePositionX(startpoint, endpoint, ymin);
			pdown.y = ymin;

			//交点在上边框延长线上,线段不可见
			if (pdown.x > xmax || pdown.x < xmin)
			{
				continue;
			}
			else	//更新结点
			{
				endpoint = pdown;
			}
		}
		DrawLine(startpoint, endpoint);


	}
	return;
}


// 线段编码
linecode CCGLabView::LineEncode(int xmax, int xmin, int ymax, int ymin, CPoint point)
{
	// TODO: 在此处添加实现代码.
	linecode pointcode;
	if (point.y > ymax) pointcode.Ct = 1;
	else pointcode.Ct = 0;
	if (point.y < ymin) pointcode.Cb = 1;
	else pointcode.Cb = 0;
	if (point.x > xmax) pointcode.Cr = 1;
	else pointcode.Cr = 0;
	if (point.x < xmin) pointcode.Cl = 1;
	else pointcode.Cl = 0;
	return pointcode;
}


// 计算交点Y值
int CCGLabView::CalculatePositionY(CPoint startpoint, CPoint endpoint, int x)
{
	// TODO: 在此处添加实现代码.
	float dy = startpoint.y - endpoint.y;
	float dx = startpoint.x - endpoint.x;
	float k = dy / dx;
	return (int)(k*(x-startpoint.x)+startpoint.y);
}


// 计算交点X值
int CCGLabView::CalculatePositionX(CPoint startpoint, CPoint endpoint, int y)
{
	// TODO: 在此处添加实现代码.
	float dy = startpoint.y - endpoint.y;
	float dx = startpoint.x - endpoint.x;
	float m = dx / dy;
	return (int)(m*(y - startpoint.y) + startpoint.x);
}


// 绘制动画小球
void CCGLabView::DrawBall()
{
	// TODO: 在此处添加实现代码.
	CDC *pDC = GetWindowDC();
	CBrush newBrush;
	colorselect = colorselect % 3;
	newBrush.CreateSolidBrush(colors[colorselect]);
	pDC->SelectObject(newBrush);
	for (int i = 0; i < cartoon_ballNum; i++)
	{
		pDC->Ellipse(cartoon_ball[i].x - 10, cartoon_ball[i].y - 10, cartoon_ball[i].x + 10, cartoon_ball[i].y + 10);
	}
	return;
}
