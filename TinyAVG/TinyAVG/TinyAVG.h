#pragma once
#include<Windows.h>
#include<D2D1.h>
#include<d2d1helper.h>
#include<wincodec.h>
#include<wincodecsdk.h>
#include<atlconv.h>
#include<dwrite.h>
#include"tinyxml2.h"
#include<string>
#include<vector>
#include<fstream>

using std::ofstream;
using std::wstring;
using std::vector;

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 810

#define SAFE_RELEASE(P) if(P){P->Release() ; P = NULL ;}

//TinyAVG_Bitmap结构

struct TinyAVG_Bitmap {
	ID2D1Bitmap* pD2DBitmap = NULL;
	D2D1_POINT_2F Position = D2D1::Point2F(0.0,0.0);
	PCSTR uri = NULL;
	float opacity = 1.0;
};

class TinyAVG {
public:
	//----------【类结构部分】----------//

		// 构造函数.
	TinyAVG();

		// 析构函数.
	~TinyAVG();

	//----------【Win32 API部分】----------//

		// 为【m_hwnd】赋值。
	void Sethwnd(HWND hwnd) { m_hwnd = hwnd; }

	//----------【资源部分】----------//

		// 初始化设备无关资源.
	HRESULT CreateDeviceIndependentResources();

		// 初始化【设备相关资源】.
	HRESULT CreateDeviceResources();

		// 释放【设备相关资源】.
	void DiscardDeviceResources();

	//----------【XML脚本部分】----------//

		// 初始化XML脚本模块.
	void InitializeXMLScript();

		// XMLScript解码器.
	void XMLScriptProcesser(tinyxml2::XMLElement* pXMLElememt);
	
		// 响应setbg命令.
	void XMLScriptOnbg(tinyxml2::XMLElement* pXMLElememt);

		// 响应settitle命令.
	void XMLScriptOntitle(tinyxml2::XMLElement* pXMLElememt);

		// 响应setcontent命令.
	void XMLScriptOncontent(tinyxml2::XMLElement* pXMLElememt);

		// 响应pause命令.
	void XMLScriptOnpause(tinyxml2::XMLElement* pXMLElememt);

		// 响应img命令.
	void XMLScriptOnimg(tinyxml2::XMLElement* pXMLElememt);
	
		// 响应clr命令.
	void XMLScriptOnclr(tinyxml2::XMLElement* pXMLElememt);

	//----------【事件响应部分】----------//

		// 渲染（用于响应WM_PAINT消息以及游戏循环渲染）.
	HRESULT OnRender();

		// 改变【Rendertarget】的大小（用于响应WM_RESIZE消息）.
	void OnResize(
		UINT width,
		UINT height
	);

		// 加载【TinyAVG_Bitmap】结构体.
	HRESULT LoadTinyAVGBitmap(TinyAVG_Bitmap& TinyAVGBitmap);

		// 绘制【TinyAVG_Bitmap】结构体.
	void DrawTinyAVGBitmap(TinyAVG_Bitmap& TinyAVGBitmap);

	void OnSpaceKey();

private:
	// 窗口实例句柄.
	HWND m_hwnd;

	// D2D工厂指针.
	ID2D1Factory* m_pD2DFactory;

	// D2DRenderTarget指针.
	ID2D1HwndRenderTarget* m_pD2DRenderTarget;

	// WIC Factory指针.
	IWICImagingFactory* m_pWICFactory;

	// D2D黑色画刷指针.
	ID2D1SolidColorBrush* m_pD2DBlackBrush;

	// D2D白色画刷指针.
	ID2D1SolidColorBrush* m_pD2DWhiteBrush;

	// D2D橙色画刷指针.
	ID2D1SolidColorBrush* m_pD2DOrangeBrush;

	// D2D对话框画刷指针.
	ID2D1SolidColorBrush* m_pD2DDialogBrush;

	// 背景位图结构体.
	TinyAVG_Bitmap m_Bitmap_bg;

	// 用于标识当前是否正在等待按下space键.
	bool m_bIsWaitingForSpace;

	// 对话框内容文本.
	wstring m_DialogContent;

	// 对话框标题文本.
	wstring m_DialogTitle;

	// 图片动态数组.
	vector<TinyAVG_Bitmap> m_vImages;

	// DirectWrite工厂指针.
	IDWriteFactory* m_pDWriteFactory;

	// 对话框内容DirectWriteTextFormat指针.
	IDWriteTextFormat* m_pDwriteTextFormatDialogContent;

	// 对话框标题DirectWriteTextFormat指针.
	IDWriteTextFormat* m_pDwriteTextFormatDialogTitle;

	// XML文档对象.
	tinyxml2::XMLDocument m_XMLDoc;

	//当前XML节点
	tinyxml2::XMLElement* m_pXMLElementNow;

};