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