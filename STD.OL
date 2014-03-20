
namespace OL
{
	class Math
	{
		static double cos(r r){r x=r*r; return ((((((0.000000002087675699)*x -0.00000027557319223989)*x +0.00024801587301587)*x -0.0013888888888888889)*x +0.041666666666666666)*x -0.5)*x +1.0;}
	}
	class List : Enumerable
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
		var GetEnumerator()
		{
			return new ListEnumerator(this);
		}
	}
	class ListEnumerator : Enumerator
	{
		ListEnumerator(var list)
		{
			this._list = list;
			this._index = 0;
		}
		var _list;
		var _index;
		var _current;
		var Current()
		{
			return _current;
		}
		int MoveNext()
		{
			if(_index > this._list.Count())
			{
				return false;
			}
			this._current = this._list[_index];
			_index = _index + 1;
			return (_index) <= this._list.Count();
		}
	}
	namespace GUI
	{
	//”qŠ
	//Œp³—~‚µ‚¢
		class Control
		{
			var _window;
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
		class Window : Control
		{
			//var _window;
			Window(var title, var width, var height)
			{
				this._window = OLRuntime::GUI::Window::Create(title, width, height);
			}
			void Show()
			{
				OLRuntime::GUI::Window::Show(this._window);
			}
		}
		class Button : Control
		{
			Button(Window window,string title, int X, int Y, int width, int height)
			{
				this._window = OLRuntime::GUI::Button::Create(window._window, title, X, Y, width, height);
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
		class TextBox : Control
		{
			//var _window;
			TextBox(Window window,string title, int X, int Y, int width, int height,bool multiline)
			{
				this._window = OLRuntime::GUI::TextBox::Create(window._window, title, X, Y, width, height,multiline);
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
		}
		class CheckBox : Control
		{
			CheckBox(Window window,string title, int X, int Y, int width, int height)
			{
				this._window = OLRuntime::GUI::CheckBox::Create(window._window, title, X, Y, width, height);
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