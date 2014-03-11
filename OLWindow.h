#if _WIN32
#pragma once
#include <vector>
#include <map>
#include <Windows.h>
#include "lang.h"
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
    public:
        std::vector<EFunc> even;
        Event::Event()
        {
        }
        void operator ()(eventargs<T> er)
        {
            if(even.size())
            for(int i=0;i<even.size();i++)
            {
                even[i](er);
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

    protected:
        void OLWindow::ctor(LPCWSTR title, int X,int Y,int nWidth,int nHeight, LPCWSTR ClassName,long style = WS_OVERLAPPEDWINDOW,HWND parent = NULL);
    public:
        static LPCWSTR classname;
        static std::map<HWND,OLWindow*> windowmap;
        WindowEvent OnClick;
        WindowEvent OnSizeChange;
        OLWindow::OLWindow();
        HINSTANCE hInst;
        HWND hWnd;
        MSG msg;
        WNDPROC proc;
        /*OLWindow(const OLWindow& copy)
        {
            windowmap[copy.hWnd] = this;
        }*/
        virtual void Copy(const OLWindow& copy)
        {
            windowmap[copy.hWnd] = this;
            *this = copy;
        }
        OLWindow(LPCWSTR title);
        OLWindow(LPCWSTR title,int nWidth,int nHeight);
        OLWindow(LPCWSTR title, int X,int Y,int nWidth,int nHeight);
        void OLWindow::Add(OLWindow& wnd);
        void Show();
        void Close();
        virtual ~OLWindow(void);
        void SetFont(TCHAR* name, int size);
        void SetFont(TCHAR* name, int size,bool bold,bool italic,bool underline,bool strike);
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
        //delete�K�{
        TCHAR* GetText()
        {
            int size = SendMessage(hWnd,WM_GETTEXTLENGTH,0,0);
            TCHAR* buf = new TCHAR[size + 2];//�O�̂���+2
            GetWindowText(hWnd,buf,size + 1);
            return buf;
        }
        void* Tag;
        HFONT hFont;
    };

    class Button : public OLWindow
    {
        static LPCWSTR classname;
    public:
        void Copy(const Button& copy)
        {
            windowmap[copy.hWnd] = this;
            *this = copy;
        }
       // Button& operator=(const Button& copy);/*
        /*{
        }*/
        /*Button& operator=(const Button& copy)
        {
            windowmap[copy.hWnd] = this;
            return Button(copy);
        }*/
       // Event<Button*,ButtonEventFunc> OnClick;
        WNDPROC baseWndProc;
        Button(){}
        Button(OLWindow& parent,LPCWSTR title, int X,int Y,int nWidth,int nHeight);
    };
    class CheckBox : public Button
    {
    public:
        void Copy(const CheckBox& copy)
        {
            windowmap[copy.hWnd] = this;
            *this = copy;
        }
       // Event<Button*,ButtonEventFunc> OnClick;
        CheckBox(){}
        CheckBox(OLWindow& parent,LPCWSTR title, int X,int Y,int nWidth,int nHeight);
    };
    class TextBox : public OLWindow
    {
    public:
        void Copy(const TextBox& copy)
        {
            windowmap[copy.hWnd] = this;
            *this = copy;
        }
        TextBox(){}
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
            if(f)
                result = result | ES_MULTILINE | ES_WANTRETURN;
            else
                result &= ~ES_MULTILINE;
            SetWindowLongPtr(this->hWnd,GWL_STYLE,result);
        }
        WNDPROC baseWndProc;
        TextBox(OLWindow& parent,LPCWSTR title, int X,int Y,int nWidth,int nHeight, bool multiline);
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
    class Label : public OLWindow
    {
        public:
        Label(){}
        Label(OLWindow& parent,LPCWSTR title, int X,int Y,int nWidth,int nHeight);
    };
}
#endif
