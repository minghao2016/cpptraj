#ifndef INC_DATASET_GRIDFLT_H
#define INC_DATASET_GRIDFLT_H
#include "DataSet_3D.h"
#include "Grid.h"
/// Single-precision three-dimensional grid.
class DataSet_GridFlt : public DataSet_3D {
  public:
    DataSet_GridFlt() : DataSet_3D(GRID_FLT, 12, 4) {}
    float& operator[](size_t idx)              { return grid_[idx];          }
    static DataSet* Alloc()       { return (DataSet*)new DataSet_GridFlt();  }
    Grid<float> const& InternalGrid()    const { return grid_; }
    // ----- DataSet functions -------------------
    size_t Size()                        const { return grid_.size();        }
    int Sync()                                 { return 1;                   }
    void Info()                          const { return;                     }
    // ----- DataSet_3D functions ----------------
    int Allocate3D(size_t x,size_t y,size_t z) { return grid_.resize(x,y,z); }
    void Write3D(CpptrajFile&,int,int,int) const;
    double GetElement(size_t x,size_t y,size_t z) const { return (double)grid_.element(x,y,z); }
    double operator[](size_t idx)        const { return (double)grid_[idx];  }
    size_t NX() const { return grid_.NX(); }
    size_t NY() const { return grid_.NY(); }
    size_t NZ() const { return grid_.NZ(); }
    // -------------------------------------------
    /// Type definition of iterator over grid elements.
    typedef Grid<float>::iterator iterator;
    iterator begin() { return grid_.begin(); }
    iterator end()   { return grid_.end();   }
    /// Increment specified grid point by given value.
    inline size_t Increment(Vec3 const&, float, bool&);
    inline void Increment(Vec3 const&, float);
    inline void Increment(size_t,size_t,size_t,float);
    /// \return grid value at specified bin.
    float GridVal(size_t x,size_t y,size_t z) const { return grid_.element(x,y,z); }
    /// \return grid index
    size_t CalcIndex(size_t i, size_t j, size_t k) const {
      return grid_.CalcIndex(i,j,k);
    }
  private:
    Grid<float> grid_;
};
// ----- INLINE FUNCTIONS ------------------------------------------------------
// DataSet_GridFlt::Increment()
size_t DataSet_GridFlt::Increment(Vec3 const& xyz, float f, bool& binned) {
  size_t i,j,k;
  if (CalcBins(xyz[0],xyz[1],xyz[2],i,j,k)) {
    binned = true;
    return grid_.incrementBy(i,j,k,f);
  }
  binned = false;
  return 0UL;
}
// DataSet_GridFlt::Increment()
void DataSet_GridFlt::Increment(Vec3 const& xyz, float f) {
  size_t i,j,k;
  if (CalcBins(xyz[0],xyz[1],xyz[2],i,j,k))
    grid_incrementBy(i,j,k,f); 
}
// DataSet_GridFlt::Increment()
void DataSet_GridFlt::Increment(size_t i, size_t j, size_t k, float f) {
  grid_.incrementBy(i,j,k,f);
}
#endif
