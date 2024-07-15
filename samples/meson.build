fs = import('fs')

fs.copyfile('demo.c', install_dir: 'samples')
fs.copyfile('demo.txt', install_dir: 'samples')

executable(
   'catter', 
   'catter/catter.cpp', 
   dependencies: libservice_dep
)

executable(
   'vscode_opener',
   'vscode_opener/code.cpp',
   dependencies: libservice_dep
)