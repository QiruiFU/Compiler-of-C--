## NJU Compiler of C--

Here is a compiler of C-- developed by Fqr as a project of course "Principles and Techniques of Compilers". This compiler is able to compile C-- source code into MIPS assembly code, which could be executed on SPIM Simulator.

### Syntax of C--
Generally speaking, C-- looks like C, but there are some differences:
* You can only define variables at the beginning of funcitons.
* All variables are global, which means the variables in different functions are not allowed to have the same name.
* You can only use While-loop in C--, For-loop and For-each are not supported.
* Struct and high-dimension array are allowed in the process of syntax check. But they are not supported currectly at the process of generating assembly code.
* Pointer and type `char` are not supported, so you cannot handle string using C--.

### Example of C-- Code
To calculate the factorial:
```C
int fac(int a){
    int res = 1;
    while(a>0){
        res = res * a;
        a = a - 1;
    }
    return res;
}

int main(){
    int n;
    n = read();
    write(fac(n));
    return 0;
}
```

### How to use
You can use `make` command to compile project and you will get a `parser` file which is executable.

Use:
```
./parser test.cmm out.s
```
to compile C-- code. `test.cmm` is C-- source code and `out.s` is the name of target file. Then you can run `out.s` on [QtSPIM](http://pages.cs.wisc.edu/~larus/spim.html).

### TODOs
1. Of course variables should not be global, it is so Anti-human.
2. Support of struct and high dimension array.
3. Better algorithm to sellect Registers and optimize intermediate reprensentation.