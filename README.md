The Rustic C language is basically just like c/c++ with minor changes...
The changes are int -> integer and float -> decimal, atleast for now.
Parser probably has errors, if i encounter problems i will try to solve them.
You can create .rc files and write "rc code", with keywords integer and decimal
Just write rc code that's it!

To build rustic-c-compiler.cpp:
use: 'g++ -o rustic-c-compiler.exe rustic-compiler.cpp'

If you don't have g++, install MSYS2 and use pacman -S mingw-w64-x86_64-gcc at the MINGW64 environment.
That's it!
