# Notes
* To add a c/h file to the build
  - create the .c in /Src and .h in /Inc
  - #include the .h in the .c and probably main.c too
  - #include anything needed inside the user.c file (such as main.h to get hal and #defines)
  - go to makefile and add the user.c file to the `C_SOURCES = \` section