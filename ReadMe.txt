﻿git clone http://github.com/otya128/OL.git
cd OL
make
./lang WINTEST.OL STD.OL
試すには
https://github.com/otya128/cpptest/raw/master/exe/lang.exe
と
https://github.com/otya128/cpptest/raw/master/exe/Hello%2CWorld.bat
をDLしてHello,World.batを実行
========================================================================
    コンソール アプリケーション: lang プロジェクトの概要
========================================================================
コマンド引数
-ahogc
	負荷の高いガベージコレクションの実行 規定値:false
-parserresult
	パースされた結果を表示 規定値:false
-gcview
	ガベージコレクションの様子の表示 規定値:false
-errorlevel n
	エラーとして扱われるレベル 規定値:0
-pause
終了後に待機するかどうか
-endpause
終了後に待機するかどうか
-json
jsonファイルとしてパース結果を出力
-e n
nを実行する
-gui
guiでファイルを選択
-prompt
プロンプトモードを明示的に指定
-nologo
OtyaLanguageを表示しない
-stdout file
stdoutの内容をfileに書き込む
-stderr file
stderrの内容をfileに書き込む
-chdir  file
ディレクトリを変更する
この lang アプリケーションは、AppWizard により作成されました。

このファイルには、lang アプリケーションを構成する各ファイルの内容の概要が含まれています。


lang.vcxproj
    これは、アプリケーション ウィザードを使用して生成された VC++ プロジェクトのメイン プロジェクト ファイルです。ファイルを生成した Visual C++ のバージョンに関する情報と、アプリケーション ウィザードで選択されたプラットフォーム、構成、およびプロジェクト機能に関する情報が含まれています。

lang.vcxproj.filters
    これは、アプリケーション ウィザードで生成された VC++ プロジェクトのフィルター ファイルです。このファイルには、プロジェクト内のファイルとフィルターとの間の関連付けに関する情報が含まれています。この関連付けは、特定のノードで同様の拡張子を持つファイルのグループ化を示すために IDE で使用されます (たとえば、".cpp" ファイルは "ソース ファイル" フィルターに関連付けられています)。

.gitignore
    これは、アプリケーション ウィザードで生成された gitのフィルター ファイルです。このファイルには、プロジェクト内のファイルとフィルターとの間の関連付けに関する情報が含まれています。この関連付けは、特定のノードで同様の拡張子を持つファイルのグループ化を示すために IDE で使用されます (たとえば、".cpp" ファイルは "ソース ファイル" フィルターに関連付けられています)。

Class.cpp
Class.h
Function.cpp
Function.h
GC.cpp
GC.h
Object.cpp
Object.h
lang.cpp
lang.h
langException.h
parseObj.cpp
parseObj.h
parser.cpp
parser.h
parserEnum.h
scope.cpp
scope.h
targetver.h
variable.cpp
variable.h
    これは、メインのアプリケーション ソース ファイルです。

/////////////////////////////////////////////////////////////////////////////
その他の標準ファイル :

StdAfx.h, StdAfx.cpp
    これらのファイルは、lang.pch という名前のプリコンパイル済みヘッダー (PCH) ファイルと、StdAfx.obj という名前のプリコンパイル済みの型ファイルをビルドするために使用されます。

/////////////////////////////////////////////////////////////////////////////
その他のメモ :

AppWizard では "TODO:" コメントを使用して、ユーザーが追加またはカスタマイズする必要のあるソース コードを示します。

/////////////////////////////////////////////////////////////////////////////
Windows,Ubuntu,Mac,PearOSで動いた
