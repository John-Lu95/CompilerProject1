//===- BasicBlockHist.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "BasicBlockHist World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"

#include <map>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
using namespace llvm;

#define DEBUG_TYPE "BasicBlockHist"

STATISTIC(BasicBlockHistCounter, "Counts number of functions greeted");

namespace {
  // BasicBlockHist - The first implementation, without getAnalysisUsage.
  struct BasicBlockHist : public ModulePass {
    static char ID; // Pass identification, replacement for typeid
    BasicBlockHist() : ModulePass(ID) {}

    bool runOnModule(Module &M) override {

     int all_total=0;
     int all_load=0;
     int all_blocks=0;
     float s_avrg=0.0;
     map<int,int> all_over_Hist,load_over_Hist;
     map<int,int>::iterator iter;

     Module::iterator fa=M.begin(),fe=M.end();
     for(;fa!=fe;fa++)
     {
      cerr << "\n\nBasicBlockHist: ";
      cerr <<(string)(fa->getName()) << '\n';

      map<int,int> Hist;
      float avg=0.0;
      int total=0;
      /* count all instructions*/
      cerr<<"All instructions"<<'\n';
      
      for(BasicBlock &bb : *fa)
      {
        if(bb.size()!=0)
        {
          ++Hist[bb.size()];
          total+=bb.size();
        }
      }

      for(iter = Hist.begin();iter!=Hist.end();iter++)
      {
        cerr<<iter->first<<" : "<<iter->second<<'\n';
      }

      all_over_Hist.insert(Hist.begin(),Hist.end());
      all_total+=total;

      avg=(float)total/fa->size();
      cerr<<"avg :"<<fixed<<setprecision(2)<<avg<<'\n';

        /*count load instruction*/
        Hist.clear();
        total=0;
        for(BasicBlock &bb : *fa)
        {
          int bb_size=0;
          for(Instruction &itr : bb)
          {
            if(itr.getOpcode()==31) /* whether this instr is load-opcode 31*/
            {
              bb_size++;
              total++;
            }
          }
          if(bb_size!=0)
            ++Hist[bb_size];
        }
        cerr<<"Load Only"<<'\n';
        for(iter = Hist.begin();iter!=Hist.end();iter++)
        {
          cerr<<iter->first<<" : "<<iter->second<<'\n';
        }

        load_over_Hist.insert(Hist.begin(),Hist.end());
        all_load+=total;

        avg=(float)total/fa->size();
        cerr<<"avg :"<<fixed<<setprecision(2)<<avg<<'\n';

        all_blocks+=fa->size();
     }
     cerr<<"\nSUMMARY\n";
     cerr<<"All instructions"<<endl;
     for(iter = all_over_Hist.begin();iter!=all_over_Hist.end();iter++)
      {
        cerr<<iter->first<<" : "<<iter->second<<'\n';
      }
     s_avrg=(float)all_total/all_blocks;
     cerr<<"avg :"<<fixed<<setprecision(2)<<s_avrg<<"\n";

     cerr<<"Load only"<<endl;
     for(iter = load_over_Hist.begin();iter!=load_over_Hist.end();iter++)
      {
        cerr<<iter->first<<" : "<<iter->second<<'\n';
      }
     s_avrg=(float)all_load/all_blocks;
     cerr<<"avg :"<<fixed<<setprecision(2)<<s_avrg<<"\n";


     
     return false;
    }
  };
}

char BasicBlockHist::ID = 0;
static RegisterPass<BasicBlockHist> X("BasicBlockHist", "BasicBlockHist World Pass");

namespace {
  // BasicBlockHist2 - The second implementation with getAnalysisUsage implemented.
  struct BasicBlockHist2 : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    BasicBlockHist2() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      ++BasicBlockHistCounter;
      cerr << "BasicBlockHist: ";
      cerr<<(string)(F.getName())<< '\n';
      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesAll();
    }
  };
}

char BasicBlockHist2::ID = 0;
static RegisterPass<BasicBlockHist2>
Y("BasicBlockHist2", "BasicBlockHist World Pass (with getAnalysisUsage implemented)");
