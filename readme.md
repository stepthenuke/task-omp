Test task OMP
================

This library simplifies creating a central service called `"com.system.sharing"` on DBus. 
This service acts as a middleman, forwarding requests to open files to appropriate registered services.

The library has only one dependency [sdbus-c++](https://github.com/Kistler-Group/sdbus-cpp).

### Building
Firstly, install [sdbus-c++](https://github.com/Kistler-Group/sdbus-cpp), and then:
```
mkdir build
meson setup build
meson compile -C build
```

### Description

The build process creates an executable named `driver` in the `build` folder. This program registers a DBus service called `com.system.sharing`. 
The service provides three methods:

- `void RegisterService(name: String, supportedFormats: Array<String>)`

   `RegisterService` method is used to register a service named `name` that can open files of formats specified in the `supportedFormats`.
- `void OpenFile (path: String)`
   
   After call to `OpenFile`, the driver program finds a suitable registered service and forwards request to it by calling it's own `OpenFile` method. 
   The choice of a service is random. If chosen service is unavailable the next suitable one is selected.

- `void OpenFileUsingService(path: String, service: String)`

   Method allows to call `OpenFile` method of specific `service`.

All services created with library use the same object path `"/"`. 

### Usage
Basic example:
```
int main() {
   const std::string serviceName {"com.qwerty.music"};
   const std::vector<std::string> supportedFormats{".mp3", ".flac", ".m4a"}
   auto onOpenFile [] (const std::string& path) = {
      if (!checkFile(path)) {
         shr::throwError(serviceName, "OpenFile", "File's not passed check");
      }
      process(path);
   };
   shr::SharingService service {};
   service.start();
}
```

For more usage examples see [samples](/samples/)

### Samples

Before starting samples be sure to start `driver`.

1. `catter` — `cat`-like application, which prints file to `stdout`.
Supported formats: ".txt", ".text", ".info", ".c".
```
cd build
./samples/catter
gdbus call -e -d com.system.sharing -o / -m com.system.sharing.OpenFile "$(realpath samples/demo.txt)"
```

2. `vscode_opener`— opens code by executing shell command `code "path to file"`.
Supported formats: ".c", ".cpp", ".h", ".hpp", "" (file without format).
```
cd build
./samples/vscode_opener
gdbus call -e -d com.system.sharing -o / -m com.system.sharing.OpenFile "$(realpath samples/demo.c)"
```
