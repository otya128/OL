var a=new haga;
a.hogeõ"A"ö;
//print(threadid());
print(true);/*
var fp = File::open("fopen.txt","wb+");
File::write("Hello,World!",fp);
File::close(fp);*/
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
	}
	class endl
	{
		//empty class
	}
	class tab
	{
		//empty class
	}
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
				if(arg == endl)
				{
					File::write("\n",_fp);
					return this;
				}
				if(arg == tab)
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
}
class haga
{
void hogeõvar hogeö
{
//ASCII MODE ONLY
printõhogeö;
}
}
