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

//TinyAVG_Bitmap�ṹ

struct TinyAVG_Bitmap {
	ID2D1Bitmap* pD2DBitmap = NULL;
	D2D1_POINT_2F Position = D2D1::Point2F(0.0,0.0);
	PCSTR uri = NULL;
	float opacity = 1.0;
};

class TinyAVG {
public:
	//----------����ṹ���֡�----------//

		// ���캯��.
	TinyAVG();

		// ��������.
	~TinyAVG();

	//----------��Win32 API���֡�----------//

		// Ϊ��m_hwnd����ֵ��
	void Sethwnd(HWND hwnd) { m_hwnd = hwnd; }

	//----------����Դ���֡�----------//

		// ��ʼ���豸�޹���Դ.
	HRESULT CreateDeviceIndependentResources();

		// ��ʼ�����豸�����Դ��.
	HRESULT CreateDeviceResources();

		// �ͷš��豸�����Դ��.
	void DiscardDeviceResources();

	//----------��XML�ű����֡�----------//

		// ��ʼ��XML�ű�ģ��.
	void InitializeXMLScript();

		// XMLScript������.
	void XMLScriptProcesser(tinyxml2::XMLElement* pXMLElememt);
	
		// ��Ӧsetbg����.
	void XMLScriptOnbg(tinyxml2::XMLElement* pXMLElememt);

		// ��Ӧsettitle����.
	void XMLScriptOntitle(tinyxml2::XMLElement* pXMLElememt);

		// ��Ӧsetcontent����.
	void XMLScriptOncontent(tinyxml2::XMLElement* pXMLElememt);

		// ��Ӧpause����.
	void XMLScriptOnpause(tinyxml2::XMLElement* pXMLElememt);

		// ��Ӧimg����.
	void XMLScriptOnimg(tinyxml2::XMLElement* pXMLElememt);
	
		// ��Ӧclr����.
	void XMLScriptOnclr(tinyxml2::XMLElement* pXMLElememt);

	//----------���¼���Ӧ���֡�----------//

		// ��Ⱦ��������ӦWM_PAINT��Ϣ�Լ���Ϸѭ����Ⱦ��.
	HRESULT OnRender();

		// �ı䡾Rendertarget���Ĵ�С��������ӦWM_RESIZE��Ϣ��.
	void OnResize(
		UINT width,
		UINT height
	);

		// ���ء�TinyAVG_Bitmap���ṹ��.
	HRESULT LoadTinyAVGBitmap(TinyAVG_Bitmap& TinyAVGBitmap);

		// ���ơ�TinyAVG_Bitmap���ṹ��.
	void DrawTinyAVGBitmap(TinyAVG_Bitmap& TinyAVGBitmap);

	void OnSpaceKey();

private:
	// ����ʵ�����.
	HWND m_hwnd;

	// D2D����ָ��.
	ID2D1Factory* m_pD2DFactory;

	// D2DRenderTargetָ��.
	ID2D1HwndRenderTarget* m_pD2DRenderTarget;

	// WIC Factoryָ��.
	IWICImagingFactory* m_pWICFactory;

	// D2D��ɫ��ˢָ��.
	ID2D1SolidColorBrush* m_pD2DBlackBrush;

	// D2D��ɫ��ˢָ��.
	ID2D1SolidColorBrush* m_pD2DWhiteBrush;

	// D2D��ɫ��ˢָ��.
	ID2D1SolidColorBrush* m_pD2DOrangeBrush;

	// D2D�Ի���ˢָ��.
	ID2D1SolidColorBrush* m_pD2DDialogBrush;

	// ����λͼ�ṹ��.
	TinyAVG_Bitmap m_Bitmap_bg;

	// ���ڱ�ʶ��ǰ�Ƿ����ڵȴ�����space��.
	bool m_bIsWaitingForSpace;

	// �Ի��������ı�.
	wstring m_DialogContent;

	// �Ի�������ı�.
	wstring m_DialogTitle;

	// ͼƬ��̬����.
	vector<TinyAVG_Bitmap> m_vImages;

	// DirectWrite����ָ��.
	IDWriteFactory* m_pDWriteFactory;

	// �Ի�������DirectWriteTextFormatָ��.
	IDWriteTextFormat* m_pDwriteTextFormatDialogContent;

	// �Ի������DirectWriteTextFormatָ��.
	IDWriteTextFormat* m_pDwriteTextFormatDialogTitle;

	// XML�ĵ�����.
	tinyxml2::XMLDocument m_XMLDoc;

	//��ǰXML�ڵ�
	tinyxml2::XMLElement* m_pXMLElementNow;

};