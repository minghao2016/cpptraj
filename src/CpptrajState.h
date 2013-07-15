#ifndef INC_CPPTRAJSTATE_H
#define INC_CPPTRAJSTATE_H
#include "TrajinList.h"
#include "TrajoutList.h"
#include "FrameList.h"
#include "TopologyList.h"
#include "DataSetList.h"
#include "DataFileList.h"
#include "ActionList.h"
#include "AnalysisList.h"
/// Hold all cpptraj state data
class CpptrajState {
  public:
    CpptrajState() : debug_(0), showProgress_(true), exitOnError_(true), nrun_(0) {}
    // TODO: Change to &
    TopologyList* PFL()     { return &parmFileList_; }
    FrameList* FL()         { return &refFrames_;    }
    DataSetList* DSL()      { return &DSL_;          }
    DataFileList* DFL()     { return &DFL_;          }
    void SetNoExitOnError() { exitOnError_ = false;  }
    void SetNoProgress()    { showProgress_ = false; }
    int Debug()       const { return debug_;         }
    int Nrun()        const { return nrun_;          }
    bool Empty()      const { return (actionList_.Empty() && analysisList_.Empty()); }
    void RunAnalyses()      { analysisList_.DoAnalyses(); }
    inline int AddTrajout( ArgList& );
    inline int AddTrajin( ArgList& );
    inline int AddReference( ArgList& );
    inline int AddAction( DispatchObject::DispatchAllocatorType, ArgList& );
    inline int AddAnalysis( DispatchObject::DispatchAllocatorType, ArgList& );
    int ListAll(ArgList&);
    int SetListDebug(ArgList&);
    int ClearList(ArgList&);
    int Run();
    /// Write all DataFiles
    void MasterDataFileWrite();
  private:
    /// Types of lists
    enum ListType {
      L_ACTION = 0, L_TRAJIN, L_REF, L_TRAJOUT, L_PARM, L_ANALYSIS,
      L_DATAFILE, L_DATASET, N_LISTS
    };
    std::vector<bool> ListsFromArg(ArgList&, bool);

    int RunNormal();
    int RunEnsemble();
    // -------------------------------------------
    /// List of parameter files 
    TopologyList parmFileList_;
    /// List of reference coordinate files
    FrameList refFrames_;
     /// List of generated data sets
    DataSetList DSL_;
    /// List of datafiles that data sets will be written to
    DataFileList DFL_;
    /// List of input trajectory files
    TrajinList trajinList_;
    // -------------------------------------------
    /// List of actions to be performed each frame
    ActionList actionList_;
    /// List of output trajectory files 
    TrajoutList trajoutList_;
    // -------------------------------------------
    /// List of analyses to be performed on datasets
    AnalysisList analysisList_;
    
    typedef std::vector<ArgList> ArgsArray;
    /// Array of trajout args for setting up ensemble trajout.
    ArgsArray trajoutArgs_;
    /// State debug level
    int debug_;
    /// Display Progress bar during run
    bool showProgress_;
    /// If true cpptraj will exit if errors are encountered instead of trying to continue
    bool exitOnError_;
    /// Number of times Run() has been called.
    int nrun_;
};
// ----- INLINE FUNCTIONS ------------------------------------------------------
// CpptrajState::AddTrajout()
int CpptrajState::AddTrajout( ArgList& argIn ) {
  // For setting up ensemble later, save trajout arg.
  trajoutArgs_.push_back( argIn );
  return trajoutList_.AddTrajout( argIn, parmFileList_ );
}
// CpptrajState::AddTrajin()
int CpptrajState::AddTrajin( ArgList& argIn ) {
  if ( trajinList_.AddTrajin( argIn, parmFileList_ ) ) return 1;
  DSL_.SetMax( trajinList_.MaxFrames() );
  return 0;
}
// CpptrajState::AddReference()
int CpptrajState::AddReference( ArgList& argIn ) {
  return refFrames_.AddReference(argIn, parmFileList_);
}

int CpptrajState::AddAction( DispatchObject::DispatchAllocatorType Alloc, ArgList& argIn ) {
  return actionList_.AddAction( Alloc, argIn, &parmFileList_, &refFrames_, &DSL_, &DFL_ );
}

int CpptrajState::AddAnalysis( DispatchObject::DispatchAllocatorType Alloc, ArgList& argIn ) {
  return analysisList_.AddAnalysis( Alloc, argIn, &parmFileList_, &DSL_, &DFL_ );
}
#endif