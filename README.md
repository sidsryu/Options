# Options
Command line parser using regex.


## Todo

Not implemented test cases in advanced-test.cpp.

- [ ] Print out auto generated help description
- [ ] Check extra arguemnt (aka. dynamic switch)
- [ ] Use shot switch (aka. no key-value separator)
- [ ] Use whole value without quotes
- [ ] Use include whitespace values
- [ ] Use case sensitive keys


## Installation

VS2019 project. Copy all source files in `/src `.


## Usage

Defaul command line format.
```
app.exe -log -file filename1 filename2 -path foldername
```

Sample code for parsing above command line.
```C++
bool isLog;
std::vector<std::wstring> fileList;
std::wstring foldername;


Options options;		

options.Add(L"log",  L"enable log", isLog);
options.Add(L"file", L"source filenames", fileList);
options.Add(L"path", L"a destination folder", foldername);


options.Parser(commandLine);

if (isLog) { /* process a log */ }
if (0 < fileList.size()) { /* process source files */ }
if (!foldername.empty()) { /* process a destination folder */ }
```


## Licensing

This project is licensed under the terms of [the MIT licnese][license].
