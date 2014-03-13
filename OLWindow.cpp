#include "stdafx.h"
#if _WIN32
#include "OLWindow.h"
#pragma comment(lib, "Uxtheme.lib")
#include <Uxtheme.h>
namespace lang
{
    BOOL SetClientSize(HWND hWnd, int width, int height);
    LPCWSTR OLWindow::classname = _T("OLWindow");
    LPCWSTR Button::classname = _T("BUTTON");
    LPCSTR OLWindow::classnameA = "OLWindow";
    LPCSTR Button::classnameA = "BUTTON";
    //GWLP_USERDATA�g���΂����I�I�I�I�I�I�I�I�I�I�I�I�I
    std::map<HWND,OLWindow*> Button::windowmap;
    OLWindow::OLWindow(){}
    OLWindow::OLWindow(LPCWSTR title)
    {
        ctor(title, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,classname);
    }

    OLWindow::OLWindow(LPCWSTR title, int nWidth,int nHeight)
    {
        ctor(title, CW_USEDEFAULT, CW_USEDEFAULT, nWidth, nHeight,classname);
    }
    OLWindow::OLWindow(LPCWSTR title, int X,int Y,int nWidth,int nHeight)
    {
        ctor(title, X, Y, nWidth, nHeight,classname);
    }
    OLWindow::OLWindow(LPCSTR title)
    {
        ctor(title, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,classname);
    }

    OLWindow::OLWindow(LPCSTR title, int nWidth,int nHeight)
    {
        ctor(title, CW_USEDEFAULT, CW_USEDEFAULT, nWidth, nHeight,classname);
    }
    OLWindow::OLWindow(LPCSTR title, int X,int Y,int nWidth,int nHeight)
    {
        ctor(title, X, Y, nWidth, nHeight,classname);
    }
    LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        static HWND hButton;
        auto hInst = GetModuleHandle(NULL);
        switch( msg )
        {
        case WM_CREATE:			// �E�B���h�E���쐬���ꂽ�Ƃ�
            // �v�b�V���{�^�����쐬����
            hButton = CreateWindow(
                _T("BUTTON"),                            // �E�B���h�E�N���X��
                _T("OK"),                                // �L���v�V����
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,   // �X�^�C���w��
                20, 20,                                  // ���W
                60, 40,                                  // �T�C�Y
                hWnd,                                    // �e�E�B���h�E�̃n���h��
                NULL,                                    // ���j���[�n���h��
                hInst,                                 // �C���X�^���X�n���h��
                NULL                                     // ���̑��̍쐬�f�[�^
                );
            return 0;
        case WM_DESTROY:  // �E�B���h�E��j������Ƃ�
            PostQuitMessage( 0 );
            return 0;
        }

        // ���̃��b�Z�[�W�́A�f�t�H���g�̏������s��
        return DefWindowProc( hWnd, msg, wp, lp );
    }
    /*WNDPROC getproc()
    {
    return OLWindow::WndProc;
    }*/
    void OLWindow::ctor(LPCWSTR title, int X,int Y,int nWidth,int nHeight, LPCWSTR ClassName,long style,HWND parent)
    {
        hInst = GetModuleHandle(NULL);

        // �E�B���h�E���쐬����
        hWnd = CreateWindow(
            OLWindow::classname,      // �E�B���h�E�N���X��
            /*_T*/(title),  // �^�C�g���o�[�ɕ\�����镶����
            style,   // �E�B���h�E�̎��
            X,         // �E�B���h�E��\������ʒu�iX���W�j
            Y,         // �E�B���h�E��\������ʒu�iY���W�j
            nWidth,         // �E�B���h�E�̕�
            nHeight,         // �E�B���h�E�̍���
            parent,                  // �e�E�B���h�E�̃E�B���h�E�n���h��
            NULL,                  // ���j���[�n���h��
            hInst,                 // �C���X�^���X�n���h��
            NULL                   // ���̑��̍쐬�f�[�^
            );
        RECT rect, wrect;
        GetClientRect(hWnd, &rect);
        GetWindowRect(hWnd, &wrect);
        if(rect.bottom - nHeight)
        {
            wrect.bottom += rect.bottom - nHeight;
        }
        if(rect.right - nHeight)
        {
            wrect.right += rect.right - nWidth;
        }
        AdjustWindowRectEx(&wrect, GetWindowLong(hWnd,GWL_STYLE),(BOOL)GetMenu(hWnd),GetWindowLong(hWnd,GWL_EXSTYLE));
        if( hWnd == NULL ){ throw GetLastError(); }
        OLWindow::windowmap[hWnd] = this;
    }
    void OLWindow::ctor(LPCSTR title, int X,int Y,int nWidth,int nHeight, LPCWSTR ClassName,long style,HWND parent)
    {
        hInst = GetModuleHandle(NULL);

        // �E�B���h�E���쐬����
        hWnd = CreateWindowA(
            OLWindow::classnameA,      // �E�B���h�E�N���X��
            title,  // �^�C�g���o�[�ɕ\�����镶����
            style,   // �E�B���h�E�̎��
            X,         // �E�B���h�E��\������ʒu�iX���W�j
            Y,         // �E�B���h�E��\������ʒu�iY���W�j
            nWidth,         // �E�B���h�E�̕�
            nHeight,         // �E�B���h�E�̍���
            parent,                  // �e�E�B���h�E�̃E�B���h�E�n���h��
            NULL,                  // ���j���[�n���h��
            hInst,                 // �C���X�^���X�n���h��
            NULL                   // ���̑��̍쐬�f�[�^
            );
        if( hWnd == NULL ){ throw GetLastError(); }
        SetClientSize(hWnd, nWidth, nHeight);
        //RECT rect, wrect;\
        GetClientRect(hWnd, &rect);\
        GetWindowRect(hWnd, &wrect);\
        if(rect.bottom - nHeight)\
        {\
            wrect.bottom -= rect.bottom - nHeight;\
        }\
        if(rect.right - nWidth)\
        {\
            wrect.right -= rect.right - nWidth;\
        }\
        ::SetWindowPos(hWnd, NULL, 0, 0, wrect.right, wrect.bottom, SWP_NOMOVE | SWP_NOZORDER);//AdjustWindowRectEx(&wrect, GetWindowLong(hWnd,GWL_STYLE),(BOOL)GetMenu(hWnd),GetWindowLong(hWnd,GWL_EXSTYLE));

        OLWindow::windowmap[hWnd] = this;
    }
    void OLWindow::Show()
    {

        // �E�B���h�E��\������
        ShowWindow( hWnd, SW_SHOW );
        UpdateWindow( hWnd );

        // ���b�Z�[�W���[�v
        while( 1 )
        {
            BOOL ret = GetMessage( &msg, NULL, 0, 0 );  // ���b�Z�[�W���擾����
            if( ret == 0 || ret == -1 )
            {
                // �A�v���P�[�V�������I�������郁�b�Z�[�W�����Ă�����A
                // ���邢�� GetMessage() �����s������( -1 ���Ԃ��ꂽ�� �j�A���[�v�𔲂���
                break;
            }
            else
            {
                // ���b�Z�[�W����������
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
    }
    void OLWindow::Close()
    {
        SendMessage( hWnd, WM_CLOSE, 0, 0 );
    }

    OLWindow::~OLWindow(void)
    {
        DeleteObject(hFont);
    }
    void OLWindow::SetFont(TCHAR* name, int size)
    {
        DeleteObject(hFont);
        hFont = CreateFont(MulDiv(size, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72), 0, 0, 0, /* �t�H���g�̍���,���ϕ�����,������������̊p�x,�x�[�X���C���̊p�x */
            FW_NORMAL, FALSE, FALSE, 0, /* �t�H���g�̑���,�Α�,����,�������� */
            SHIFTJIS_CHARSET, /* �����Z�b�g�̎��ʎq */
            OUT_DEFAULT_PRECIS, /* �o�͐��x */
            CLIP_DEFAULT_PRECIS, /* �N���b�s���O���x */
            DEFAULT_QUALITY, DEFAULT_PITCH, name); /* �o�͕i��,�s�b�`�ƃt�@�~��,�t�H���g�� */
        SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0)); /* �t�H���g�̕ύX */
    }
    void OLWindow::SetFont(TCHAR* name, int size,bool bold,bool italic,bool underline,bool strike)
    {
        DeleteObject(hFont);
        hFont = CreateFont(MulDiv(size, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72), 0, 0, 0, /* �t�H���g�̍���,���ϕ�����,������������̊p�x,�x�[�X���C���̊p�x */
            bold ? FW_BOLD : FW_NORMAL, italic, underline, strike, /* �t�H���g�̑���,�Α�,����,�������� */
            SHIFTJIS_CHARSET, /* �����Z�b�g�̎��ʎq */
            OUT_DEFAULT_PRECIS, /* �o�͐��x */
            CLIP_DEFAULT_PRECIS, /* �N���b�s���O���x */
            DEFAULT_QUALITY, DEFAULT_PITCH, name); /* �o�͕i��,�s�b�`�ƃt�@�~��,�t�H���g�� */
        SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0)); /* �t�H���g�̕ύX */
    }
#if _UNICODE
    void OLWindow::SetFont(const char* name, int size)
    {
        DeleteObject(hFont);
        hFont = CreateFontA(MulDiv(size, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72), 0, 0, 0, /* �t�H���g�̍���,���ϕ�����,������������̊p�x,�x�[�X���C���̊p�x */
            FW_NORMAL, FALSE, FALSE, 0, /* �t�H���g�̑���,�Α�,����,�������� */
            SHIFTJIS_CHARSET, /* �����Z�b�g�̎��ʎq */
            OUT_DEFAULT_PRECIS, /* �o�͐��x */
            CLIP_DEFAULT_PRECIS, /* �N���b�s���O���x */
            DEFAULT_QUALITY, DEFAULT_PITCH, name); /* �o�͕i��,�s�b�`�ƃt�@�~��,�t�H���g�� */
        SendMessageA(hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0)); /* �t�H���g�̕ύX */
    }
    void OLWindow::SetFont(const char* name, int size,bool bold,bool italic,bool underline,bool strike)
    {
        DeleteObject(hFont);
        hFont = CreateFontA(MulDiv(size, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72), 0, 0, 0, /* �t�H���g�̍���,���ϕ�����,������������̊p�x,�x�[�X���C���̊p�x */
            bold ? FW_BOLD : FW_NORMAL, italic, underline, strike, /* �t�H���g�̑���,�Α�,����,�������� */
            SHIFTJIS_CHARSET, /* �����Z�b�g�̎��ʎq */
            OUT_DEFAULT_PRECIS, /* �o�͐��x */
            CLIP_DEFAULT_PRECIS, /* �N���b�s���O���x */
            DEFAULT_QUALITY, DEFAULT_PITCH, name); /* �o�͕i��,�s�b�`�ƃt�@�~��,�t�H���g�� */
        SendMessageA(hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0)); /* �t�H���g�̕ύX */
    }
#endif
    void OLWindow::Add(OLWindow& wnd)
    {
        SetParent(wnd.hWnd, this->hWnd);
    }
    LRESULT ButtonProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        Button* win = static_cast<Button*>(OLWindow::windowmap[hWnd]);
        //���݃`�F�b�N����K�v������
        switch( msg )
        {
        case WM_LBUTTONUP:
            win->OnClick(eventargs<OLWindow*>(win));
            break;
        }
        return CallWindowProc( win->baseWndProc, hWnd, msg, wp, lp );
    }
    Button::Button(OLWindow& parent,LPCWSTR title, int X,int Y,int nWidth,int nHeight) 
    {

        hInst = GetModuleHandle(NULL);
        this->hWnd = CreateWindow(
            _T("BUTTON"),                            // �E�B���h�E�N���X��
            title,                                // �L���v�V����
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,   // �X�^�C���w��
            X, Y,                                  // ���W
            nWidth, nHeight,                                  // �T�C�Y
            parent.hWnd,                                    // �e�E�B���h�E�̃n���h��
            (HMENU)this,                                    // ���j���[�n���h��
            hInst,                                 // �C���X�^���X�n���h��
            NULL                                     // ���̑��̍쐬�f�[�^
            );
        baseWndProc = (WNDPROC)(LONG_PTR)GetWindowLong( hWnd, GWL_WNDPROC );
        auto proc = ButtonProc/*[] /*CALLBACK*//*(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT
                                               {
                                               Button* win = static_cast<Button*>(OLWindow::windowmap[hWnd]);
                                               //���݃`�F�b�N����K�v������
                                               switch( msg )
                                               {
                                               case WM_LBUTTONUP:
                                               win->OnClick(eventargs<OLWindow*>(win));
                                               break;
                                               }
                                               return CallWindowProc( win->baseWndProc, hWnd, msg, wp, lp );
                                               }*/;
        SetWindowLong( hWnd, GWL_WNDPROC, (LONG)(LONG_PTR)(proc) );
        lang::OLWindow::windowmap[this->hWnd] = this;
        //SetWindowTheme(hWnd, L"", L"");
        //ctor(title, X, Y, nWidth, nHeight,L"BUTTON",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,parent.hWnd);
    }
    Button::Button(OLWindow& parent,LPCSTR title, int X,int Y,int nWidth,int nHeight) 
    {

        hInst = GetModuleHandle(NULL);
        this->hWnd = CreateWindowA(
            "BUTTON",                            // �E�B���h�E�N���X��
            title,                                // �L���v�V����
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,   // �X�^�C���w��
            X, Y,                                  // ���W
            nWidth, nHeight,                                  // �T�C�Y
            parent.hWnd,                                    // �e�E�B���h�E�̃n���h��
            (HMENU)this,                                    // ���j���[�n���h��
            hInst,                                 // �C���X�^���X�n���h��
            NULL                                     // ���̑��̍쐬�f�[�^
            );
        baseWndProc = (WNDPROC)(LONG_PTR)GetWindowLong( hWnd, GWL_WNDPROC );
        auto proc = ButtonProc;
        SetWindowLong( hWnd, GWL_WNDPROC, (LONG)(LONG_PTR)(proc) );
        lang::OLWindow::windowmap[this->hWnd] = this;
        //HTHEME tm = GetWindowTheme(hWnd);\
        SetWindowTheme(hWnd, L"", NULL);\
        auto tm2 = GetWindowTheme(hWnd);
    }
    LRESULT CheckBoxProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        Button* win = static_cast<Button*>(OLWindow::windowmap[hWnd]);
        //���݃`�F�b�N����K�v������
        switch( msg )
        {
        case WM_LBUTTONUP:
            win->OnClick(eventargs<OLWindow*>(win));
            break;
        }
        return CallWindowProc( win->baseWndProc, hWnd, msg, wp, lp );
    }
    CheckBox::CheckBox(OLWindow& parent,LPCWSTR title, int X,int Y,int nWidth,int nHeight) 
    {
        hInst = GetModuleHandle(NULL);
        this->hWnd = CreateWindow(
            _T("BUTTON"),                            // �E�B���h�E�N���X��
            title,                                // �L���v�V����
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,   // �X�^�C���w��
            X, Y,                                  // ���W
            nWidth, nHeight,                                  // �T�C�Y
            parent.hWnd,                                    // �e�E�B���h�E�̃n���h��
            (HMENU)this,                                    // ���j���[�n���h��
            hInst,                                 // �C���X�^���X�n���h��
            NULL                                     // ���̑��̍쐬�f�[�^
            );
        baseWndProc = (WNDPROC)(LONG_PTR)GetWindowLong( hWnd, GWL_WNDPROC );
        OLWindow::windowmap[this->hWnd] = this;
        auto proc = (WNDPROC)CheckBoxProc;/*[] (HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT
                                          {
                                          Button* win = static_cast<Button*>(OLWindow::windowmap[hWnd]);
                                          //���݃`�F�b�N����K�v������
                                          switch( msg )
                                          {
                                          case WM_LBUTTONUP:
                                          win->OnClick(eventargs<OLWindow*>(win));
                                          break;
                                          }
                                          return CallWindowProc( win->baseWndProc, hWnd, msg, wp, lp );
                                          };*/
        SetWindowLong( hWnd, GWL_WNDPROC, (LONG)(LONG_PTR)static_cast<WNDPROC>(proc) );
        //ctor(title, X, Y, nWidth, nHeight,L"BUTTON",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,parent.hWnd);
    }
    CheckBox::CheckBox(OLWindow& parent,LPCSTR title, int X,int Y,int nWidth,int nHeight) 
    {
        hInst = GetModuleHandle(NULL);
        this->hWnd = CreateWindowA(
            "BUTTON",                            // �E�B���h�E�N���X��
            title,                                // �L���v�V����
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,   // �X�^�C���w��
            X, Y,                                  // ���W
            nWidth, nHeight,                                  // �T�C�Y
            parent.hWnd,                                    // �e�E�B���h�E�̃n���h��
            (HMENU)this,                                    // ���j���[�n���h��
            hInst,                                 // �C���X�^���X�n���h��
            NULL                                     // ���̑��̍쐬�f�[�^
            );
        baseWndProc = (WNDPROC)(LONG_PTR)GetWindowLong( hWnd, GWL_WNDPROC );
        OLWindow::windowmap[this->hWnd] = this;
        auto proc = (WNDPROC)CheckBoxProc;
        SetWindowLong( hWnd, GWL_WNDPROC, (LONG)(LONG_PTR)static_cast<WNDPROC>(proc) );
    }

    LRESULT TextBoxProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        TextBox* win = static_cast<TextBox*>(OLWindow::windowmap[hWnd]);
        //���݃`�F�b�N����K�v������
        switch( msg )
        {
        case WM_LBUTTONUP:
            //win->OnClick(eventargs<Button*>(win));
            break;
        case WM_SIZING:
            win->OnSizeChange(eventargs<OLWindow*>(win));
            break;
        }
        return CallWindowProc( win->baseWndProc, hWnd, msg, wp, lp );
    }
    TextBox::TextBox(OLWindow& parent,LPCWSTR title, int X,int Y,int nWidth,int nHeight, bool multiline) 
    {

        hInst = GetModuleHandle(NULL);
        if(multiline)this->hWnd = CreateWindow(
            _T("EDIT"),                            // �E�B���h�E�N���X��
            title,                                // �L���v�V����
            WS_CHILD | WS_VISIBLE | ES_MULTILINE | 
            WS_HSCROLL | WS_VSCROLL |
            ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | WS_BORDER,   // �X�^�C���w��
            X, Y,                                  // ���W
            nWidth, nHeight,                                  // �T�C�Y
            parent.hWnd,                                    // �e�E�B���h�E�̃n���h��
            (HMENU)this,                                    // ���j���[�n���h��
            hInst,                                 // �C���X�^���X�n���h��
            NULL                                     // ���̑��̍쐬�f�[�^
            );
        else 
            this->hWnd = CreateWindow(
            _T("EDIT"),                            // �E�B���h�E�N���X��
            title,                                // �L���v�V����
            WS_CHILD | WS_VISIBLE |
            // WS_HSCROLL | WS_VSCROLL |
            ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | WS_BORDER,   // �X�^�C���w��
            X, Y,                                  // ���W
            nWidth, nHeight,                                  // �T�C�Y
            parent.hWnd,                                    // �e�E�B���h�E�̃n���h��
            (HMENU)this,                                    // ���j���[�n���h��
            hInst,                                 // �C���X�^���X�n���h��
            NULL                                     // ���̑��̍쐬�f�[�^
            );
        baseWndProc = (WNDPROC)(LONG_PTR)GetWindowLong( hWnd, GWL_WNDPROC );
        OLWindow::windowmap[this->hWnd] = this;
        auto proc = TextBoxProc;/*[] /*CALLBACK*//*(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT
                                                 {
                                                 TextBox* win = static_cast<TextBox*>(OLWindow::windowmap[hWnd]);
                                                 //���݃`�F�b�N����K�v������
                                                 switch( msg )
                                                 {
                                                 case WM_LBUTTONUP:
                                                 //win->OnClick(eventargs<Button*>(win));
                                                 break;
                                                 case WM_SIZING:
                                                 win->OnSizeChange(eventargs<OLWindow*>(win));
                                                 break;
                                                 }
                                                 return CallWindowProc( win->baseWndProc, hWnd, msg, wp, lp );
                                                 };*/
        SetWindowLong( hWnd, GWL_WNDPROC, (LONG)(LONG_PTR)/*static_cast<WNDPROC>*/(proc) );
    }
    TextBox::TextBox(OLWindow& parent,LPCSTR title, int X,int Y,int nWidth,int nHeight, bool multiline) 
    {

        hInst = GetModuleHandle(NULL);
        if(multiline)this->hWnd = CreateWindowA(
            "EDIT",                            // �E�B���h�E�N���X��
            title,                                // �L���v�V����
            WS_CHILD | WS_VISIBLE | ES_MULTILINE | 
            WS_HSCROLL | WS_VSCROLL |
            ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | WS_BORDER,   // �X�^�C���w��
            X, Y,                                  // ���W
            nWidth, nHeight,                                  // �T�C�Y
            parent.hWnd,                                    // �e�E�B���h�E�̃n���h��
            (HMENU)this,                                    // ���j���[�n���h��
            hInst,                                 // �C���X�^���X�n���h��
            NULL                                     // ���̑��̍쐬�f�[�^
            );
        else 
            this->hWnd = CreateWindowA(
            "EDIT",                            // �E�B���h�E�N���X��
            title,                                // �L���v�V����
            WS_CHILD | WS_VISIBLE |
            // WS_HSCROLL | WS_VSCROLL |
            ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | WS_BORDER,   // �X�^�C���w��
            X, Y,                                  // ���W
            nWidth, nHeight,                                  // �T�C�Y
            parent.hWnd,                                    // �e�E�B���h�E�̃n���h��
            (HMENU)this,                                    // ���j���[�n���h��
            hInst,                                 // �C���X�^���X�n���h��
            NULL                                     // ���̑��̍쐬�f�[�^
            );
        baseWndProc = (WNDPROC)(LONG_PTR)GetWindowLong( hWnd, GWL_WNDPROC );
        OLWindow::windowmap[this->hWnd] = this;
        auto proc = TextBoxProc;
        SetWindowLong( hWnd, GWL_WNDPROC, (LONG)(LONG_PTR)/*static_cast<WNDPROC>*/(proc) );
    }
    //copyhttp://d.hatena.ne.jp/yus_iri/20110911/1315730376
    BOOL SetClientSize(HWND hWnd, int width, int height)
    {
        RECT rw, rc;
        ::GetWindowRect(hWnd, &rw);
        ::GetClientRect(hWnd, &rc);

        int new_width = (rw.right - rw.left) - (rc.right - rc.left) + width;
        int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + height;

        return ::SetWindowPos(hWnd, NULL, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);
    }
    LONG OLWindow::SetWidth(LONG width)
    {
        SetClientSize(hWnd, width, GetHeight());
        return width;
    }
    LONG OLWindow::SetHeight(LONG height)
    {
        SetClientSize(hWnd, GetWidth(),height);
        return height;
    }
    void OLWindow::SetResize(BOOL r)
    {
        auto result = GetWindowLongPtr(this->hWnd,GWL_STYLE);
        if(r)result &= WS_THICKFRAME;
        else
            result &= ~WS_THICKFRAME;
        SetWindowLongPtr(this->hWnd,GWL_STYLE,result);
    }
    Label::Label(OLWindow& parent,LPCWSTR title, int X,int Y,int nWidth,int nHeight) 
    {
        hInst = GetModuleHandle(NULL);
        // �E�B���h�E�N���X�̏���ݒ�

        this->hWnd = CreateWindow(
            _T("STATIC"),                            // �E�B���h�E�N���X��
            title,                                // �L���v�V����
            WS_CHILD | WS_VISIBLE,   // �X�^�C���w��
            X, Y,                                  // ���W
            nWidth, nHeight,                                  // �T�C�Y
            parent.hWnd,                                    // �e�E�B���h�E�̃n���h��
            (HMENU)this,                                    // ���j���[�n���h��
            hInst,                                 // �C���X�^���X�n���h��
            NULL                                     // ���̑��̍쐬�f�[�^
            );
        OLWindow::windowmap[this->hWnd] = this;
    }
    Label::Label(OLWindow& parent,LPCSTR title, int X,int Y,int nWidth,int nHeight) 
    {
        hInst = GetModuleHandle(NULL);
        // �E�B���h�E�N���X�̏���ݒ�

        this->hWnd = CreateWindowA(
            "STATIC",                            // �E�B���h�E�N���X��
            title,                                // �L���v�V����
            WS_CHILD | WS_VISIBLE,   // �X�^�C���w��
            X, Y,                                  // ���W
            nWidth, nHeight,                                  // �T�C�Y
            parent.hWnd,                                    // �e�E�B���h�E�̃n���h��
            (HMENU)this,                                    // ���j���[�n���h��
            hInst,                                 // �C���X�^���X�n���h��
            NULL                                     // ���̑��̍쐬�f�[�^
            );
        OLWindow::windowmap[this->hWnd] = this;
    }
    LRESULT OLWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        OLWindow* win = static_cast<OLWindow*>(OLWindow::windowmap[hWnd]);
        switch( msg )
        {
        case WM_COMMAND:
            std::cout<< OLWindow::windowmap[(HWND)lp]<<win;
            break;
        case WM_LBUTTONUP:
            win->OnClick(eventargs<OLWindow*>(win));
            break;
        case WM_SIZING:
            win->OnSizeChange(eventargs<OLWindow*>(win));
            break;
        case WM_DESTROY:  // �E�B���h�E��j������Ƃ�
            PostQuitMessage( 0 );
            return 0;
        }

        // ���̃��b�Z�[�W�́A�f�t�H���g�̏������s��
        return DefWindowProc( hWnd, msg, wp, lp );
    }
    BYTE WindowClassInit()
    {
        WNDCLASSEX wc;
        // �E�B���h�E�N���X�̏���ݒ�
        wc.cbSize = sizeof(wc);               // �\���̃T�C�Y
        wc.style = CS_HREDRAW | CS_VREDRAW;   // �X�^�C��
        wc.lpfnWndProc = (WNDPROC)OLWindowProc;/*[] /*CALLBACK*//*(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT{
                                                                };*/
        wc.cbClsExtra = 0;                    // �g�����P
        wc.cbWndExtra = 0;                    // �g�����Q
        wc.hInstance = GetModuleHandle(NULL);                 // �C���X�^���X�n���h��
        wc.hIcon = (HICON)LoadImage(          // �A�C�R��
            NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON,
            0, 0, LR_DEFAULTSIZE | LR_SHARED
            );
        wc.hIconSm = wc.hIcon;                // �q�A�C�R��
        wc.hCursor = (HCURSOR)LoadImage(      // �}�E�X�J�[�\��
            NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR,
            0, 0, LR_DEFAULTSIZE | LR_SHARED
            );
        //GetClassInfoEx(GetModuleHandle(NULL),_T("STATIC"),&wc);
        wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE); //NULL;//GetSysColorBrush(COLOR_WINDOWFRAME);//(HBRUSH)GetStockObject(_BRUSH); // �E�B���h�E�w�i
        wc.lpszMenuName = NULL;                     // ���j���[��
        wc.lpszClassName = OLWindow::classname;// �E�B���h�E�N���X��

        // �E�B���h�E�N���X��o�^����
        if( RegisterClassEx( &wc ) == 0 ){ throw GetLastError();}//return ; }
#ifdef CPP11
        //================LABEL================
        wc.cbSize = sizeof(wc);               // �\���̃T�C�Y
        wc.style = CS_HREDRAW | CS_VREDRAW;   // �X�^�C��
        wc.lpfnWndProc = [] /*CALLBACK*/(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT{

            OLWindow* win = static_cast<OLWindow*>(OLWindow::windowmap[hWnd]);
            switch( msg )
            {
            case WM_PAINT:
                {
                    HFONT hFont = win->hFont;//(HFONT)DefWindowProc( hWnd, WM_GETFONT, wp, lp );
                    auto reult = DefWindowProc( hWnd, msg, wp, lp );
                    RECT rc;
                    HDC hDC = GetDC( hWnd );
                    auto text = win->GetTextW();
                    GetClientRect(hWnd, &rc);
                    ////GetWindowRect(hWnd, &rc);
                    SelectObject( hDC, hFont );
                    DrawText( hDC, text, -1, &rc, 0 );
                    //SetTextColor( hDC, RGB(255,0,0) );
                    //TextOut( hDC, 50, 50, text, (int)_tcslen(text) );
                    delete text;
                    ReleaseDC( hWnd, hDC );
                    return reult;
                }    
            case WM_LBUTTONUP:
                win->OnClick(eventargs<OLWindow*>(win));
                break;
            case WM_SIZING:
                win->OnSizeChange(eventargs<OLWindow*>(win));
                break;
            case WM_DESTROY:  // �E�B���h�E��j������Ƃ�
                PostQuitMessage( 0 );
                return 0;
            }

            // ���̃��b�Z�[�W�́A�f�t�H���g�̏������s��
            return DefWindowProc( hWnd, msg, wp, lp );
        };             // �E�B���h�E�v���V�[�W��
        wc.cbClsExtra = 0;                    // �g�����P
        wc.cbWndExtra = 0;                    // �g�����Q
        wc.hInstance = GetModuleHandle(NULL);                 // �C���X�^���X�n���h��
        wc.hIcon = (HICON)LoadImage(          // �A�C�R��
            NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON,
            0, 0, LR_DEFAULTSIZE | LR_SHARED
            );
        wc.hIconSm = wc.hIcon;                // �q�A�C�R��
        wc.hCursor = (HCURSOR)LoadImage(      // �}�E�X�J�[�\��
            NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR,
            0, 0, LR_DEFAULTSIZE | LR_SHARED
            );
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // �E�B���h�E�w�i
        wc.lpszMenuName = NULL;                     // ���j���[��
        wc.lpszClassName = _T("OLlabel");// �E�B���h�E�N���X��

        // �E�B���h�E�N���X��o�^����
        if( RegisterClassEx( &wc ) == 0 ){ throw GetLastError();}
#endif
        return NULL;
    }
    BYTE Init = WindowClassInit();

    /*Button& Button::operator=(const Button& copy)// : OnClick(copy.OnClick)
    {
    windowmap[copy.hWnd] = this;
    hWnd = copy.hWnd;
    hInst = copy.hInst;
    hFont = copy.hFont;
    Tag = copy.Tag;
    baseWndProc = copy.baseWndProc;
    OnClick = copy.OnClick;
    OnSizeChange = copy.OnSizeChange;
    //OLWindow* ptr = ((OLWindow*)this);
    //*ptr = OLWindow(copy);
    //memcpy(this, &copy, sizeof(copy));
    //this->OnClick = decltype(copy.OnClick)();//copy.OnClick;
    //this->OnClick.even = decltype(this->OnClick.even)();
    //this->OnSizeChange = decltype(copy.OnSizeChange)();//copy.OnSizeChange;
    //this->OnSizeChange.even = decltype(this->OnSizeChange.even)();
    return *(this);
    }*/
    void __MessageBox(const char* message)
    {
        ::MessageBoxA(NULL, message, NULL, MB_OK);
    }
    void __MessageBox(OLWindow &parent,const char* message)
    {
        ::MessageBoxA(parent.hWnd, message, NULL, MB_OK);
    }
    void __MessageBox(const wchar_t* message)
    {
        ::MessageBoxW(NULL, message, NULL, MB_OK);
    }
    void __MessageBox(OLWindow &parent,const wchar_t* message)
    {
        ::MessageBoxW(parent.hWnd, message, NULL, MB_OK);
    }
}
#endif
