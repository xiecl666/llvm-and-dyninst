//����pass
clang `llvm-config --cxxflags` -Wl,-znodelete -fno-rtti -fPIC -shared modify.cpp -o LLVMModify.so `llvm-config --ldflags`
-Wl,-znodelete: ���ѡ�������������ֹɾ����̬����δʹ�õķ��š�
-fno-rtti: ���ѡ�����������ʱ������Ϣ��RTTI�����Լ������ɵĴ����С��
-fPIC: ���ѡ������λ�ö����Ĵ��룬���ڶ�̬���ӡ�
-shared: ���ѡ����߱���������һ������⣬Ҳ���Ƕ�̬���ӿ⡣
`llvm-config --ldflags`: ������ֻὫ llvm-config --ldflags ����������Ϊ����ѡ��������� LLVM ���������Ϣ��

//������庯����Դ�ļ��б���
clang -flegacy-pass-manager -g -Xclang -load -Xclang ./LLVMModify.so test.c -o testmodified

//������ڿ��Լ�-cʹ��ֻ���벻����


//�鿴�޸ĺ��IR
clang �CO0 �CS �Cemit-llvm sample.c �Co sample.ll
opt -load (path_to_.so_file)/FuncBlockCount.so -modpass sample.ll

-modpass Ϊ�������static RegisterPass<ModifyPass> X("modpass", "modify Pass");