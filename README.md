CLC -- Containers library in C

# list
Doubly linked list implementation in C

Create static lib <br>
$ gcc -c -o libdlist lib/container/list.c -I include/<br>
$ ar rcs libdlist.a libdlist<br>
Use lib<br>
$ gcc -o demo main.c -L. -ldlist -I include/<br>
Run<br>
$ ./demo<br>
