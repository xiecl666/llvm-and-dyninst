#include <string>
#include <system_error>
#include <iostream>

#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/IRBuilder.h"
//#include "llvm/IR/BasicBlock.h"
//#include "llvm/IR/Constants.h"
//#include "llvm/IR/LLVMContext.h"
//#include "llvm/IR/Type.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

struct ModifyPass : public FunctionPass
{
  static char ID;
  ModifyPass() : FunctionPass(ID){}
    
  bool runOnFunction(Function &F) override
  {
    printf("------------- runOnFunction:%s --------------\n",F.getName());
    if (F.getName()!="main")
    {
      return false;
    }
//����һ��ȫ�ֱ������Ա��¼ǰ����
    Module *M = F.getParent();
    LLVMContext &context = M->getContext(); 
    Type *intType = Type::getInt32Ty(context);
    GlobalVariable *globalVar = new GlobalVariable(
    /* ģ�� */ *M,
    /* �������� */ intType,
    /* �Ƿ��ǳ��� */ false,
    /* �������� */ GlobalValue::ExternalLinkage,
    /* ��ʼֵ */ ConstantInt::get(intType, -1), // ��ʼֵΪ���� 0
    /* ������ */ "globalVar"
);   
    Type *param1 = Type::getInt32Ty(context);
    Type *param2 = Type::getInt32Ty(context);
    Type *param3 = Type::getInt64Ty(context);
    FunctionType *type1 = FunctionType::get(Type::getVoidTy(context), {}, false);
    FunctionType *type2 = FunctionType::get(Type::getVoidTy(context), {}, false);
    FunctionType *type3 = FunctionType::get(Type::getInt64Ty(context), {}, false);
    FunctionType *type4 = FunctionType::get(Type::getVoidTy(context), {param1,param2,param3}, false);
//����������Ϊ1����ֻ��ǰ�����¼������㣬ĩβ��������
    printf("%d\n",F.size());
    if(F.size()==1)
    {
      FunctionCallee set_event_count=M->getOrInsertFunction("set_event_count", type1);
      FunctionCallee setzero=M->getOrInsertFunction("setzero", type2);
      FunctionCallee readcyc=M->getOrInsertFunction("readcyc", type3);
      FunctionCallee print=M->getOrInsertFunction("print", type4);
      BasicBlock &bb = F.getEntryBlock();
      Instruction *beginInst = &*(bb.getFirstInsertionPt());
      IRBuilder<> builder(beginInst);
      builder.CreateCall(set_event_count, {});
      builder.CreateCall(setzero, {});
      //ĩβ
      Value *bkid = ConstantInt::get(intType, 0);
      Instruction *lastInstruction = bb.getTerminator();
      builder.SetInsertPoint(lastInstruction);
      Value *returnValue1 = builder.CreateCall(readcyc, {});
      builder.CreateCall(print, {bkid,bkid,returnValue1});
      return true;
    }
//����ڿ�ǰ���ʼ�������㡢ȫ�ֱ�����ֵ
    BasicBlock &bb = F.getEntryBlock();
    Instruction *beginInst = &*(bb.getFirstInsertionPt());
    IRBuilder<> builder(beginInst);
    FunctionCallee set_event_count=M->getOrInsertFunction("set_event_count", type1);
    FunctionCallee setzero=M->getOrInsertFunction("setzero", type2);
    builder.CreateCall(set_event_count, /* �����б�Ϊ�� */ {});
    builder.CreateCall(setzero, /* �����б�Ϊ�� */ {});
    Instruction* lastInstruction = bb.getTerminator();
    builder.SetInsertPoint(lastInstruction);
    builder.CreateStore(ConstantInt::get(intType, 0), globalVar);
//��������ǰ���������ȡ�������������  
    FunctionCallee readcyc=M->getOrInsertFunction("readcyc", type3);
    FunctionCallee print=M->getOrInsertFunction("print", type4); 
    int a=1; 
    Function::iterator I = F.begin(), E = F.end();
    I++;E--;
    for (; I != E; ++I)
    {
      
      Value *bkid = ConstantInt::get(intType, a);
      BasicBlock &BB = *I;
      Instruction *beginInst = &*(BB.getFirstInsertionPt());
      builder.SetInsertPoint(beginInst);
      Value *returnValue = builder.CreateCall(readcyc, {});
      Value *globalVarValue = builder.CreateLoad(intType,globalVar,"preid");
      builder.CreateCall(print, {globalVarValue,bkid,returnValue});
      builder.CreateCall(setzero, {});
      Instruction* lastInstruction = BB.getTerminator();
      builder.SetInsertPoint(lastInstruction);
      builder.CreateStore(bkid, globalVar);
      a++;
    }
//�����һ������������������������㡢���
    Value *bkid = ConstantInt::get(intType, a);
    BasicBlock *lastBlock = &F.back();
    beginInst = &*(lastBlock->getFirstInsertionPt());
    builder.SetInsertPoint(beginInst);
    Value *returnValue = builder.CreateCall(readcyc, {});
    Value *globalVarValue = builder.CreateLoad(intType,globalVar,"preid");
    builder.CreateCall(print, {globalVarValue,bkid,returnValue});
    builder.CreateCall(setzero, {});
    lastInstruction = lastBlock->getTerminator();
    builder.SetInsertPoint(lastInstruction);
    Value *returnValue1 = builder.CreateCall(readcyc, {});
    builder.CreateCall(print, {bkid,bkid,returnValue1});
    printf("\n");
    return true;
  }
};

char ModifyPass::ID = 0;

static RegisterPass<ModifyPass> X("modpass", "modify Pass");
// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
//�ɰ��
static void registerModifyPass(const PassManagerBuilder &, legacy::PassManagerBase &PM) 
{
  PM.add(new ModifyPass());
}

static RegisterStandardPasses RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible, registerModifyPass);