//生成pass
clang `llvm-config --cxxflags` -Wl,-znodelete -fno-rtti -fPIC -shared modify.cpp -o LLVMModify.so `llvm-config --ldflags`
-Wl,-znodelete: 这个选项告诉链接器禁止删除动态库中未使用的符号。
-fno-rtti: 这个选项禁用了运行时类型信息（RTTI），以减少生成的代码大小。
-fPIC: 这个选项生成位置独立的代码，用于动态链接。
-shared: 这个选项告诉编译器生成一个共享库，也就是动态链接库。
`llvm-config --ldflags`: 这个部分会将 llvm-config --ldflags 命令的输出作为链接选项。它包含了 LLVM 库的链接信息。

//如果被插函数在源文件中编译
clang -flegacy-pass-manager -g -Xclang -load -Xclang ./LLVMModify.so test.c -o testmodified

//如果不在可以加-c使其只编译不链接


//查看修改后的IR
clang –O0 –S –emit-llvm sample.c –o sample.ll
opt -load (path_to_.so_file)/FuncBlockCount.so -modpass sample.ll

-modpass 为你的命名static RegisterPass<ModifyPass> X("modpass", "modify Pass");