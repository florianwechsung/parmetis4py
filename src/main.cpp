#include <parmetis.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

int add(int i, int j) { return i + j; }

namespace py = pybind11;

PYBIND11_MODULE(parmetis4py, m) {
    // m.doc() = R"pbdoc(
    //    Pybind11 example plugin
    //    -----------------------

    //    .. currentmodule:: python_example

    //    .. autosummary::
    //       :toctree: _generate

    //       add
    //       subtract
    //)pbdoc";

    // m.def("add", &add, R"pbdoc(
    //    Add two numbers

    //    Some other explanation about the add function.
    //)pbdoc");

    // m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
    //    Subtract two numbers

    //    Some other explanation about the subtract function.
    //)pbdoc");
    //
    m.def("ParMETIS_V3_PartKway",
          [](std::vector<idx_t> vtxdist, std::vector<idx_t> xadj,
             std::vector<idx_t> adjncy, std::vector<idx_t> vwgt, int nparts) {

              int n_vtxdist = vtxdist.size();
              int n_xadj = xadj.size();
              int n_adjncy = adjncy.size();
              //int n_vwgt = vwgt.size();
              int mpi_rank, mpi_size;
              MPI_Comm comm = MPI_COMM_WORLD;
              MPI_Comm_size(comm, &mpi_size);
              MPI_Comm_rank(comm, &mpi_rank);
              if(nparts == -1) {
                nparts = mpi_size;
              }

              idx_t *adjwgt = NULL;
              idx_t wgtflag = 2;

              idx_t numflag = 0;
              idx_t ncon = 1;
              idx_t ncommonnodes = 2;
              std::vector<real_t> tpwgts(ncon * nparts, 1. / (ncon * nparts));

              std::vector<real_t> ubvec{1.01};
              std::vector<idx_t> options{0};
              idx_t edgecut;
              std::vector<idx_t> part(vtxdist[mpi_rank+1]-vtxdist[mpi_rank]);

              int err = ParMETIS_V3_PartKway(
                  &vtxdist[0], &xadj[0], &adjncy[0], &vwgt[0], adjwgt, &wgtflag,
                  &numflag, &ncon, &nparts, &tpwgts[0], &ubvec[0], &options[0], &edgecut,
                  &part[0], &comm);
              return std::make_pair(edgecut, part);
              // if (err == METIS_ERROR) {
              //    return s-1;
              //} else {
              //    return edgecut;
              //}
          }, py::arg("vtxdist"), py::arg("xadj"), py::arg("adjncy"), py::arg("vwgt"), py::arg("nparts")=-1);

    m.def("ParMETIS_V3_RefineKway",
          [](std::vector<idx_t> vtxdist, std::vector<idx_t> xadj,
             std::vector<idx_t> adjncy, std::vector<idx_t> vwgt, int nparts) {

              int n_vtxdist = vtxdist.size();
              int n_xadj = xadj.size();
              int n_adjncy = adjncy.size();
              //int n_vwgt = vwgt.size();
              int mpi_rank, mpi_size;
              MPI_Comm comm = MPI_COMM_WORLD;
              MPI_Comm_size(comm, &mpi_size);
              MPI_Comm_rank(comm, &mpi_rank);
              if(nparts == -1) {
                nparts = mpi_size;
              }

              idx_t *adjwgt = NULL;
              idx_t wgtflag = 2;

              idx_t numflag = 0;
              idx_t ncon = 1;
              idx_t ncommonnodes = 2;
              std::vector<real_t> tpwgts(ncon * nparts, 1. / (ncon * nparts));

              std::vector<real_t> ubvec{1.01};
              std::vector<idx_t> options{0};
              idx_t edgecut;
              std::vector<idx_t> part(vtxdist[mpi_rank+1]-vtxdist[mpi_rank]);

              int err = ParMETIS_V3_RefineKway(
                  &vtxdist[0], &xadj[0], &adjncy[0], &vwgt[0], adjwgt, &wgtflag,
                  &numflag, &ncon, &nparts, &tpwgts[0], &ubvec[0], &options[0], &edgecut,
                  &part[0], &comm);
              return std::make_pair(edgecut, part);
              // if (err == METIS_ERROR) {
              //    return s-1;
              //} else {
              //    return edgecut;
              //}
          }, py::arg("vtxdist"), py::arg("xadj"), py::arg("adjncy"), py::arg("vwgt"), py::arg("nparts")=-1);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
