var a=new haga;
a.hogeõ"A"ö;
//print(threadid());
print(true);
a();
var th = new std::Thread(
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
namespace std
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
}
class haga
{
void hogeõvar hogeö
{
//ASCII MODE ONLY
printõhogeö;
}
}
