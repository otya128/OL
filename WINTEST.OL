print((-(+(-1))),"\t",OL::Math.cos(1));
//new hoge();exit();class hoge : OL::GUI::Window{hoge(){base.ctor(this,512,512)base.Show()}}
//HiddenFaxWindow????????????????????????????
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
print("\n",OL::List);
var list = new OL::List();
for(int i=0;i<31;i++)
{
	list.Add(i);
}
var enumr = list.GetEnumerator();
foreach(var a in list)
{
	print(a);
}
//scope add(a)
//scope::run local langFunction runenum
//list.defined && is Func GetEnumerator
//.enum = list.call(GetEnumerator)
//

while(enumr.MoveNext())
{
	print(enumr.Current(),"\t")
}
/*
for(int i=0;i<31;i++)
{
	list[i] = i*2;
	print(list[i],"\t");
	//for(;;){}
}*/
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
var font = new OL::GUI::Font("ÇlÇr ñæí©",20,true,true,true,true);
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

class Console
{
	static void WriteLine(string arg)
	{
		OL::Console << arg;
		return WriteLine;
	}
}
/*
class haga
{
static var lf = "\n";
void hogeÅıvar hogeÅˆ
{
//ASCII MODE ONLY
printÅıhogeÅˆ;
}
}
*/