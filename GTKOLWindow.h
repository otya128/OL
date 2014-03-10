
#pragma comment(lib, "C:/GTK/LIB/glib-2.0.lib")
#pragma comment(lib, "C:/GTK/LIB/gobject-2.0.lib")
#pragma comment(lib, "C:/GTK/LIB/gtk-win32-2.0.lib")
#include <vector>
#include <map>
#include <gtk/gtk.h> 
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
            void OLWindow::ctor(const char* title, int nWidth,int nHeight);
        public:
            GtkWidget* window;
            OLWindow::OLWindow();
            //OLWindow::OLWindow(const char* title);
            OLWindow::OLWindow(const char* title,int nWidth,int nHeight);
            //OLWindow(const char* title, int X,int Y,int nWidth,int nHeight);
            void OLWindow::Show();
        };
    }
}
using namespace lang::gtk;
