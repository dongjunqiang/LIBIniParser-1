windows提供了ini配置文件操作的API， GetPrivateProfileString， WritePrivateProfileString等，但是
这些API只能用在windows平台。

目前INI配置文件也存在很多第三方库，比如inifile，LibINI， UltraLightINIParser，IniParser， minini，
simpleini等，但是这些库很多都实现复杂，功能也未必符合需求， 接口也不容易操作。

所以，自己写了一个INI文件的解析类，功能很强大， 接口非常简单，支持section重名， 支持一个section下有重名key。
还提供数据结构方便使用者获取任何的信息。采用C++语言实现。