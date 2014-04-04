print(dynamiccall("D:/Visual Studio 2013/Projects/OLRCLR/Debug/OLRCLR.dll","?callbacktostr@@YGPADP6APADH@Z@Z",
(VarPtr(
	tonativefunc(
	((int i)=>{return ""+i;}),string
	)
	)
)));
exit(0);
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
var hwnd = dynamiccall("user32.DLL","CreateWindowExA",0,"OLWindow","OLWindow",/*-2147483648*/13565952,0,0,100,100,0,0,hinst,0)
dynamiccall("user32.DLL","ShowWindow",hwnd,5);
dynamiccall("user32.DLL","UpdateWindow",hwnd);
	while( true )
	{
		var msg = new MSG();
		BOOL ret = dynamiccall("user32.DLL","GetMessageA", VarPtr(msg), 0, 0, 0 );
		if( ret == 0 || ret == -1 )
		{
			break;
		}
		else
		{
			dynamiccall("user32.DLL","TranslateMessage", VarPtr(msg) );
			dynamiccall("user32.DLL","DispatchMessageA", VarPtr(msg) );
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
var p = new CPerson("‚Ù‚°", 16);
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