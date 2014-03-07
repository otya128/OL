#include "stdafx.h"
#include "OLWindow.h"

namespace lang
{
    LPCWSTR OLWindow::classname = _T("OLWindow");
    LPCWSTR Button::classname = _T("BUTTON");
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
        // �E�B���h�E�N���X�̏���ݒ�
        wc.cbSize = sizeof(wc);               // �\���̃T�C�Y
        wc.style = CS_HREDRAW | CS_VREDRAW;   // �X�^�C��
        auto a = this;
        wc.lpfnWndProc = [] /*CALLBACK*/(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT{

            OLWindow* win = static_cast<OLWindow*>(OLWindow::windowmap[hWnd]);
            switch( msg )
            {
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
        wc.hInstance = hInst;                 // �C���X�^���X�n���h��
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
        wc.lpszClassName = (ClassName);// �E�B���h�E�N���X��

        // �E�B���h�E�N���X��o�^����
        if( RegisterClassEx( &wc ) == 0 ){ throw GetLastError();}//return ; }

        // �E�B���h�E���쐬����
        hWnd = CreateWindow(
            wc.lpszClassName,      // �E�B���h�E�N���X��
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
        if( hWnd == NULL ){ throw GetLastError(); }

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
    }
    void OLWindow::Add(OLWindow& wnd)
    {
        SetParent(wnd.hWnd, this->hWnd);
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
        OLWindow::windowmap[this->hWnd] = this;
        auto proc = [] /*CALLBACK*/(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT
        {
            Button* win = static_cast<Button*>(OLWindow::windowmap[hWnd]);
            //���݃`�F�b�N����K�v������
            switch( msg )
            {
            case WM_LBUTTONUP:
                win->OnClick(eventargs<Button*>(win));
                break;
            }
            return CallWindowProc( win->baseWndProc, hWnd, msg, wp, lp );
        };
        SetWindowLong( hWnd, GWL_WNDPROC, (LONG)(LONG_PTR)static_cast<WNDPROC>(proc) );
        //ctor(title, X, Y, nWidth, nHeight,L"BUTTON",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,parent.hWnd);
    }

    TextBox::TextBox(OLWindow& parent,LPCWSTR title, int X,int Y,int nWidth,int nHeight) 
    {

        hInst = GetModuleHandle(NULL);
        this->hWnd = CreateWindow(
            _T("EDIT"),                            // �E�B���h�E�N���X��
            title,                                // �L���v�V����
            WS_CHILD | WS_VISIBLE | 
            // | WS_HSCROLL | WS_VSCROLL |
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
        auto proc = [] /*CALLBACK*/(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT
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
        };
        SetWindowLong( hWnd, GWL_WNDPROC, (LONG)(LONG_PTR)static_cast<WNDPROC>(proc) );
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
}
