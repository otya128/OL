//ADD char,wchar type
if(true)
{
var test = new child;
test.hage();
while(false)
{
print("true");
break;
}
//new OL::Thread(function(){while(true){print(OL::Thread.ID())}}).Start();
}
else
{
print("Ç±Ç±ÇÕé¿çsÇ≥ÇÍÇ»Ç¢");
}
var list = new OL::List();
for(int i=0;i<32;i++)
{
	list.Add(i);
}
list[0];
for(int i=0;i<32;i++)
{
	list[i] = i*2;
	print(list[i],"\t");
	//for(;;){}
}
for(int i=1;i<10;i++)
{
	if(i%2)
	{
		print("continue");
		continue;
	}
	else
	{
		print(i);
	}
	if(i > 5)
	{
		print("break!");
		break;
	}
}
//using OL::GUI;
var form1 = new OL::GUI::Window("Hello",256,128);
var button1 = new OL::GUI::Button(form1,"Button",0,0,128,24);
var button2 = new OL::GUI::Button(form1,"Click!",0,40,128,24);
var label1 = new OL::GUI::Label(form1,"Hello,World!!",0,24,128,16);
var textbox1 = new OL::GUI::TextBox(form1,"",0,64,256,192,true);
var font = new OL::GUI::Font("ÇlÇr ÉSÉVÉbÉN",20,true,true,true,true);
textbox1.SetFont(font);
//textbox1.SetMultiLine(false);
var checkbox1 = new OL::GUI::CheckBox(textbox1,"CheckBox",0,88,128,16);
button2.SetOnClick(function(var btn){form1.MessageBox(textbox1.GetText());});
button1.SetOnClick(function(var btn){textbox1.SetText(textbox1.GetText() + "!");});
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
class child : parent
{
	int hage()
	{
		print("this is ",this);
		print("\nbase is ",base );
		this.base.hage();//êeÉNÉâÉXÇÃä÷êîÇåƒÇ—èoÇ∑
	}
}

class parent
{
	int hage()
	{
		print("\nthis is ",this);
		print("\nbase  is ",base );
	}
}

namespace OL
{
	class List
	{
		private _array;
		private _arraysize;
		private _length;
		List()
		{
			this._array = new Array(16);
			this._length = 0;
			this._arraysize = 16;
		}
		int Count()
		{
			return this._length;
		}
		void Add(var item)
		{
			this._length=this._length+1;
			if(this._arraysize < this._length)
			{
				var resize = new Array(this._arraysize*2);
				//var index = 0;
				for(int index = 0;index < this._arraysize;index++)
				{
					resize[index] = this._array[index];
				}
				this._array = resize;
				this._arraysize = this._arraysize * 2;
			}
			this._array[this._length - 1] = item;		
		}
		var bracket(int index)
		{
			return this._array[index];
		}
		var bracketequal(int index, var item)
		{
			return this._array[index] = item;
		}
	}

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
			void SetFont(Font font)
			{
				OLRuntime::GUI::Window::SetFont(this._window, font.name, font.size, font.bold, font.italic, font.underline, font.strike);
			}
			string GetText()
			{
				return OLRuntime::GUI::Window::GetText(this._window);
			}
			string SetText(string text)
			{
				return OLRuntime::GUI::Window::SetText(this._window, text);
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
			void SetFont(Font font)
			{
				OLRuntime::GUI::Window::SetFont(this._window, font.name, font.size, font.bold, font.italic, font.underline, font.strike);
			}
			string GetText()
			{
				return OLRuntime::GUI::Window::GetText(this._window);
			}
			string SetText(string text)
			{
				return OLRuntime::GUI::Window::SetText(this._window, text);
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
			void SetFont(Font font)
			{
				OLRuntime::GUI::Window::SetFont(this._window, font.name, font.size, font.bold, font.italic, font.underline, font.strike);
			}
			string GetText()
			{
				return OLRuntime::GUI::Window::GetText(this._window);
			}
			string SetText(string text)
			{
				return OLRuntime::GUI::Window::SetText(this._window, text);
			}
		}
		class Font
		{
			Font(string _name, int _size,bool _bold,bool _italic,bool _underline,bool _strike)
			{
				this.name = _name;
				this.size = _size;
				this.bold = _bold;
				this.italic = _italic;
				this.underline = _underline;
				this.strike = _strike;
			}
			string name;
			int size;
			bool bold;
			bool italic;
			bool underline;
			bool strike;
		}
		class TextBox
		{
			var _window;
			TextBox(Window window,string title, int X, int Y, int width, int height,bool multiline)
			{
				this._window = OLRuntime::GUI::TextBox::Create(window._window, title, X, Y, width, height,multiline);
			}
			var _OnClick;
			void SetOnClick(func f)
			{
				this._OnClick = f;
				OLRuntime::GUI::Window::SetOnClick(this._window, this._OnClick);
			}
			void SetReadOnly(bool f)
			{
				OLRuntime::GUI::TextBox::SetReadOnly(this._window,f);
			}
			void SetMultiLine(bool f)
			{
				OLRuntime::GUI::TextBox::SetMultiLine(this._window,f);
			}
			bool GetReadOnly()
			{
				return OLRuntime::GUI::TextBox::GetReadOnly(this._window);
			}
			bool GetMultiLine()
			{
				return OLRuntime::GUI::TextBox::GetMultiLine(this._window);
			}
			void SetFont(Font font)
			{
				OLRuntime::GUI::Window::SetFont(this._window, font.name, font.size, font.bold, font.italic, font.underline, font.strike);
			}
			string GetText()
			{
				return OLRuntime::GUI::Window::GetText(this._window);
			}
			string SetText(string text)
			{
				return OLRuntime::GUI::Window::SetText(this._window, text);
			}
		}
		class CheckBox
		{
			var _window;
			CheckBox(Window window,string title, int X, int Y, int width, int height)
			{
				this._window = OLRuntime::GUI::CheckBox::Create(window._window, title, X, Y, width, height);
			}
			var _OnClick;
			void SetOnClick(func f)
			{
				this._OnClick = f;
				OLRuntime::GUI::Window::SetOnClick(this._window, this._OnClick);
			}
			void SetFont(Font font)
			{
				OLRuntime::GUI::Window::SetFont(this._window, font.name, font.size, font.bold, font.italic, font.underline, font.strike);
			}
			string GetText()
			{
				return OLRuntime::GUI::Window::GetText(this._window);
			}
			string SetText(string text)
			{
				return OLRuntime::GUI::Window::SetText(this._window, text);
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
