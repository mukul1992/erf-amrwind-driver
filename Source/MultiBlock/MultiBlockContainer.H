#ifndef MultiBlockContainer_H_
#define MultiBlockContainer_H_

#include <AMReX_NonLocalBC.H>
#include <ERF.H>
#include <amr-wind/incflo.H>

class MultiBlockContainer
{
public:

  // Constructor with vector of parameters
  MultiBlockContainer (const std::vector<amrex::RealBox>& rb_v,
                       std::vector<int> max_level_in_v,
                       const std::vector<amrex::Vector<int>>& n_cell_in_v,
                       std::vector<int> coord_v,
                       const std::vector<amrex::Vector<amrex::IntVect>>& ref_ratios_v,
                       const std::vector<amrex::Array<int,AMREX_SPACEDIM>>& is_per_v,
                       std::vector<std::string> prefix_v,
                       int max_step);

  // Destructor
  ~MultiBlockContainer ();

  // Initialize block data
  void InitializeBlocks ();

  // Set up BoxList vector for use with Communication Meta Data
  void SetBoxLists ();

  // Set up MB Communication Meta Data
  void SetBlockCommMetaData ();

  // Advance blocks
  void AdvanceBlocks ();

  // Fill MFs from other block(s)
  void FillPatchBlocksAE ();

  // Fill AMR-Wind Boundary Regsiter from ERF1
  void CopyERFtoAMRWindBoundaryReg (
    amrex::BndryRegister& receive_br,  amrex::Orientation ori,
    amrex::Real time, const std::string& field);

  void PopulateErfTimesteps(amrex::Real* tsteps);

  // boundary registers for AMR-Wind
  // Only two fields supported for now -- velocity and temperature
  static constexpr int num_fields {2};
  amrex::Vector<amrex::BndryRegister*> bndry1;
  amrex::Vector<amrex::BndryRegister*> bndry2;

  amrex::Real old_bndry_time {0.0};
  amrex::Real new_bndry_time {0.0};

  incflo& get_amrwind () {return amrwind;}

private:
  // TODO: why does amrwind have to come before erf?
  // It seems that not doing so results in a conflict
  // in reading the inputs
  incflo amrwind;
  ERF erf1;

  int m_max_step;
  bool do_two_way_coupling {false};
  int two_way_coupling_frequency {1};
  int erf_to_aw_dl_ratio {1};

  std::vector<amrex::Box> eboxvec_afrome;
  std::vector<amrex::Box> aboxvec_afrome;
  amrex::NonLocalBC::MultiBlockIndexMapping dtos_efroma;
  amrex::NonLocalBC::MultiBlockIndexMapping dtos_afrome;
  std::vector<amrex::NonLocalBC::MultiBlockCommMetaData*> cmd_efroma;
  //std::vector<std::vector<amrex::NonLocalBC::MultiBlockCommMetaData*>> cmd_afrome;
  amrex::Box ebox_efroma;
  amrex::Box abox_efroma;
};

#endif
