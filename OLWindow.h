#pragma once
#include <vector>
#include <map>
#include <Windows.h>
namespace lang
{
    template<class T = void*>
    class eventargs
    {
    public:
        T Arg;
        eventargs(T a) : Arg(a)
        {
        }
        //Button
    };
    class OLWindow;
    class Button;
    typedef void (*EventFunc)(eventargs<void*>);
    typedef void (*WindowEventFunc)(eventargs<OLWindow*>);
    typedef void (*ButtonEventFunc)(eventargs<Button*>);
    template<class T = void*, class EFunc = EventFunc>
    class Event
    {
        std::vector<EFunc> even;
    public:
        Event::Event()
        {
        }
        void operator ()(eventargs<T> er)
        {
            for(auto e : even)
            {
                e(er);
            }
        }
        EFunc operator += (EFunc e)
        {
            even.push_back(e);
            return e;
        }
    };
    typedef Event<OLWindow*,WindowEventFunc> WindowEvent;
    class OLWindow
    {

        static LPCWSTR classname;
    protected:
        void OLWindow::ctor(LPCWSTR title, int X,int Y,int nWidth,int nHeight, LPCWSTR ClassName,long style = WS_OVERLAPPEDWINDOW,HWND parent = NULL);
    public:
        static std::map<HWND,OLWindow*> windowmap;
        WindowEvent OnClick;
        WindowEvent OnSizeChange;
        OLWindow::OLWindow();
        HINSTANCE hInst;
        WNDCLASSEX wc;
        HWND hWnd;
        MSG msg;
        WNDPROC proc;
        OLWindow(LPCWSTR title);
        OLWindow(LPCWSTR title,int nWidth,int nHeight);
        OLWindow(LPCWSTR title, int X,int Y,int nWidth,int nHeight);
        void OLWindow::Add(OLWindow& wnd);
        void Show();
        void Close();
        ~OLWindow(void);
        LONG GetWidth()
        {
            RECT rc;
            GetClientRect(hWnd, &rc);
            return rc.right;
        }
        LONG GetHeight()
        {
            RECT rc;
            GetClientRect(hWnd, &rc);
            return rc.bottom;
        }
        LONG SetWidth(LONG width);
        /*{
        RECT rc;
        GetClientRect(hWnd, &rc);
        rc.right = width;
        AdjustWindowRectEx(&rc, GetWindowLong(hWnd,GWL_STYLE),(BOOL)GetMenu(hWnd),GetWindowLong(hWnd,GWL_EXSTYLE));
        return rc.right;
        }*/
        LONG SetHeight(LONG height);
        void SetResize(BOOL r);
        void SetText(const TCHAR* text)
        {
            SetWindowText(hWnd, text);
        }
        int GetText(TCHAR* text,size_t bufsiz)
        {
            return GetWindowText(hWnd,text,bufsiz);
        }
    };
    class Button : public OLWindow
    {
        static LPCWSTR classname;
    public:
        Event<Button*,ButtonEventFunc> OnClick;
        WNDPROC baseWndProc;
        Button(OLWindow& parent,LPCWSTR title, int X,int Y,int nWidth,int nHeight);
    };

    class TextBox : public OLWindow
    {
    public:
        BOOL SetReadOnly(BOOL fr)
        {
            return SendMessage(this->hWnd, EM_SETREADONLY, fr,0);
        }
        BOOL GetReadOnly()
        {
            auto result = GetWindowLongPtr(this->hWnd,GWL_STYLE);//ES_READONLY
            return !!(result & ES_READONLY);
        }
        BOOL GetMultiLine()
        {
            auto result = GetWindowLongPtr(this->hWnd,GWL_STYLE);//ES_READONLY
            return !!(result & ES_MULTILINE);
        }
        void SetMultiLine(BOOL f)
        {
            auto result = GetWindowLongPtr(this->hWnd,GWL_STYLE);
            if(f)result &= ES_MULTILINE;
            else
                result &= ~ES_MULTILINE;
            SetWindowLongPtr(this->hWnd,GWL_STYLE,result);
        }
        WNDPROC baseWndProc;
        TextBox(OLWindow& parent,LPCWSTR title, int X,int Y,int nWidth,int nHeight);
    };
    class OpenFileDialog
    {
        OPENFILENAME ofn;
        TCHAR filename_full[MAX_PATH];   // ファイル名(フルパス)を受け取る領域
        TCHAR filename[MAX_PATH];        // ファイル名を受け取る領域
    public:
        //_T("text(*.txt)\0*.txt\0All files(*.*)\0*.*\0\0")
        //txt
        //ファイルを開く
        OpenFileDialog(LPCWSTR strFilter, LPCWSTR DefExt,LPCWSTR Title,OLWindow& Owner)
        {
            memset(&ofn,0,sizeof(ofn));
            memset(&filename_full,0,sizeof(filename_full));
            memset(&filename,0,sizeof(filename));
            ofn.lStructSize = sizeof(ofn);         // 構造体のサイズ
            ofn.hwndOwner = Owner.hWnd;                  // コモンダイアログの親ウィンドウハンドル
            ofn.lpstrFilter = strFilter; // ファイルの種類
            ofn.lpstrFile = filename_full;         // 選択されたファイル名(フルパス)を受け取る変数のアドレス
            ofn.lpstrFileTitle = filename;         // 選択されたファイル名を受け取る変数のアドレス
            ofn.nMaxFile = sizeof(filename_full);  // lpstrFileに指定した変数のサイズ
            ofn.nMaxFileTitle = sizeof(filename);  // lpstrFileTitleに指定した変数のサイズ
            ofn.Flags = OFN_FILEMUSTEXIST;         // フラグ指定
            ofn.lpstrTitle = Title; // コモンダイアログのキャプション
            ofn.lpstrDefExt = DefExt;           // デフォルトのファイルの種類
            //ofn.hInstance = GetModuleHandle(NULL);

            // ファイルを開くコモンダイアログを作成
        }
        OpenFileDialog(LPCWSTR strFilter, LPCWSTR DefExt,LPCWSTR Title)
        {
            memset(&ofn,0,sizeof(ofn));
            memset(&filename_full,0,sizeof(filename_full));
            memset(&filename,0,sizeof(filename));
            ofn.lStructSize = sizeof(ofn);         // 構造体のサイズ
            ofn.hwndOwner = NULL;                  // コモンダイアログの親ウィンドウハンドル
            ofn.lpstrFilter = strFilter; // ファイルの種類
            ofn.lpstrFile = filename_full;         // 選択されたファイル名(フルパス)を受け取る変数のアドレス
            ofn.lpstrFileTitle = filename;         // 選択されたファイル名を受け取る変数のアドレス
            ofn.nMaxFile = sizeof(filename_full);  // lpstrFileに指定した変数のサイズ
            ofn.nMaxFileTitle = sizeof(filename);  // lpstrFileTitleに指定した変数のサイズ
            ofn.Flags = OFN_FILEMUSTEXIST;         // フラグ指定
            ofn.lpstrTitle = Title; // コモンダイアログのキャプション
            ofn.lpstrDefExt = DefExt;           // デフォルトのファイルの種類

            // ファイルを開くコモンダイアログを作成
        }
        BOOL Show()
        {
            return GetOpenFileName( &ofn );
        }
        TCHAR* GetFileName()
        {
            return filename_full;
        }
    };
}
