namespace WIN32
{
class WNDCLASSEXA : OL::ArrayBuffer
{
	WNDCLASSEXA()
	{
		base.ctor(48);
	}
    UINT        cbSize
	{get{return this[0,int]}set{this[0]=value}}
    /* Win 3.x */
    UINT        style
	{get{return this[4,int]}set{this[4]=value}}
    WNDPROC     lpfnWndProc
	{get{return this[8,int]}set{this[8]=value}}
    int         cbClsExtra
	{get{return this[12,int]}set{this[12]=value}}
    int         cbWndExtra
	{get{return this[16,int]}set{this[16]=value}}
    HINSTANCE   hInstance
	{get{return this[20,int]}set{this[20]=value}}
    HICON       hIcon
	{get{return this[24,int]}set{this[24]=value}}
    HCURSOR     hCursor
	{get{return this[28,int]}set{this[28]=value}}
    HBRUSH      hbrBackground
	{get{return this[32,int]}set{this[32]=value}}
    LPCSTR      lpszMenuName
	{get{return this[36,int]}set{this[36]=value}}
    LPCSTR      lpszClassName
	{get{return this[40,int]}set{this[40]=value}}
    /* Win 4.0 */
    HICON       hIconSm
	{get{return this[44,int]}set{this[44]=value}}
}
class MSG : OL::ArrayBuffer
{
	MSG()
	{
		base.ctor(40);
	}
	var        hwnd
	{
		get
		{
			return this[0,int]
		}
		set
		{
			this[0]=value
		}
	}
    UINT        message
	{
		get
		{
			return this[4,int]
		}
	}
    WPARAM      wParam
	{
		get
		{
			return this[8,int]
		}
	}
    LPARAM      lParam
	{
		get
		{
			return this[16,int]
		}
	}
    DWORD       time
	{
		get
		{
			return this[24,int]
		}
	}
    POINT       ptX
	{
		get
		{
			return this[32,int]
		}
	}
	POINT       ptY
	{
		get
		{
			return this[40,int]
		}
	}
}
static hInst = dynamiccall("KERNEL32.DLL","GetModuleHandleA",0);
static DefWindowProc = getnativefunction("user32","DefWindowProcA");
static GetMessage = getnativefunction("user32","GetMessageA");
static TranslateMessage = getnativefunction("user32","TranslateMessage");
static DispatchMessage = getnativefunction("user32","DispatchMessageA");
static RegisterClassEx = getnativefunction("user32","RegisterClassExA");
static ShowWindow = getnativefunction("user32","ShowWindow");
static UpdateWindow = getnativefunction("user32","UpdateWindow");
static CreateWindowExA = getnativefunction("user32","CreateWindowExA");
static GetSysColorBrush = getnativefunction("user32","GetSysColorBrush")
static COLOR_BTNFACE = 15;
static NULL = 0;
static LRESULT = int;
static HWND = int;
static UINT = int;
static WPARAM = int;
static LPARAM = int;
static BOOL = int;
}
