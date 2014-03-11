#include "stdafx.h"
#include "GTKOLWindow.h"
namespace lang
{
    namespace gtk
    {
        std::map<GtkWidget*,OLWindow*> windowmap;
        OLWindow::OLWindow()
        {
        }
        OLWindow::OLWindow(const gchar* title,int nWidth,int nHeight)
        {
            ctor(title,nWidth,nHeight);
        }
        void OLWindow::ctor(const gchar* title, int nWidth,int nHeight)
        {
            gtk_init(0, (char***)&"");
            window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
            gtk_window_set_title ((GtkWindow*)window, title);
            gtk_widget_set_size_request (window, nWidth, nHeight);
            g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
            windowmap[window] = this;
            fixed = (GtkFixed*)gtk_fixed_new();//g_object_set(G_OBJECT(window), "objproperty", gtk_fixed_new(), NULL);
            //g_object_set_property
            gtk_container_add(GTK_CONTAINER(window), (GtkWidget*)fixed);
            g_signal_connect(window,"destroy",gtk_main_quit,NULL);
        }
        OLWindow::~OLWindow(void)
        {
        }
        void OLWindow::Show()
        {
            gtk_widget_show_all(window);
            gtk_main();
        }
        Button::Button(void)
        {
        }
        void onclick_callback( GtkWidget *widget, gpointer data)
        {
            ((OLWindow*)data)->OnClick((OLWindow*)data);
        }
        void Button::ctor(OLWindow& parent, const gchar* title, int X, int Y, int nWidth,int nHeight)
        {
            //GValue a;
            //GtkWidget* con;
            //gtk_widget_style_get_property(window, "fixed", &a);//gtk_fixed_new();
            //con = (GtkWidget*)&a;
            //GValue a;
            //GtkWidget *con;
            //g_object_get(G_OBJECT(parent.window), "objproperty", &a, NULL);
            //con = (GtkWidget*)&a;
            //gtk_container_add(GTK_CONTAINER(parent.window), con);
            //gtk_container_add(GTK_CONTAINER(parent.window), con);
            window = gtk_button_new_with_label(title);
            gtk_widget_set_size_request(window, nWidth, nHeight);
            //gtk_widget_set_usize(window, nWidth, nHeight);
            //GtkRequisition req = {nWidth, nHeight};
            //gtk_widget_size_request(window, &req);
            //gtk_widget_set_uposition(window, X, Y);
            gtk_fixed_put(/*GTK_FIXED(con)*/parent.fixed, window, X, Y);
            //gtk_widget_set_resizable(
            //gtk_widget_resize
            //this->SetHeight(nHeight);
            windowmap[window] = this;
            gtk_signal_connect (GTK_OBJECT (window), "clicked",
                GTK_SIGNAL_FUNC (onclick_callback), (gpointer) this);

        }
        Button::Button(OLWindow& parent, const gchar* title, int X, int Y, int width, int height)
        {
            ctor(parent, title, X, Y, width, height);
        }
        CheckBox::CheckBox(void)
        {
        }
        void CheckBox::ctor(OLWindow& parent, const gchar* title, int X, int Y, int nWidth,int nHeight)
        {
            window = gtk_check_button_new_with_label(title);
            gtk_widget_set_size_request(window, nWidth, nHeight);
            gtk_fixed_put(parent.fixed, window, X, Y);
            windowmap[window] = this;
            gtk_signal_connect (GTK_OBJECT (window), "clicked",
                GTK_SIGNAL_FUNC (onclick_callback), (gpointer) this);

        }
        CheckBox::CheckBox(OLWindow& parent, const gchar* title, int X, int Y, int width, int height)
        {
            ctor(parent, title, X, Y, width, height);
        }
        TextBox::TextBox(void)
        {
        }
        void TextBox::ctor(OLWindow& parent, const gchar* title, int X, int Y, int nWidth,int nHeight, gboolean multiline)
        {
            if(multiline)
            {
                GtkWidget* scr = gtk_scrolled_window_new(NULL, NULL);
                gtk_fixed_put(parent.fixed, scr, X, Y);
                gtk_widget_set_size_request(scr, nWidth, nHeight);
                window = gtk_text_view_new();
                gtk_container_add(GTK_CONTAINER(scr), window);
                gtk_text_view_set_editable(GTK_TEXT_VIEW(window), multiline);
                gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(window)),title, strlen(title));
            }
            else
            {
                window = gtk_entry_new();
                gtk_entry_set_editable(GTK_ENTRY(window), TRUE);
                gtk_entry_append_text(GTK_ENTRY(window), title);
                gtk_widget_set_size_request(window, nWidth, nHeight);
                gtk_fixed_put(parent.fixed, window, X, Y);
            }
            windowmap[window] = this;
            gtk_signal_connect (GTK_OBJECT (window), "clicked", GTK_SIGNAL_FUNC (onclick_callback), (gpointer) this);
        }
        TextBox::TextBox(OLWindow& parent, const gchar* title, int X, int Y, int width, int height, gboolean multiline)
        {
            ctor(parent, title, X, Y, width, height, multiline);
        }
        Label::Label(OLWindow& parent,const gchar* title, int X,int Y,int nWidth,int nHeight)
        {
            window = gtk_label_new(title);
            gtk_widget_set_size_request(window, nWidth, nHeight);
            gtk_fixed_put(parent.fixed, window, X, Y);
            windowmap[window] = this;
        }
    }
}

