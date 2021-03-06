
// CGLabView.h: CCGLabView 类的接口
//

#include <vector>
using namespace std;
#include <stack>
using std::stack;

#include "math.h"
const float PI = 3.1415926;

//线段编码位:4bit,上下左右
struct linecode
{
	unsigned int Ct : 1;
	unsigned int Cb : 1;
	unsigned int Cl : 1;
	unsigned int Cr : 1;
};

#pragma once


class CCGLabView : public CView
{
protected: // 仅从序列化创建
	CCGLabView();
	DECLARE_DYNCREATE(CCGLabView)

// 特性
public:
	CCGLabDoc* GetDocument() const;
	int m_pointNum;	//输入点个数
	int graphtype;	//图形类型：1：多边形
	vector <CPoint> m_point;	//存储输入点

	int graphfilltype;	//图形填充类型：1：简单种子4联通
	CPoint pSeed;		//填充种子
	stack <CPoint> m_stk;	//存储种子填充过程的点

	int graphtranstype;	//图形变换类型：1：平移；2：X轴对称；3、Y轴对称；4、原点对称；5、旋转；6、缩放
	bool graphsemmtryflag;//是否进行图形对称
	float up_distance;	//向上平移距离
	float down_distance;	//向下平移距离
	float left_distance;	//向左平移距离
	float right_distance;	//向右平移距离
	bool key_down[2];	//down键按下，抬起
	bool key_left[2];	//left键按下，抬起
	bool key_right[2];	//right键按下，抬起
	bool key_up[2];	//up键按下，抬起
	CPoint pSemmetry;	//对称点，X轴，Y轴

	bool graphrotationflag;	//是否进行图形旋转
	CPoint pRotation;	//旋转点
	float rotationangle;//旋转角度

	bool graphzoomflag;	//是否进行图形缩放
	CPoint pZoom;		//缩放点
	float zoomproportionx;//x缩放比
	float zoomproportiony;//y缩放比

	CPoint pXYSystem;	//坐标系原点

	vector <CPoint> cut_point;	//裁剪框对角点+待裁剪线段
	int cut_pointNum;			//裁剪框点数
	bool graphcutflag;	//是否进行图形裁剪
	int graphcuttype;	//图形裁剪类型,1:编码裁剪

	bool cartoonflag;	//是否进行动画
	int cartoontype;	//动画类型
	vector <CPoint> cartoon_ball;	//运动小球
	int cartoon_ballNum;	//运动小球个数
	vector <int> xadd;		//x方向增量
	vector <int> yadd;		//y方向增量
	

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCGLabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawpoly();
	afx_msg void OnSeedfill4();
	afx_msg void OnTranslation();
	afx_msg void OnSemmetryX();
	afx_msg void OnSemmetryY();
	afx_msg void OnSemmetryPoint();
	afx_msg void OnRotation();
	afx_msg void OnZoom();
	afx_msg void OnCohenCut();
	afx_msg void OnCartoon1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnInitall();
	//绘制多边形
	void DrawPoly();
	// 绘制直线
	void DrawLine(CPoint p1, CPoint p2);
	// 初始化+清屏
	void Initallthing();
	// 多边形填充
	void PolyFill();
	void SeedFill4(CPoint pSeed, int fill_color, int bound_color);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//坐标系绘制
	void DrawXYSystem();
	// 矩阵变换
	void MatrixTrans();
	// 绘制裁剪边框
	void DrawCutEdge();
	// 绘制矩形裁剪边框
	void DrawCutRec();
	// 绘制裁剪线段
	void DrawCutLine();
	// 线段编码
	linecode LineEncode(int xmax,int xmin,int ymax,int ymin, CPoint point);
	// 计算交点Y值
	int CalculatePositionY(CPoint startpoint,CPoint endpoint,int x);
	// 计算交点X值
	int CalculatePositionX(CPoint startpoint, CPoint endpoint, int y);
	// 绘制动画小球
	void DrawBall();
};

#ifndef _DEBUG  // CGLabView.cpp 中的调试版本
inline CCGLabDoc* CCGLabView::GetDocument() const
   { return reinterpret_cast<CCGLabDoc*>(m_pDocument); }
#endif

