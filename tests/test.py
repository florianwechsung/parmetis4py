import time
import numpy as np
from mpi4py import MPI
from parmetis4py import ParMETIS_V3_PartKway

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

vtxdist = [0,4,8,12]

if rank==0: xadj = [0,1,3,5,7]
if rank==1: xadj = [0,3,6,9,12]
if rank==2: xadj = [0,2,4,6,7]

if rank==0: adjncy = [6, 6,7, 7,8, 6,9]
if rank==1: adjncy = [7,9,10, 8,10,11, 0,1,3, 1,2,4]
if rank==2: adjncy = [2,5, 3,4, 4,5, 5]

if rank==0: vwgt = [1, 1, 1, 1]
if rank==1: vwgt = [2, 1, 1, 1]
if rank==2: vwgt = [5, 1, 1, 1]

nparts = 3


edgecut, part = ParMETIS_V3_PartKway(vtxdist, xadj, adjncy, vwgt, nparts)

if rank==0:
    print(part)

if rank==1:
    time.sleep(0.1)
    print(part)

if rank==2:
    time.sleep(0.2)
    print(part)
