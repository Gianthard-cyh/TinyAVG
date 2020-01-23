#include "TinyAVG.h"

// ���캯��.
TinyAVG::TinyAVG() :
	m_hwnd(NULL),
	m_pD2DFactory(NULL),
	m_pD2DRenderTarget(NULL),
	m_pD2DBlackBrush(NULL),
	m_pWICFactory(NULL)
{
	InitializeXMLScript();
	m_DialogTitle = L"�Ի������"; 
	m_bIsWaitingForSpace = true;
}

// ��������.
TinyAVG::~TinyAVG()
{
	SAFE_RELEASE(m_pD2DFactory);
	SAFE_RELEASE(m_pD2DRenderTarget);
	SAFE_RELEASE(m_pWICFactory);
	SAFE_RELEASE(m_pD2DBlackBrush);
}

HRESULT TinyAVG::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// ������D2DFactory��.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	if (SUCCEEDED(hr)) {
		// ����WIC Factory.
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&m_pWICFactory)
		);
	}

	if (SUCCEEDED(hr)) {
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			(IUnknown**)&m_pDWriteFactory
		);
	}

	return hr;
}

HRESULT TinyAVG::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pD2DRenderTarget) {
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// ������D2DRenderTarget��.
		hr = m_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pD2DRenderTarget
		);
	}

	if (!m_pD2DBlackBrush) {
		if (SUCCEEDED(hr)) {
			// ��������ɫ��ˢ��.
			m_pD2DRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black),
				&m_pD2DBlackBrush
			);
		}
	}

	if (!m_pD2DWhiteBrush) {
		if (SUCCEEDED(hr)) {
			// ��������ɫ��ˢ��.
			m_pD2DRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&m_pD2DWhiteBrush
			);
		}
	}

	if (!m_pD2DOrangeBrush) {
		if (SUCCEEDED(hr)) {
			// ��������ɫ��ˢ��.
			m_pD2DRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Orange),
				&m_pD2DOrangeBrush
			);
		}
	}

	if (!m_pD2DDialogBrush) {
		if (SUCCEEDED(hr)) {
			// �������Ի���ˢ��.
			m_pD2DRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black, 0.7f),
				&m_pD2DDialogBrush
			);
		}
	}

	if (!m_pDwriteTextFormatDialogContent) {
		m_pDWriteFactory->CreateTextFormat(
			L"΢���ź�",
			NULL,
			DWRITE_FONT_WEIGHT_BOLD,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			25.0f,
			L"zh-cn",
			&m_pDwriteTextFormatDialogTitle
		);

		if (SUCCEEDED(hr)) {
			hr = m_pDwriteTextFormatDialogTitle->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		}

		if (SUCCEEDED(hr)) {
			hr = m_pDwriteTextFormatDialogTitle->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		}

		if (SUCCEEDED(hr)) {
			hr = m_pDwriteTextFormatDialogTitle->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
		}
	}

	if (!m_pDwriteTextFormatDialogContent) {
		m_pDWriteFactory->CreateTextFormat(
			L"΢���ź�",
			NULL,
			DWRITE_FONT_WEIGHT_LIGHT,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20.0f,
			L"zh-cn",
			&m_pDwriteTextFormatDialogContent
		);

		if (SUCCEEDED(hr)) {
			hr = m_pDwriteTextFormatDialogContent->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		}

		if (SUCCEEDED(hr)) {
			hr = m_pDwriteTextFormatDialogContent->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		}

		if (SUCCEEDED(hr)) {
			hr = m_pDwriteTextFormatDialogContent->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
		}
	}

	if (!m_Bitmap_bg.pD2DBitmap) {
		if (SUCCEEDED(hr)) {
			LoadTinyAVGBitmap(m_Bitmap_bg);
		}
	}

	for (int i = 0; i < m_vImages.size(); i++) {
		if (!m_vImages[i].pD2DBitmap) {
			LoadTinyAVGBitmap(m_vImages[i]);
		}
	}

	return hr;
}

void TinyAVG::DiscardDeviceResources()
{
	// �ͷš�D2DRenderTarget��.
	SAFE_RELEASE(m_pD2DRenderTarget);

	// �ͷš�D2DBlackBrush��.
	SAFE_RELEASE(m_pD2DBlackBrush);
}

void TinyAVG::InitializeXMLScript()
{
	tinyxml2::XMLError XMLerr;

	// ����XML�ĵ�.
	XMLerr = m_XMLDoc.LoadFile("Script.xml");

	if (XMLerr == tinyxml2::XML_ERROR_FILE_NOT_FOUND) {
		// ��XML�ĵ������ڣ��򵯳���ʾ���ں�ֱ���˳�������-1.
		MessageBox(NULL, "����XML�ű�ʧ�ܣ�XML�ļ�������", "����", NULL);

		exit(-1);
	}

	m_pXMLElementNow = m_XMLDoc.FirstChildElement("script")->FirstChildElement("block")->FirstChildElement();
	XMLScriptProcesser(m_pXMLElementNow);
}

void TinyAVG::XMLScriptProcesser(tinyxml2::XMLElement* pXMLElememt)
{
	if (!pXMLElememt)
		exit(-2);

	std::string strValue = pXMLElememt->Value();

	if (strValue == "block") {
		m_pXMLElementNow = pXMLElememt->FirstChildElement();
		XMLScriptProcesser(m_pXMLElementNow);
	}
	else if (strValue == "setbg" || strValue == "setbackground") {
		XMLScriptOnbg(pXMLElememt);
	}
	else if (strValue == "settitle" || strValue == "setT") {
		XMLScriptOntitle(pXMLElememt);
	}
	else if (strValue == "setcontent" || strValue == "setC") {
		XMLScriptOncontent(pXMLElememt);
	}
	else if (strValue == "pause") {
		XMLScriptOnpause(pXMLElememt);
	}
	else if (strValue == "img") {
		XMLScriptOnimg(pXMLElememt);
	}
	else if (strValue == "clr") {
		XMLScriptOnclr(pXMLElememt);
	}

}

void TinyAVG::XMLScriptOnbg(tinyxml2::XMLElement* pXMLElememt)
{
	// ������λͼ·����ΪsetbgԪ�ص�uri����.
	m_Bitmap_bg.uri = pXMLElememt->Attribute("uri");

	// �ͷű���λͼD2DBitmap�����Ա���Ⱦǰ����CreateDeviceResources()��
	// ����D2DBitmapָ��.
	SAFE_RELEASE(m_Bitmap_bg.pD2DBitmap);

	// ����ǰXMLԪ����Ϊ��ǰԪ�ص���һ���ֵ�Ԫ��.(setbgΪ������������)
	m_pXMLElementNow = pXMLElememt->NextSiblingElement();

	// ����������ǰԪ�ص���һ���ֵ�Ԫ��.(setbgΪ������������)
	XMLScriptProcesser(m_pXMLElementNow);
}

void TinyAVG::XMLScriptOntitle(tinyxml2::XMLElement* pXMLElememt)
{
	// T2W()ת����ʹ��ǰ�������USES_CONVERSION��
	USES_CONVERSION;

	m_DialogTitle = T2W(pXMLElememt->Attribute("content"));

	m_pXMLElementNow = pXMLElememt->NextSiblingElement();

	XMLScriptProcesser(m_pXMLElementNow);
}

void TinyAVG::XMLScriptOncontent(tinyxml2::XMLElement* pXMLElememt)
{
	// T2W()ת����ʹ��ǰ�������USES_CONVERSION��
	USES_CONVERSION;

	m_DialogContent = T2W(pXMLElememt->Attribute("content"));

	m_pXMLElementNow = pXMLElememt->NextSiblingElement();

	XMLScriptProcesser(m_pXMLElementNow);
}

void TinyAVG::XMLScriptOnpause(tinyxml2::XMLElement* pXMLElememt)
{
	m_bIsWaitingForSpace = true;
}

void TinyAVG::XMLScriptOnimg(tinyxml2::XMLElement* pXMLElememt)
{
	TinyAVG_Bitmap Temp;

	Temp.uri = pXMLElememt->Attribute("uri");

	Temp.Position = D2D1::Point2F(
		pXMLElememt->FloatAttribute("x"),
		pXMLElememt->FloatAttribute("y")
	);

	Temp.pD2DBitmap = NULL;

	m_vImages.push_back(Temp);

	m_pXMLElementNow = pXMLElememt->NextSiblingElement();

	XMLScriptProcesser(m_pXMLElementNow);
}

void TinyAVG::XMLScriptOnclr(tinyxml2::XMLElement* pXMLElememt)
{
	// ��Ⱦimgλͼ.
	for (int i = 0; i < m_vImages.size(); i++) {
		if (m_vImages[i].pD2DBitmap) {
			SAFE_RELEASE(m_vImages[i].pD2DBitmap);
		}
	}

	m_vImages.clear();
}

HRESULT TinyAVG::OnRender()
{
	HRESULT hr = S_OK;
	//��Ӧ��WM_PAINT����Ϣ�Լ���Ϸѭ�����ã��Դ������ݽ�����Ⱦ.
	hr = CreateDeviceResources();

	if (SUCCEEDED(hr)) {
		m_pD2DRenderTarget->BeginDraw();

		m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pD2DRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		// ��ȡRenderTarget��С.
		D2D1_SIZE_F rtSize = m_pD2DRenderTarget->GetSize();

		// ��Ⱦ����λͼ .
		if (m_Bitmap_bg.pD2DBitmap) {
			DrawTinyAVGBitmap(m_Bitmap_bg);
		}

		// ��Ⱦimgλͼ.
		for (int i = 0; i < m_vImages.size(); i++) {
			if (m_vImages[i].pD2DBitmap) {
				DrawTinyAVGBitmap(m_vImages[i]);
			}
		}

		// �Ի���ĸ߶�.
		const float Dialog_Height = 230.0;

		// �Ի��������ܵĿ�϶.
		const float Dialog_Space = 40.0;

		// ����Ի�����ƾ�������.
		D2D1_RECT_F DialogRectF = D2D1::RectF(
			Dialog_Space,
			rtSize.height - Dialog_Space - Dialog_Height,
			rtSize.width - Dialog_Space,
			rtSize.height - Dialog_Space
		);

		// ��Ⱦ�Ի��򱳾�.
		m_pD2DRenderTarget->FillRectangle(
			DialogRectF,
			m_pD2DDialogBrush
		);

		// ����Ի�������ı�Ŀ�����.
		D2D1_RECT_F DialogTitleRectF = D2D1::RectF(
			DialogRectF.left + 20,
			DialogRectF.top + 20,
			DialogRectF.right - 20,
			DialogRectF.bottom - 20
		);

		// ��Ⱦ�Ի������.
		m_pD2DRenderTarget->DrawTextA(
			m_DialogTitle.c_str(),
			m_DialogTitle.size(),
			m_pDwriteTextFormatDialogTitle,
			DialogTitleRectF,
			m_pD2DOrangeBrush
		);

		// ����Ի��������ı�Ŀ�����.
		D2D1_RECT_F DialogContentRectF = D2D1::RectF(
			DialogTitleRectF.left,
			DialogTitleRectF.top + 40,
			DialogTitleRectF.right,
			DialogTitleRectF.bottom
		);

		// ��Ⱦ�Ի�������.
		m_pD2DRenderTarget->DrawTextA(
			m_DialogContent.c_str(),
			m_DialogContent.size(),
			m_pDwriteTextFormatDialogContent,
			DialogContentRectF,
			m_pD2DWhiteBrush
		);

		m_pD2DRenderTarget->EndDraw();
	}

	return hr;
}

void TinyAVG::OnResize(UINT width, UINT height)
{
	// ��Ӧ��WM_RESIZE����Ϣ����D2DRenderTarget����ָ����С.
	if (m_pD2DRenderTarget) {
		m_pD2DRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

HRESULT TinyAVG::LoadTinyAVGBitmap(TinyAVG_Bitmap& TinyAVGBitmap)
{
	HRESULT hr = S_OK;

	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICFormatConverter* pConverter = NULL;

	USES_CONVERSION;

	hr = m_pWICFactory->CreateDecoderFromFilename(
		T2W(TinyAVGBitmap.uri),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr)) {
		// ����֡.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr)) {
		hr = m_pWICFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr)) {
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr)) {
		hr = m_pD2DRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&TinyAVGBitmap.pD2DBitmap
		);
	}

	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pSource);
	SAFE_RELEASE(pConverter);

	return hr;
}

void TinyAVG::DrawTinyAVGBitmap(TinyAVG_Bitmap& TinyAVGBitmap)
{
	D2D1_SIZE_F size = TinyAVGBitmap.pD2DBitmap->GetSize();

	m_pD2DRenderTarget->DrawBitmap(
		TinyAVGBitmap.pD2DBitmap,
		D2D1::RectF(
			TinyAVGBitmap.Position.x,
			TinyAVGBitmap.Position.y,
			TinyAVGBitmap.Position.x + size.width,
			TinyAVGBitmap.Position.x + size.height
		),
		TinyAVGBitmap.opacity,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
	);
}

void TinyAVG::OnSpaceKey()
{
	m_bIsWaitingForSpace = false;

	m_pXMLElementNow = m_pXMLElementNow->NextSiblingElement();

	XMLScriptProcesser(m_pXMLElementNow);
}
