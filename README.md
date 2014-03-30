OL
=======

OtyaLanguage OL
# Sample
```C#
using Sample;
print("namespace static member:",HELLO,"\n");
print("class static member:",Sample.StaticMember,"\n");
print("create Class Object:");
var sample = new Sample("test\n");
//tryのいらない例外構文
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
```
### Result
```
namespace static member:Hello, World
class static member:Hello, World
create Class Object:test
Hellothrow Exception
hogecatch Exception:hoge{function:ctor}
property:Hello

lang::langRuntimeException - lang::scope::run
private setterにはアクセスできません
場所?:
sample.hoge = "A";//ERROR!!
StackTrace
```
