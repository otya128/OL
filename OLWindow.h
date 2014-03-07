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
        TCHAR filename_full[MAX_PATH];   // �t�@�C����(�t���p�X)���󂯎��̈�
        TCHAR filename[MAX_PATH];        // �t�@�C�������󂯎��̈�
    public:
        //_T("text(*.txt)\0*.txt\0All files(*.*)\0*.*\0\0")
        //txt
        //�t�@�C�����J��
        OpenFileDialog(LPCWSTR strFilter, LPCWSTR DefExt,LPCWSTR Title,OLWindow& Owner)
        {
            memset(&ofn,0,sizeof(ofn));
            memset(&filename_full,0,sizeof(filename_full));
            memset(&filename,0,sizeof(filename));
            ofn.lStructSize = sizeof(ofn);         // �\���̂̃T�C�Y
            ofn.hwndOwner = Owner.hWnd;                  // �R�����_�C�A���O�̐e�E�B���h�E�n���h��
            ofn.lpstrFilter = strFilter; // �t�@�C���̎��
            ofn.lpstrFile = filename_full;         // �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
            ofn.lpstrFileTitle = filename;         // �I�����ꂽ�t�@�C�������󂯎��ϐ��̃A�h���X
            ofn.nMaxFile = sizeof(filename_full);  // lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
            ofn.nMaxFileTitle = sizeof(filename);  // lpstrFileTitle�Ɏw�肵���ϐ��̃T�C�Y
            ofn.Flags = OFN_FILEMUSTEXIST;         // �t���O�w��
            ofn.lpstrTitle = Title; // �R�����_�C�A���O�̃L���v�V����
            ofn.lpstrDefExt = DefExt;           // �f�t�H���g�̃t�@�C���̎��
            //ofn.hInstance = GetModuleHandle(NULL);

            // �t�@�C�����J���R�����_�C�A���O���쐬
        }
        OpenFileDialog(LPCWSTR strFilter, LPCWSTR DefExt,LPCWSTR Title)
        {
            memset(&ofn,0,sizeof(ofn));
            memset(&filename_full,0,sizeof(filename_full));
            memset(&filename,0,sizeof(filename));
            ofn.lStructSize = sizeof(ofn);         // �\���̂̃T�C�Y
            ofn.hwndOwner = NULL;                  // �R�����_�C�A���O�̐e�E�B���h�E�n���h��
            ofn.lpstrFilter = strFilter; // �t�@�C���̎��
            ofn.lpstrFile = filename_full;         // �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
            ofn.lpstrFileTitle = filename;         // �I�����ꂽ�t�@�C�������󂯎��ϐ��̃A�h���X
            ofn.nMaxFile = sizeof(filename_full);  // lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
            ofn.nMaxFileTitle = sizeof(filename);  // lpstrFileTitle�Ɏw�肵���ϐ��̃T�C�Y
            ofn.Flags = OFN_FILEMUSTEXIST;         // �t���O�w��
            ofn.lpstrTitle = Title; // �R�����_�C�A���O�̃L���v�V����
            ofn.lpstrDefExt = DefExt;           // �f�t�H���g�̃t�@�C���̎��

            // �t�@�C�����J���R�����_�C�A���O���쐬
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
