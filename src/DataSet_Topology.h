#ifndef INC_DATASET_TOPOLOGY
#define INC_DATASET_TOPOLOGY
#include "DataSet.h"
#include "Topology.h"
/// Hold Topology data
class DataSet_Topology : public DataSet {
  public:
    DataSet_Topology() : DataSet(TOPOLOGY, GENERIC, TextFormat(), 0) {}
    static DataSet* Alloc() { return (DataSet*)new DataSet_Topology();}
    // ----- DataSet functions -------------------
    size_t Size()                  const { return (size_t)top_.Natom(); }
    int Sync()                           { return 1;                    }
    void Info()                    const { top_.Brief(0);               }
    int Allocate(SizeArray const&)       { return 0;                    }
    void Add( size_t, const void* ) {}
    void WriteBuffer(CpptrajFile&, SizeArray const&) const {}
    int Append(DataSet*)                 { return 1;                    }
    // -------------------------------------------
    int LoadTopFromFile(ArgList const&, int);
    void SetTop(Topology const& t) { top_ = t; }
    int StripTop( std::string const& );
    void SetPindex(int p) { top_.SetPindex( p ); }
    Topology* TopPtr() { return &top_; } // FIXME make const?
    Topology const& Top() const { return top_; }
  private:
    Topology top_;
};
#endif
