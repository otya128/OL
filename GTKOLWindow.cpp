#include "stdafx.h"
#include "GTKOLWindow.h"
namespace lang
{
    namespace gtk
    {
        OLWindow::OLWindow()
        {
        }
        OLWindow::OLWindow(const char* title,int nWidth,int nHeight)
        {
            ctor(title,nWidth,nHeight);
        }
        void OLWindow::ctor(const char* title, int nWidth,int nHeight)
        {
            gtk_init(0, (char***)&"");
            window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
            gtk_window_set_title ((GtkWindow*)window, title);
            gtk_widget_set_size_request (window, nWidth, nHeight);
            g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
        }

        void OLWindow::Show()
        {
            gtk_widget_show_all(window);
            gtk_main();
        }
    }
}

