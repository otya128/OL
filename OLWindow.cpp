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
        case WM_CREATE:			// ウィンドウが作成されたとき
            // プッシュボタンを作成する
            hButton = CreateWindow(
                _T("BUTTON"),                            // ウィンドウクラス名
                _T("OK"),                                // キャプション
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,   // スタイル指定
                20, 20,                                  // 座標
                60, 40,                                  // サイズ
                hWnd,                                    // 親ウィンドウのハンドル
                NULL,                                    // メニューハンドル
                hInst,                                 // インスタンスハンドル
                NULL                                     // その他の作成データ
                );
            return 0;
        case WM_DESTROY:  // ウィンドウを破棄するとき
            PostQuitMessage( 0 );
            return 0;
        }

        // 他のメッセージは、デフォルトの処理を行う
        return DefWindowProc( hWnd, msg, wp, lp );
    }
    /*WNDPROC getproc()
    {
    return OLWindow::WndProc;
    }*/
    void OLWindow::ctor(LPCWSTR title, int X,int Y,int nWidth,int nHeight, LPCWSTR ClassName,long style,HWND parent)
    {
        hInst = GetModuleHandle(NULL);
        // ウィンドウクラスの情報を設定
        wc.cbSize = sizeof(wc);               // 構造体サイズ
        wc.style = CS_HREDRAW | CS_VREDRAW;   // スタイル
        auto a = this;
        wc.lpfnWndProc = [] /*CALLBACK*/(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT{

            OLWindow* win = static_cast<OLWindow*>(OLWindow::windowmap[hWnd]);
            switch( msg )
            {
            case WM_SIZING:
                win->OnSizeChange(eventargs<OLWindow*>(win));
                break;
            case WM_DESTROY:  // ウィンドウを破棄するとき
                PostQuitMessage( 0 );
                return 0;
            }

            // 他のメッセージは、デフォルトの処理を行う
            return DefWindowProc( hWnd, msg, wp, lp );
        };             // ウィンドウプロシージャ
        wc.cbClsExtra = 0;                    // 拡張情報１
        wc.cbWndExtra = 0;                    // 拡張情報２
        wc.hInstance = hInst;                 // インスタンスハンドル
        wc.hIcon = (HICON)LoadImage(          // アイコン
            NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON,
            0, 0, LR_DEFAULTSIZE | LR_SHARED
            );
        wc.hIconSm = wc.hIcon;                // 子アイコン
        wc.hCursor = (HCURSOR)LoadImage(      // マウスカーソル
            NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR,
            0, 0, LR_DEFAULTSIZE | LR_SHARED
            );
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ウィンドウ背景
        wc.lpszMenuName = NULL;                     // メニュー名
        wc.lpszClassName = (ClassName);// ウィンドウクラス名

        // ウィンドウクラスを登録する
        if( RegisterClassEx( &wc ) == 0 ){ throw GetLastError();}//return ; }

        // ウィンドウを作成する
        hWnd = CreateWindow(
            wc.lpszClassName,      // ウィンドウクラス名
            /*_T*/(title),  // タイトルバーに表示する文字列
            style,   // ウィンドウの種類
            X,         // ウィンドウを表示する位置（X座標）
            Y,         // ウィンドウを表示する位置（Y座標）
            nWidth,         // ウィンドウの幅
            nHeight,         // ウィンドウの高さ
            parent,                  // 親ウィンドウのウィンドウハンドル
            NULL,                  // メニューハンドル
            hInst,                 // インスタンスハンドル
            NULL                   // その他の作成データ
            );
        if( hWnd == NULL ){ throw GetLastError(); }

        OLWindow::windowmap[hWnd] = this;
    }
    void OLWindow::Show()
    {

        // ウィンドウを表示する
        ShowWindow( hWnd, SW_SHOW );
        UpdateWindow( hWnd );

        // メッセージループ
        while( 1 )
        {
            BOOL ret = GetMessage( &msg, NULL, 0, 0 );  // メッセージを取得する
            if( ret == 0 || ret == -1 )
            {
                // アプリケーションを終了させるメッセージが来ていたら、
                // あるいは GetMessage() が失敗したら( -1 が返されたら ）、ループを抜ける
                break;
            }
            else
            {
                // メッセージを処理する
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
            _T("BUTTON"),                            // ウィンドウクラス名
            title,                                // キャプション
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,   // スタイル指定
            X, Y,                                  // 座標
            nWidth, nHeight,                                  // サイズ
            parent.hWnd,                                    // 親ウィンドウのハンドル
            (HMENU)this,                                    // メニューハンドル
            hInst,                                 // インスタンスハンドル
            NULL                                     // その他の作成データ
            );
        baseWndProc = (WNDPROC)(LONG_PTR)GetWindowLong( hWnd, GWL_WNDPROC );
        OLWindow::windowmap[this->hWnd] = this;
        auto proc = [] /*CALLBACK*/(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT
        {
            Button* win = static_cast<Button*>(OLWindow::windowmap[hWnd]);
            //存在チェックする必要がある
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
            _T("EDIT"),                            // ウィンドウクラス名
            title,                                // キャプション
            WS_CHILD | WS_VISIBLE | 
            // | WS_HSCROLL | WS_VSCROLL |
            ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | WS_BORDER,   // スタイル指定
            X, Y,                                  // 座標
            nWidth, nHeight,                                  // サイズ
            parent.hWnd,                                    // 親ウィンドウのハンドル
            (HMENU)this,                                    // メニューハンドル
            hInst,                                 // インスタンスハンドル
            NULL                                     // その他の作成データ
            );
        baseWndProc = (WNDPROC)(LONG_PTR)GetWindowLong( hWnd, GWL_WNDPROC );
        OLWindow::windowmap[this->hWnd] = this;
        auto proc = [] /*CALLBACK*/(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT
        {
            TextBox* win = static_cast<TextBox*>(OLWindow::windowmap[hWnd]);
            //存在チェックする必要がある
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
