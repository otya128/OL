//ADD char,wchar type

using OL::GUI;
var form1 = new Window("Hello",256,128);
var button1 = new Button(form1,"Button",0,0,128,24);
var button2 = new Button(form1,"Click!",0,40,128,24);
var label1 = new Label(form1,"Hello,World!!",0,24,128,16);
button2.SetOnClick(function(var btn){form1.MessageBox("Click!");});
form1.Show();
var window = OLRuntime::GUI::Window::Create("Hello",256,128);
var button = OLRuntime::GUI::Button::Create(window,"Button",0,0,128,24);
OLRuntime::GUI::Window::SetOnClick(button,function(var btn){OLRuntime::GUI::MessageBox("Click!");});
var button2 = OLRuntime::GUI::Button::Create(window,"Click!",0,40,128,24);
OLRuntime::GUI::Window::SetOnClick(button2,function(var btn){OLRuntime::GUI::MessageBox("Click!");});
var label1 = OLRuntime::GUI::Label::Create(window,"Hello,World!!",0,24,128,16);
OLRuntime::GUI::Window::Show(window);

//var view = function(char a){Console.WriteLine(a);};
//var v = view;
//v('É÷')v(L'É÷')v(L'É÷');//ÇΩÇµÇ©Ç…Å`
Console.WriteLine("aho")("hoge")("huga")("hege")('A')(0)(false)(true)(false)(null)('\n');
//thread(function(){GC();});
//OLRuntime::GC::BackgroundGC();
var a=new haga;
print("\na.static",a.static,"\na",a,"\nnew (a.static)",new (a.static));
a.hogeÅı"\nA\n"Åˆ;
print(threadid(),OL::Thread.ID(),OL::ID,Console);
print(true,haga.lf);/*
var fp = File::open("fopen.txt","wb+");
File::write("Hello,World!",fp);
File::close(fp);*/
OL::Console << "Hello" << OL::tab << "World!!" << OL::endl;
OL::Console<<"Hello,World!!"<<OL::endl;
var file = OL::IO::MakeFile(File::stdout());//new OL::IO::FileStream("fopen.txt","wb+");
file<<("Hello,World!!OL::IO::FileStream");
file.close();
a();
//a();
var th = new OL::Thread(
	function()
	{
		for(;1;)
		{
			print("hello!",threadid(),"\n");
		}
	});
th.Start();
/*
thread(function(){});
thread(function(){for(;1;){print("hello!",threadid(),"\n");}});
thread(function(){for(;1;){print("hello!",threadid(),"\n");}});
thread(function(){for(;1;){print("hello!",threadid(),"\n");}});
thread(function(){for(;1;){print("hello!",threadid(),"\n");}});
thread(function(){for(;1;){print("hello!",threadid(),"\n");}});
thread(function(){for(;1;){print("hello!",threadid(),"\n");}});
thread(function(){for(;1;){print("hello!",threadid(),"\n");}});
thread(function(){for(;1;){print("hello!",threadid(),"\n");}});
thread(function(){for(;1;){print("hello!",threadid(),"\n");}});
thread(function(){for(;1;){print("hello!",threadid(),"\n");}});
thread(function(){for(;1;){print("hello!",threadid(),"\n");}});
for(;1;){print("hi",threadid(),"\n");}
thread(function(){for(;1;){print("thread:",threadid());}});*/
for(;1;){print("thread:",threadid(),"\n");}
namespace OL
{
	namespace GUI
	{
	//îqèä
	//åpè≥ó~ÇµÇ¢
		class Window
		{
			var _window;
			Window(var title, var width, var height)
			{
				this._window = OLRuntime::GUI::Window::Create(title, width, height);
			}
			void Show()
			{
				OLRuntime::GUI::Window::Show(this._window);
			}
			var _OnClick;
			void SetOnClick(func f)
			{
				this._OnClick = f;
				OLRuntime::GUI::Window::SetOnClick(this._window, this._OnClick);
			}
			void MessageBox(string mes)
			{
				OLRuntime::GUI::MessageBox(mes, this._window);
			}
		}
		class Button
		{
			var _window;
			Button(Window window,string title, int X, int Y, int width, int height)
			{
				this._window = OLRuntime::GUI::Button::Create(window._window, title, X, Y, width, height);
			}
			var _OnClick;
			void SetOnClick(func f)
			{
				this._OnClick = f;
				OLRuntime::GUI::Window::SetOnClick(this._window, this._OnClick);
			}
		}
		class Label
		{
			var _window;
			Label(Window window,string title, int X, int Y, int width, int height)
			{
				this._window = OLRuntime::GUI::Label::Create(window._window, title, X, Y, width, height);
			}
			var _OnClick;
			void SetOnClick(func f)
			{
				this._OnClick = f;
				OLRuntime::GUI::Window::SetOnClick(this._window, this._OnClick);
			}
		}
	}
	class Thread
	{
		var func;
		Thread(var f)
		{
			this.func = f;
		}
		void Start()
		{
			thread(this.func);
		}
		void Join()
		{
			thread::join(this.func);
		}
		static String ID()
		{
			return threadid();
		}
	}
	class endl
	{
		//empty class
	}
	class tab
	{
		//empty class
	}
	static Console = OL::IO::MakeFile(File::stdout());
	namespace IO
	{
		FileStream MakeFile(var fp)
		{
			var file = new FileStream;
			file._fp = fp;
			return file;
		}
		class FileStream
		{
			var _fp;
			File(var name,var ac)
			{
				_fp = File::open(name,ac);
			}
			var leftShift(var arg)
			{
				if(arg == OL::endl)
				{
					File::write("\n",_fp);
					return this;
				}
				if(arg == OL::tab)
				{
					File::write("\t",_fp);
					return this;
				}
				File::write(arg,_fp);
				return this;
			}
			var close()
			{
				var result = File::close(this._fp);
				this._fp = 0;
				return result;
			}
			var finalize()
			{
				if(this._fp)
				{
					close();
				}
			}
		}
	}
	class Enumerable
	{
		var GetEnumerator()
		{
			return new Enumerator;
		}
	}
	class Enumerator
	{
		var Current()
		{
			return null;
		}
		int MoveNext()
		{
			return 0;
		}
	}
}
class Console
{
	static void WriteLine(string arg)
	{
		OL::Console << arg;
		return WriteLine;
	}
}
class haga
{
static var lf = "\n";
void hogeÅıvar hogeÅˆ
{
//ASCII MODE ONLY
printÅıhogeÅˆ;
}
}
