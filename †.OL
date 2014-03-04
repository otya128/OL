
Console.WriteLine("aho")("hoge")("huga")("hege");
var a=new haga;
a.hogeõ"A"ö;
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
void hogeõvar hogeö
{
//ASCII MODE ONLY
printõhogeö;
}
}
