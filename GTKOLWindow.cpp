#include "stdafx.h"
#include "GTKOLWindow.h"
namespace lang
{
    namespace gtk
    {
        std::map<GtkWidget*,OLWindow*> OLWindow::windowmap;
        OLWindow::OLWindow()
        {
            fixed = NULL;
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
            auto win = OLWindow::windowmap[widget];
            (win)->OnClick(win);
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
            if(!parent.fixed)
            {
                parent.fixed = (GtkFixed*)gtk_fixed_new();
                gtk_container_add(GTK_CONTAINER(window), (GtkWidget*)parent.fixed);
            }
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
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#define ConvSJistoUtf8_FREE(pDist) delete [] pDist
#else
#define ConvSJistoUtf8_FREE(pDist) 
#endif
namespace lang
{
    namespace gtk
    {
        //copypaste http://www.lab.its55.com/?p=32
        int ConvSJistoUtf8(const char* pSource, char*& pDist )
        {
            #ifndef _WIN32
                pDist = pSource;
                return TRUE;
            #endif
            int pSize = 0;

            //ShiftJIS‚©‚çUTF-16‚Ö•ÏŠ·
            const int nSize = ::MultiByteToWideChar( CP_ACP, 0, (char*)
                pSource, -1, NULL, 0 );

            char* buffUtf16 = new char[ nSize * 2 + 2 ];
            ::MultiByteToWideChar( CP_ACP, 0, (char*)pSource, -1, (wchar_t*)
                buffUtf16, nSize );

            //UTF-16‚©‚çShift-JIS‚Ö•ÏŠ·
            const int nSizeUtf8 = ::WideCharToMultiByte( CP_UTF8, 0, (wchar_t*)
                buffUtf16, -1, NULL, 0, NULL, NULL );
            //if( !pDist ){
                pSize = nSizeUtf8;
                pDist = new char[pSize + 1];
                //delete buffUtf16;
                //return TRUE;
            //}

            char* buffUtf8 = new char[ nSizeUtf8 * 2 ];
            memset( buffUtf8, 0, nSizeUtf8 * 2 );
            ::WideCharToMultiByte( CP_UTF8, 0, (wchar_t*)buffUtf16, -1, (LPSTR)
                buffUtf8, nSizeUtf8, NULL, NULL );

            pSize = strlen( (char*)buffUtf8 );
            memcpy( pDist, buffUtf8, pSize );
            pDist[pSize] = '\0';
            delete buffUtf16;
            delete buffUtf8;

            return TRUE;
        }
        void OLWindow::SetFont(const gchar* name, int size)
        {
            //__v(L'ƒÖ')v__;
            std::stringstream fontname;
            fontname << name << ' ' << size;
            gtk_widget_modify_font(window, pango_font_description_from_string(fontname.str().c_str()));
        }
        void OLWindow::SetFont(const gchar* name, int size,bool bold,bool italic,bool underline,bool strike)
        {
            //__v(L'ƒÖ')v__;
            std::stringstream fontname;
            gchar* utf8;
            ConvSJistoUtf8(name, utf8);
            fontname << utf8 << ' ' << size;
            if(underline)fontname << ' ' << "Underline";
            if(strike)fontname << ' ' << "Strike";
            auto font = pango_font_description_from_string(fontname.str().c_str());
            pango_font_description_set_style(font, italic ? PangoStyle::PANGO_STYLE_ITALIC : PangoStyle::PANGO_STYLE_NORMAL);
            pango_font_description_set_weight(font, bold ? PangoWeight::PANGO_WEIGHT_BOLD : PangoWeight::PANGO_WEIGHT_NORMAL);
            //pango_font_description_set_under
            //pango_font_description_set //PangoAttrType::PANGO_ATTR_UNDERLINE
            gtk_widget_modify_font(window, font);
            ConvSJistoUtf8_FREE(utf8);
        }
        void __MessageBox(const gchar* message)
        {
            auto dialog = gtk_message_dialog_new(NULL, GtkDialogFlags::GTK_DIALOG_MODAL, GtkMessageType::GTK_MESSAGE_INFO, GtkButtonsType::GTK_BUTTONS_OK, message);
            gtk_dialog_run((GtkDialog*)dialog);
            gtk_widget_destroy(dialog);
        }
        void __MessageBox(OLWindow &parent,const gchar* message)
        {
            auto dialog = gtk_message_dialog_new((GtkWindow*)(parent.window), GtkDialogFlags::GTK_DIALOG_MODAL, GtkMessageType::GTK_MESSAGE_INFO, GtkButtonsType::GTK_BUTTONS_OK, message);
            gtk_dialog_run((GtkDialog*)dialog);
            gtk_widget_destroy(dialog);
        }
    }
}

