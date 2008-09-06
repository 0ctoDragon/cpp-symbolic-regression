// GraphView.cpp : implementation file
//

#include "stdafx.h"
#include "SymbolRegress.h"
#include "GraphView.h"
#include "SymbolRegressDoc.h"
#include ".\graphview.h"
#include "EvaluatingFunction.h"
#include "DNAStatement.h"

// GraphView

IMPLEMENT_DYNCREATE(GraphView, CView)

GraphView::GraphView()
{
}

GraphView::~GraphView()
{
}

BEGIN_MESSAGE_MAP(GraphView, CView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// GraphView drawing

void GraphView::OnDraw(CDC* pDC)
{
	BOOL bResult = wglMakeCurrent (pDC->m_hDC, m_hrc);
	if (!bResult)
	{
		TRACE("wglMakeCurrent Failed %x\r\n", GetLastError() ) ;
	}

	    
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	    
	::glPushMatrix();
	::glTranslatef(0.0f, 0.0f, 0.0f);                         
		RenderScene() ;
	::glPopMatrix();

	::glFinish();


	if ( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc() ) )
		{

		} 
}


// GraphView diagnostics

#ifdef _DEBUG
void GraphView::AssertValid() const
{
	CView::AssertValid();
}

void GraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSymbolRegressDoc* GraphView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSymbolRegressDoc)));
	return (CSymbolRegressDoc*)m_pDocument;
}
#endif //_DEBUG


// GraphView message handlers
/****************************************
OpenGL construction Methods
****************************************/
int GraphView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	InitializeOpenGL();
	DocPtr = GetDocument();
	DocPtr->m_Graph = this;
	return 0;
}

BOOL GraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

BOOL GraphView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
	
}

void GraphView::OnDestroy()
{
	CView::OnDestroy();
	wglMakeCurrent(NULL, NULL) ;
	if (m_hrc){
		wglDeleteContext(m_hrc) ;
		m_hrc = NULL ;
	}
}

void GraphView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CView::OnSize(nType, cx, cy);
	WinWidth = (GLfloat) cx;
	WinHeight = (GLfloat) cy;

	CClientDC dc(this);
	BOOL bResult = wglMakeCurrent (dc.m_hDC, m_hrc);
	if (!bResult){
		TRACE("wglMakeCurrent Failed %x\r\n", GetLastError() ) ;
		return ;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	        
	glOrtho(0.0f,(GLdouble) cx, (GLdouble) -cy, 0.0f, 0.0, 0.0 /*0.1f, 100.0f*/);
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_MODELVIEW);                         // Select The Modelview Matrix
	glLoadIdentity();                                   // Reset The Modelview Matrix           
}



BOOL GraphView::SetupPixelFormat( void ){


  static PIXELFORMATDESCRIPTOR pfd = 
    {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,               // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
	//        32,                             // 32-bit z-buffer
		16, // NOTE: better performance with 16-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( 0 == (pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) )
		{
		return FALSE;
		}

	if ( FALSE == ::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) )
		{
		return FALSE;
		}
	return TRUE;
}

BOOL GraphView::InitializeOpenGL(){
    this->m_pDC = new CClientDC(this) ;
    if ( NULL == m_pDC ) 
    {
        TRACE("failure to get DC") ;
        return false;
    }
    if (!SetupPixelFormat())
    {
        return false;
    }

    if ( 0 == (m_hrc = ::wglCreateContext( m_pDC->GetSafeHdc() ) ) )
        {
        return false;
        }

    if ( FALSE == ::wglMakeCurrent( m_pDC->GetSafeHdc(), m_hrc ) )
        {
        
            return false;
        }   


    ::glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
    ::glClearDepth( 1.0f );
    ::glEnable( GL_DEPTH_TEST );
    ::glEnable(GL_LIGHTING) ;
    ::glEnable(GL_LIGHT0) ;
    ::glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE) ;
    ::glEnable(GL_COLOR_MATERIAL) ;

    return true;
}

/******************************
Drawing Routines
*******************************/
void GraphView::RenderScene(){

	GLfloat Scalefactor = 0.2f;
	glPushMatrix();
	glScalef(Scalefactor, Scalefactor, Scalefactor);

	try{
		/*glColor3f(0.7f, 0.6f, 0.2f);
		DocPtr->m_Population[DocPtr->m_CurrentIndividual]->draw(60, DocPtr->RangeMin, DocPtr->RangeMax);*/

		glColor3f(0.0f, 0.0f, 1.0f);
		DocPtr->m_Population[DocPtr->m_BestIndex]->draw(60, DocPtr->RangeMin, DocPtr->RangeMax);
		
		if(DocPtr->EvalFunc)
			DocPtr->EvalFunc->draw();
		
	}
	catch(CString Msg){
		
		//CString gh("\r\n Unable to draw Function");
		//Msg += gh;
		//AfxMessageBox(Msg);
	}
	glPopMatrix();
}

   