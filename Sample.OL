using WIN32;
var wc = new WNDCLASSEXA();
wc.cbSize = 48; 
wc.style = 1 | 2;
wc.lpfnWndProc = tonativefunc(((HWND hWnd, WIN32::UINT msg, WIN32::WPARAM wp, WIN32::LPARAM lp)=>
				{
					return DefWindowProc(hWnd,msg,wp,lp);
				}),
				WIN32::LRESULT);
wc.cbClsExtra = 0;
wc.cbWndExtra = 0;
wc.hInstance = hInst;
wc.hIcon = 0;
wc.hIconSm = 0;
wc.hCursor = 0;
wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);//0;
wc.lpszMenuName = 0;
wc.lpszClassName = "tesaaaat";
RegisterClassEx(VarPtr(wc))
var hwnd = CreateWindowExA(0,"tesaaaat","title",13565952,0,0,100,100,0,0,hInst,0)
WIN32::ShowWindow(hwnd,5);
WIN32::UpdateWindow(hwnd);
var msg = new MSG();
while( true )
{
	BOOL ret = GetMessage(VarPtr(msg), 0, 0, 0 );
	if( ret == 0 || ret == -1 )
	{
		break;
	}
	else
	{
		TranslateMessage(VarPtr(msg) );
		DispatchMessage(VarPtr(msg))
	}
}
exit(0);
/*var wndprocf = getnativefunction("D:/Visual Studio 2013/Projects/DLL/Debug/DLL.dll","?wndprocf@@YGXP6GHHHHH@Z@Z");
wndprocf((
			tonativefunc(
				((int i,int i2,int i3,int i4)=>
				{//てててていいかかまななななららららららきききききみかかみかかみかかみかかみかかみかかみかか
					print(i,'\t',i2,'\t',i3,'\t',i4,'\n');
					return 1;
				}
				),int)));*/
/*dynamiccall("D:/Visual Studio 2013/Projects/DLL/Debug/DLL.dll","?wndprocf@@YGXP6GHHHHH@Z@Z",(
			tonativefunc(
				((int i,int i2,int i3,int i4)=>
				{//てててていいかかまななななららららららきききききみかかみかかみかかみかかみかかみかかみかか
					print(i,'\t',i2,'\t',i3,'\t',i4,'\n');
					return 1;
				}
				),int)));*/
//exit(0);
/*var rettype = new OL::ArrayBuffer(4 * 4)
print('\n',dynamiccall("D:/Visual Studio 2013/Projects/OLRCLR/Debug/OLRCLR.dll",rettype,"?callbackary@@YG?AUarybuftest@@P6A?AU1@U1@@Z@Z",
((
	tonativefunc(
	((rettype i)=>{i[12] = 13;return i;}),rettype
	)
	)
))[12,int],"\n");
print(rettype[4,int])
print(dynamiccall("D:/Visual Studio 2013/Projects/OLRCLR/Debug/OLRCLR.dll","?callbacktostr@@YGPADP6APADH@Z@Z",
((
	tonativefunc(
	((int i)=>{return ""+i;}),string
	)
	)
)));
exit(0);*/
class CArray : OL::ArrayBuffer
{
	CArray(array ary)
	{
		base.ctor(512);
		int size = 0;
		foreach(var i in ary)
		{
			this[size] = i;
			size += sizeof(i);
		}
		this[size] = 0;
		//this[0] = " Hello,NET!";
		//this[4] = 0;
	}
}
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
var hinst = dynamiccall("KERNEL32.DLL","GetModuleHandleA",0);
var DefWindowProc = getnativefunction("user32","DefWindowProcA");
var GetMessage = getnativefunction("user32","GetMessageA");
var TranslateMessage = getnativefunction("user32","TranslateMessage");
var DispatchMessage = getnativefunction("user32","DispatchMessageA");
var LRESULT = int;
var HWND = int;
var UINT = int;
var WPARAM = int;
var LPARAM = int;
var wc = new WNDCLASSEXA();
        wc.cbSize = 48;               // 構造体サイズ
        wc.style = 1 | 2;   // スタイルdynamiccall("user32","DefWindowProcA",i,i2,i3,i4);
        wc.lpfnWndProc = tonativefunc(
				((HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)=>
				{
					return DefWindowProc(hWnd,msg,wp,lp);
				}
				),LRESULT);
		print(wc.lpfnWndProc,"\n")
        wc.cbClsExtra = 0;                    // 拡張情報１
        wc.cbWndExtra = 0;                    // 拡張情報２
        wc.hInstance = hinst;                 // インスタンスハンドル
        wc.hIcon = 0;
        wc.hIconSm = 0;                // 子アイコン
        wc.hCursor = 0;
        wc.hbrBackground = 0; //NULL;//GetSysColorBrush(COLOR_WINDOWFRAME);//(HBRUSH)GetStockObject(_BRUSH); // ウィンドウ背景
        wc.lpszMenuName = 0;                     // メニュー名
        wc.lpszClassName = "tesaaaat";// ウィンドウクラス名

        // ウィンドウクラスを登録する
        print(dynamiccall( "user32","RegisterClassExA",VarPtr(wc )),"\n")
		var hwnd = dynamiccall("user32.DLL","CreateWindowExA",0,"tesaaaat","title",/*-2147483648*/13565952,0,0,100,100,0,0,hinst,0)
dynamiccall("user32.DLL","ShowWindow",hwnd,5);
dynamiccall("user32.DLL","UpdateWindow",hwnd);
		var msg = new MSG();
	while( true )
	{
		BOOL ret = GetMessage(VarPtr(msg), 0, 0, 0 );//dynamiccall("user32.DLL","GetMessageA", VarPtr(msg), 0, 0, 0 );
		if( ret == 0 || ret == -1 )
		{
			break;
		}
		else
		{
			TranslateMessage(VarPtr(msg) );
			DispatchMessage(VarPtr(msg))
		}
	}
dynamiccall("D:/Visual Studio 2013/Projects/OLRCLR/Debug/OLRCLR.dll","?callnet@@YGXPAD0PAPAD@Z","System.Console","WriteLine",VarPtr(new CArray(["{0}, {1}","Hello", ".NET"])));
class CInt : OL::ArrayBuffer
{
	public var Get
	{
		get
		{
			return this[0, int];
		}
		set
		{
			this[0] = value;
		}
	}
}
var ab = new OL::ArrayBuffer(8);
ab[0]="AHO%s";ab[4]="HOGE";var ret = new CInt(4);
ret = dynamiccall("msvcr120",ret,"printf:va_arg",ab);
print(ret.Get)
class Person
{
	private var name;
	private var age;
	public var Name
	{
		get
		{
			return this;
		}
		set
		{
			name = value;
		}
	}
	public var Age
	{
		get
		{
			return age;
		}
		set
		{
			age = value;
		}
	}
	public Person(stirng name_,string age_)
	{
		this.name = name_;
		this.age = age_;
	}
}
class CPerson : OL::ArrayBuffer
{
	public var Name
	{
		get
		{
			return this[0,string];
		}
		set
		{
			this[0] = value;
		}
	}
	public var Age
	{
		get
		{
			return this[4,int];
		}
		set
		{
			this[4] = value;
		}
	}
	public CPerson(stirng name,string age)
	{
		base.ctor(8);
		this.Name = name;
		this.Age = age;
	}
}
var p = new CPerson("ほげ", 16);
print(p.Name, p.Age);
exit();
using Sample;
print("namespace static member:",HELLO,"\n");
print("class static member:",Sample.StaticMember,"\n");
print("create Class Object:");
var sample = new Sample("test\n");
new OL::Thread(()=>
{
	var window = new OL::GUI::Window("hello",100,100)
		var window2 = new OL::GUI::Window("hello",100,100)
	new OL::Thread(()=>
	{
		window2.Show()
		//for(;;){}
	}).Start();
	window.Show()
	//for(;;){}
}).Start();
{
	print("throw Exception\n")
	new Sample();
	catch(OL::Exception ex)
	{
		print(ex._what);
		print("catch Exception:", ex, "\n");
	}
}
print("property:",sample.hoge,"\n");
getline();
sample.hoge = "A";//ERROR!!
namespace Sample
{
	static HELLO = "Hello, World";
	class Sample
	{
		static var StaticMember = HELLO;
		public var hoge
		{
			get
			{
				return "Hello";
			}
			protected set
			{
				print(value);
			}
		}
		Sample(string a)
		{
			print(a,(this.hoge));
		}
		Sample()
		{
			throw new OL::Exception("hoge");
		}
	}
}