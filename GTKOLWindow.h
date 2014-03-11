
#pragma comment(lib, "C:/GTK/LIB/glib-2.0.lib")
#pragma comment(lib, "C:/GTK/LIB/gobject-2.0.lib")
#pragma comment(lib, "C:/GTK/LIB/gtk-win32-2.0.lib")
#include <vector>
#include <map>
#define GTK_ENABLE_BROKEN
#include <gtk/gtk.h> 
#include "lang.h"
//__v('KANI')v__//これだとKANI ERROR
__v('ω')v____v('ω')v____v('ω')v____v('ω')v____v('ω')v____v('ω')v____v('ω')v____v('ω')v__
    namespace lang
{
    namespace gtk
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
            void OLWindow::ctor(const gchar* title, int nWidth,int nHeight);
        public:
            WindowEvent OnClick;
            WindowEvent OnSizeChange;
            GtkWidget* window;
            GtkFixed *fixed;
            OLWindow::OLWindow();
            //OLWindow::OLWindow(const char* title);
            OLWindow::OLWindow(const gchar* title,int nWidth,int nHeight);
            //OLWindow(const char* title, int X,int Y,int nWidth,int nHeight);
            void OLWindow::Show();
            virtual ~OLWindow(void);
            void SetFont(TCHAR* name, int size);
            void SetFont(TCHAR* name, int size,bool bold,bool italic,bool underline,bool strike);
            void Close()
            {
                gtk_main_quit( ) ;
                gtk_widget_destroy(window);
            }
            virtual int GetWidth()
            {
                gint width,height;
                gtk_window_get_size((GtkWindow*)window, &width, &height);
                return width;
            }
            virtual int GetHeight()
            {
                gint width,height;
                gtk_window_get_size((GtkWindow*)window, &width, &height);
                return height;
            }
            virtual void SetWidth(int width)
            {
                gtk_window_resize((GtkWindow*)window, width, GetHeight());
            }
            virtual void SetHeight(int height)
            {
                gtk_window_resize((GtkWindow*)window, GetWidth(), height);
            }
            virtual void SetResize(gboolean r)
            {
                gtk_window_set_resizable((GtkWindow*)window, r);
            }
            virtual void SetText(const gchar* text)
            {
                gtk_window_set_title ((GtkWindow*)window, text);
            }
            virtual void GetText(gchar*& text,size_t bufsiz)
            {
                text = (gchar*)gtk_window_get_title((GtkWindow*)window);
            }
            //delete必須
            virtual gchar* GetText()
            {
                gchar* text = (gchar*)gtk_window_get_title((GtkWindow*)window);
                int len = strlen(text);
                gchar* copy = new gchar[len + 1];
                strcpy_s(copy, len, text);
                return copy;
                //int size = SendMessage(hWnd,WM_GETTEXTLENGTH,0,0);
                //TCHAR* buf = new TCHAR[size + 2];//念のため+2
                //GetWindowText(hWnd,buf,size + 1);
                //return buf;
            }
            void* Tag;
        };
        class Button : public OLWindow
        {
            void ctor(OLWindow& parent, const gchar* title, int X, int Y, int nWidth,int nHeight);
        public:
            Button(void);
            Button(OLWindow& parent, const gchar* title, int X, int Y, int width, int height);
        };
        class CheckBox : public Button
        {
            void ctor(OLWindow& parent, const gchar* title, int X, int Y, int nWidth,int nHeight);
        public:
            CheckBox();
            CheckBox(OLWindow& parent,const gchar* title, int X,int Y,int nWidth,int nHeight);
        };
        class TextBox : public OLWindow
        {
            void ctor(OLWindow& parent, const gchar* title, int X, int Y, int nWidth,int nHeight, gboolean multiline);
        public:
            virtual void SetText(gchar* text)
            {
                if(GTK_IS_ENTRY(window))
                {
                    gtk_entry_set_text(GTK_ENTRY(window), text);
                }
                else
                {
                    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(window)), text, strlen(text));
                }
            }
            virtual gchar* GetText()
            {
                if(GTK_IS_ENTRY(window))
                {
                    const gchar* refe = gtk_entry_get_text(GTK_ENTRY(window));
                    int len = GTK_ENTRY(window)->text_length;
                    gchar* ret = new gchar[len + 1];
                    ret[len] = '\0';
                    strncpy_s(ret, len, refe, len);
                    return ret;
                }
                else
                {
                    auto buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(window));
                    GtkTextIter enditer,startiter;
                    gtk_text_buffer_get_end_iter(buffer,&enditer);
                    gtk_text_buffer_get_start_iter(buffer,&startiter);
                    const gchar* refe = gtk_text_buffer_get_text(buffer, &startiter, &enditer, TRUE);
                    int len = strlen(refe);//GTK_TEXT_VIEW(window)->text_length;
                    gchar* ret = new gchar[len + 1];
                    ret[len] = '\0';
                    strncpy_s(ret, len, refe, len);
                    return ret;
                }
            }
            virtual void GetText(gchar* buf, int bufsiz)
            {
                if(GTK_IS_ENTRY(window))
                {
                    const gchar* refe = gtk_entry_get_text(GTK_ENTRY(window));
                    strcpy_s(buf, bufsiz, refe);//, GTK_ENTRY(window)->text_length);
                    buf[bufsiz - 1] = '\0';
                    return;
                }
                else
                {
                    auto buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(window));
                    GtkTextIter enditer,startiter;
                    gtk_text_buffer_get_end_iter(buffer,&enditer);
                    gtk_text_buffer_get_start_iter(buffer,&startiter);
                    const gchar* refe = gtk_text_buffer_get_text(buffer, &startiter, &enditer, TRUE);
                    strcpy_s(buf, bufsiz, refe);
                    buf[bufsiz - 1] = '\0';
                    return;
                }
            }
            TextBox(void);
            TextBox(OLWindow& parent, const gchar* title, int X, int Y, int width, int height, gboolean multiline);
        };
        class OpenFileDialog
        {
            GtkWidget* ofn;
            gchar* filename_full;   // ファイル名(フルパス)を受け取る領域
            gchar* filename;        // ファイル名を受け取る領域
        public:
            //_T("text(*.txt)\0*.txt\0All files(*.*)\0*.*\0\0")
            //txt
            //ファイルを開く
            OpenFileDialog(gchar* strFilter, gchar* DefExt,gchar* Title,OLWindow& Owner)
            {
                ofn = gtk_file_chooser_dialog_new (Title,
                    GTK_WINDOW (Owner.window),
                    GTK_FILE_CHOOSER_ACTION_OPEN,
                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                    NULL);

            }
            OpenFileDialog(gchar* strFilter, gchar* DefExt,gchar* Title)
            {
                ofn = gtk_file_chooser_dialog_new (Title,
                    NULL,
                    GTK_FILE_CHOOSER_ACTION_OPEN,
                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                    NULL);
            }
            gint Show()
            {
                gint response;
                response = gtk_dialog_run (GTK_DIALOG (ofn));
                filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (ofn));
                filename_full = filename;
                gtk_widget_destroy( ofn) ;
                return response;
            }
            gchar* GetFileName()
            {
                return filename_full;
            }
            ~OpenFileDialog()
            {
                g_free(filename);
            }
        };
        class Label : public OLWindow
        {
        public:
            Label(){}
            Label(OLWindow& parent,const gchar* title, int X,int Y,int nWidth,int nHeight);
        };

    }
}
using namespace lang::gtk;
