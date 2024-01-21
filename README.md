# NJU Compiler of C--

Here is a compiler of C-- developed by Fqr as a project of course "Principles and Techniques of Compilers" in Nanjing University. This compiler is able to compile C-- source code into MIPS assembly code, which could be executed on SPIM Simulator.

### Syntax of C--
Generally speaking, C-- looks like C, but there are some differences:
* ~~You can only define variables at the beginning of funcitons.~~
* ~~All variables are global, which means the variables in different functions are not allowed to have the same name.~~
* You can only use While-loop in C--, For-loop and For-each are not supported.
* ~~Struct and high-dimension array are allowed in the process of syntax check. But they are not supported currectly at the process of generating assembly code.~~
* Pointer and type `char` are not supported, so you cannot handle string using C--.
* Structure and high-dimentsion array can **not** be used as return value and parameters.
* Some designs cause that recursion is not supported currenctly, but it will be fixed in the future.

### Example of C-- Code
```C
struct my_struct{
    int val;
    int list[10];
};

int main(){
    struct my_struct test[10];
    int i=0;
    while(i<10){
        int j=0;
        while(j<10){
            test[i].list[j] = read();
            j=j+1;
        }
        i=i+1;
    }

    i=0;
    while(i<10){
        int j=0;
        test[i].val = 0;
        while(j<10){
            test[i].val = test[i].val + test[i].list[j];
            j=j+1;
        }
        write(test[i].val);
        i=i+1;
    }
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
1. ~~Of course variables should not be global, it is so Anti-human.~~
2. ~~Support of struct and high dimension array.~~
3. Better algorithm to sellect Registers and optimize intermediate reprensentation.
4. More operations of structure and high-dimentsion array, such as using them as parameters.
5. I know there are still a lot of bugs in my project. If you find any of them please contact me, it will be so helpful that I can fix them accordingly.