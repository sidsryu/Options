# Options
Command line parser using regex.

[license]: LICENSE 

## Todo

Not implemented test cases in advanced-test.cpp.

- [ ] Print out auto generated help description
- [ ] Check extra arguemnt (aka. dynamic sigil)
- [ ] Use shot flag (aka. no separator)
- [ ] Use whole value without quotes
- [ ] Use include whitespace values
- [ ] Use case sensitive keys


## Installation

VS2019 project. Copy all source files in `/src `.


## Usage

Default command line format.
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


## Supported Command Line Formats

Options supports many different formats by runtime settings.

```
# default format
app.exe -log -source filename1 filename2 -target path

# free different sigil
app.exe /log /source filename1 filename2 /target path

# free long sigil
app.exe --help

# no sigil
app.exe launch ip port

# free key-value separtor
app.exe -source=filename

# free serial values separator
app.exe -source filename1, filename2, filename3

# quote values
app.exe -path "../../whitespace name file.txt"
```


## Licensing

This project is licensed under the terms of [the MIT licnese][license].
