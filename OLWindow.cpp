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
    //GWLP_USERDATA使えばいい！！！！！！！！！！！！！
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

        // ウィンドウを作成する
        hWnd = CreateWindow(
            OLWindow::classname,      // ウィンドウクラス名
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

        // ウィンドウを作成する
        hWnd = CreateWindowA(
            OLWindow::classnameA,      // ウィンドウクラス名
            title,  // タイトルバーに表示する文字列
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
        DeleteObject(hFont);
    }
    void OLWindow::SetFont(TCHAR* name, int size)
    {
        DeleteObject(hFont);
        hFont = CreateFont(MulDiv(size, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72), 0, 0, 0, /* フォントの高さ,平均文字幅,文字送り方向の角度,ベースラインの角度 */
            FW_NORMAL, FALSE, FALSE, 0, /* フォントの太さ,斜体,下線,取り消し線 */
            SHIFTJIS_CHARSET, /* 文字セットの識別子 */
            OUT_DEFAULT_PRECIS, /* 出力精度 */
            CLIP_DEFAULT_PRECIS, /* クリッピング精度 */
            DEFAULT_QUALITY, DEFAULT_PITCH, name); /* 出力品質,ピッチとファミリ,フォント名 */
        SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0)); /* フォントの変更 */
    }
    void OLWindow::SetFont(TCHAR* name, int size,bool bold,bool italic,bool underline,bool strike)
    {
        DeleteObject(hFont);
        hFont = CreateFont(MulDiv(size, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72), 0, 0, 0, /* フォントの高さ,平均文字幅,文字送り方向の角度,ベースラインの角度 */
            bold ? FW_BOLD : FW_NORMAL, italic, underline, strike, /* フォントの太さ,斜体,下線,取り消し線 */
            SHIFTJIS_CHARSET, /* 文字セットの識別子 */
            OUT_DEFAULT_PRECIS, /* 出力精度 */
            CLIP_DEFAULT_PRECIS, /* クリッピング精度 */
            DEFAULT_QUALITY, DEFAULT_PITCH, name); /* 出力品質,ピッチとファミリ,フォント名 */
        SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0)); /* フォントの変更 */
    }
#if _UNICODE
    void OLWindow::SetFont(const char* name, int size)
    {
        DeleteObject(hFont);
        hFont = CreateFontA(MulDiv(size, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72), 0, 0, 0, /* フォントの高さ,平均文字幅,文字送り方向の角度,ベースラインの角度 */
            FW_NORMAL, FALSE, FALSE, 0, /* フォントの太さ,斜体,下線,取り消し線 */
            SHIFTJIS_CHARSET, /* 文字セットの識別子 */
            OUT_DEFAULT_PRECIS, /* 出力精度 */
            CLIP_DEFAULT_PRECIS, /* クリッピング精度 */
            DEFAULT_QUALITY, DEFAULT_PITCH, name); /* 出力品質,ピッチとファミリ,フォント名 */
        SendMessageA(hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0)); /* フォントの変更 */
    }
    void OLWindow::SetFont(const char* name, int size,bool bold,bool italic,bool underline,bool strike)
    {
        DeleteObject(hFont);
        hFont = CreateFontA(MulDiv(size, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72), 0, 0, 0, /* フォントの高さ,平均文字幅,文字送り方向の角度,ベースラインの角度 */
            bold ? FW_BOLD : FW_NORMAL, italic, underline, strike, /* フォントの太さ,斜体,下線,取り消し線 */
            SHIFTJIS_CHARSET, /* 文字セットの識別子 */
            OUT_DEFAULT_PRECIS, /* 出力精度 */
            CLIP_DEFAULT_PRECIS, /* クリッピング精度 */
            DEFAULT_QUALITY, DEFAULT_PITCH, name); /* 出力品質,ピッチとファミリ,フォント名 */
        SendMessageA(hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0)); /* フォントの変更 */
    }
#endif
    void OLWindow::Add(OLWindow& wnd)
    {
        SetParent(wnd.hWnd, this->hWnd);
    }
    LRESULT ButtonProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        Button* win = static_cast<Button*>(OLWindow::windowmap[hWnd]);
        //存在チェックする必要がある
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
        auto proc = ButtonProc/*[] /*CALLBACK*//*(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT
                                               {
                                               Button* win = static_cast<Button*>(OLWindow::windowmap[hWnd]);
                                               //存在チェックする必要がある
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
            "BUTTON",                            // ウィンドウクラス名
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
        //存在チェックする必要がある
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
            _T("BUTTON"),                            // ウィンドウクラス名
            title,                                // キャプション
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,   // スタイル指定
            X, Y,                                  // 座標
            nWidth, nHeight,                                  // サイズ
            parent.hWnd,                                    // 親ウィンドウのハンドル
            (HMENU)this,                                    // メニューハンドル
            hInst,                                 // インスタンスハンドル
            NULL                                     // その他の作成データ
            );
        baseWndProc = (WNDPROC)(LONG_PTR)GetWindowLong( hWnd, GWL_WNDPROC );
        OLWindow::windowmap[this->hWnd] = this;
        auto proc = (WNDPROC)CheckBoxProc;/*[] (HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT
                                          {
                                          Button* win = static_cast<Button*>(OLWindow::windowmap[hWnd]);
                                          //存在チェックする必要がある
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
            "BUTTON",                            // ウィンドウクラス名
            title,                                // キャプション
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,   // スタイル指定
            X, Y,                                  // 座標
            nWidth, nHeight,                                  // サイズ
            parent.hWnd,                                    // 親ウィンドウのハンドル
            (HMENU)this,                                    // メニューハンドル
            hInst,                                 // インスタンスハンドル
            NULL                                     // その他の作成データ
            );
        baseWndProc = (WNDPROC)(LONG_PTR)GetWindowLong( hWnd, GWL_WNDPROC );
        OLWindow::windowmap[this->hWnd] = this;
        auto proc = (WNDPROC)CheckBoxProc;
        SetWindowLong( hWnd, GWL_WNDPROC, (LONG)(LONG_PTR)static_cast<WNDPROC>(proc) );
    }

    LRESULT TextBoxProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
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
    }
    TextBox::TextBox(OLWindow& parent,LPCWSTR title, int X,int Y,int nWidth,int nHeight, bool multiline) 
    {

        hInst = GetModuleHandle(NULL);
        if(multiline)this->hWnd = CreateWindow(
            _T("EDIT"),                            // ウィンドウクラス名
            title,                                // キャプション
            WS_CHILD | WS_VISIBLE | ES_MULTILINE | 
            WS_HSCROLL | WS_VSCROLL |
            ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | WS_BORDER,   // スタイル指定
            X, Y,                                  // 座標
            nWidth, nHeight,                                  // サイズ
            parent.hWnd,                                    // 親ウィンドウのハンドル
            (HMENU)this,                                    // メニューハンドル
            hInst,                                 // インスタンスハンドル
            NULL                                     // その他の作成データ
            );
        else 
            this->hWnd = CreateWindow(
            _T("EDIT"),                            // ウィンドウクラス名
            title,                                // キャプション
            WS_CHILD | WS_VISIBLE |
            // WS_HSCROLL | WS_VSCROLL |
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
        auto proc = TextBoxProc;/*[] /*CALLBACK*//*(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT
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
                                                 };*/
        SetWindowLong( hWnd, GWL_WNDPROC, (LONG)(LONG_PTR)/*static_cast<WNDPROC>*/(proc) );
    }
    TextBox::TextBox(OLWindow& parent,LPCSTR title, int X,int Y,int nWidth,int nHeight, bool multiline) 
    {

        hInst = GetModuleHandle(NULL);
        if(multiline)this->hWnd = CreateWindowA(
            "EDIT",                            // ウィンドウクラス名
            title,                                // キャプション
            WS_CHILD | WS_VISIBLE | ES_MULTILINE | 
            WS_HSCROLL | WS_VSCROLL |
            ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | WS_BORDER,   // スタイル指定
            X, Y,                                  // 座標
            nWidth, nHeight,                                  // サイズ
            parent.hWnd,                                    // 親ウィンドウのハンドル
            (HMENU)this,                                    // メニューハンドル
            hInst,                                 // インスタンスハンドル
            NULL                                     // その他の作成データ
            );
        else 
            this->hWnd = CreateWindowA(
            "EDIT",                            // ウィンドウクラス名
            title,                                // キャプション
            WS_CHILD | WS_VISIBLE |
            // WS_HSCROLL | WS_VSCROLL |
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
        // ウィンドウクラスの情報を設定

        this->hWnd = CreateWindow(
            _T("STATIC"),                            // ウィンドウクラス名
            title,                                // キャプション
            WS_CHILD | WS_VISIBLE,   // スタイル指定
            X, Y,                                  // 座標
            nWidth, nHeight,                                  // サイズ
            parent.hWnd,                                    // 親ウィンドウのハンドル
            (HMENU)this,                                    // メニューハンドル
            hInst,                                 // インスタンスハンドル
            NULL                                     // その他の作成データ
            );
        OLWindow::windowmap[this->hWnd] = this;
    }
    Label::Label(OLWindow& parent,LPCSTR title, int X,int Y,int nWidth,int nHeight) 
    {
        hInst = GetModuleHandle(NULL);
        // ウィンドウクラスの情報を設定

        this->hWnd = CreateWindowA(
            "STATIC",                            // ウィンドウクラス名
            title,                                // キャプション
            WS_CHILD | WS_VISIBLE,   // スタイル指定
            X, Y,                                  // 座標
            nWidth, nHeight,                                  // サイズ
            parent.hWnd,                                    // 親ウィンドウのハンドル
            (HMENU)this,                                    // メニューハンドル
            hInst,                                 // インスタンスハンドル
            NULL                                     // その他の作成データ
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
        case WM_DESTROY:  // ウィンドウを破棄するとき
            PostQuitMessage( 0 );
            return 0;
        }

        // 他のメッセージは、デフォルトの処理を行う
        return DefWindowProc( hWnd, msg, wp, lp );
    }
    BYTE WindowClassInit()
    {
        WNDCLASSEX wc;
        // ウィンドウクラスの情報を設定
        wc.cbSize = sizeof(wc);               // 構造体サイズ
        wc.style = CS_HREDRAW | CS_VREDRAW;   // スタイル
        wc.lpfnWndProc = (WNDPROC)OLWindowProc;/*[] /*CALLBACK*//*(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT{
                                                                };*/
        wc.cbClsExtra = 0;                    // 拡張情報１
        wc.cbWndExtra = 0;                    // 拡張情報２
        wc.hInstance = GetModuleHandle(NULL);                 // インスタンスハンドル
        wc.hIcon = (HICON)LoadImage(          // アイコン
            NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON,
            0, 0, LR_DEFAULTSIZE | LR_SHARED
            );
        wc.hIconSm = wc.hIcon;                // 子アイコン
        wc.hCursor = (HCURSOR)LoadImage(      // マウスカーソル
            NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR,
            0, 0, LR_DEFAULTSIZE | LR_SHARED
            );
        //GetClassInfoEx(GetModuleHandle(NULL),_T("STATIC"),&wc);
        wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE); //NULL;//GetSysColorBrush(COLOR_WINDOWFRAME);//(HBRUSH)GetStockObject(_BRUSH); // ウィンドウ背景
        wc.lpszMenuName = NULL;                     // メニュー名
        wc.lpszClassName = OLWindow::classname;// ウィンドウクラス名

        // ウィンドウクラスを登録する
        if( RegisterClassEx( &wc ) == 0 ){ throw GetLastError();}//return ; }
#ifdef CPP11
        //================LABEL================
        wc.cbSize = sizeof(wc);               // 構造体サイズ
        wc.style = CS_HREDRAW | CS_VREDRAW;   // スタイル
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
            case WM_DESTROY:  // ウィンドウを破棄するとき
                PostQuitMessage( 0 );
                return 0;
            }

            // 他のメッセージは、デフォルトの処理を行う
            return DefWindowProc( hWnd, msg, wp, lp );
        };             // ウィンドウプロシージャ
        wc.cbClsExtra = 0;                    // 拡張情報１
        wc.cbWndExtra = 0;                    // 拡張情報２
        wc.hInstance = GetModuleHandle(NULL);                 // インスタンスハンドル
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
        wc.lpszClassName = _T("OLlabel");// ウィンドウクラス名

        // ウィンドウクラスを登録する
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
