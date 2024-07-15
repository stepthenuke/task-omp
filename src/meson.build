src = [
   'common.cpp',
   'registering_service.cpp',
   'service.cpp',
   'sharing_service.cpp'
]

sdbus_dep = dependency(
   'sdbus-c++', 
   required: true
)

libservice = shared_library(
   'dbus_service',
   src,
   version: '0.1337',
   soversion: 0,
   dependencies: [sdbus_dep]
)

libservice_dep = declare_dependency(
   link_with: libservice,
   dependencies: [sdbus_dep],
   include_directories: ['..']
)