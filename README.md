# Compress Polylla mesh

This repository is a compress version of the algorithm Polylla using as data structure a compress halfedge generated with pemb de José Fuentes.

- To see the no compress Polylla with POO: https://github.com/ssalinasfe/Polylla-Mesh-DCEL
- To see the original code of pemb:  https://github.com/jfuentess/pemb
- To see the POO version of pemb: https://github.com/jfuentess/sdsl-lite

# Compress Polylla: Compress Polygonal meshing algorithm based on terminal-edge regions


<p align="center">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/polyllalogo2.png" width="80%">
</p>
New algorithm to generate polygonal meshes of arbitrary shape, using any kind of triangulation as input, adaptable to any kind of complex geometry, no addition of extra points and uses the classic Doubly connected edge list (Half edge data struct) easy to implement wih another programming language.

<p align="center">
<img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/pikachu_A500000_T531_V352.png" width="50%">
</p>

The algorithm needs a initial triangulation as input, any triangulations will work, in the following Figure the example of a Planar Straigh Line Graph (PSLG) with holes (left image), triangulizated (middle image) to generate a Polylla mesh (right image).

<p align="center">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/faceoriginalPSLG.png" width="30%">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/facewithtrianglesblack.png" width="30%">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/final.png" width="30%">
</p>

<p align="center">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/pikachu PLSG.png" width="30%">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/pikachutriangulization.png" width="30%">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/pikachuPolylla.png" width="30%">
</p>


This triangulation is represented as a [.node file](https://www.cs.cmu.edu/~quake/triangle.node.html) with the nodes of the triangulations and a plannar graph file (WIP)


Input commands of compress polylla are:

```
./compressPolylla <node_file .og> <graph_file .pg> <output_off.off> <output_hedge_polylla .hedge> < output_hedge_triangulation .hedge << <output_new_node .node>;

```

Hedge files are necessary to plot the mesh in matplotlib.

## Scripts

Scripts made to facilizate the process of test the algorithm:

(in data folder) To generate random points, plot triangulization with indices of vertices, plot triangulization with indices of pemp, generate polylla mesh and plot polylla. Halfedges are labeled with the index left to it

```
./generatemesh.sh <number of vertices of triangulation>
```


(in build folder)

To call Polylilla from a .node file and a .pg file

```
./generatemesh.sh <nodefilename.node> <plannargraphfilename.pg>
```

The output files are in data as the name output. The output will be an output.off file to use with geomview and an output.pdf with the image of the polylla mesh.


(in build folder) To generate random point mesh and plotting

```
./datagenerator.sh <number of vertices of triangulation>
```


Triangulazitation are generated with [triangle](https://www.cs.cmu.edu/~quake/triangle.html) with the [command -zn](https://www.cs.cmu.edu/~quake/triangle.switch.html).


## TODO

## Pemb

- [ ] Arreglar generador de datos iniciales

## Polylla

- [ ] Agregar reparation phase 

### TODO scripts

- [ ] Line 45 of plotting depends on a transpose, store edges directly as the transpose of edge vectors and remove it.
- [ ] Define a input and output folder scripts
- [ ] Define -n in plot_triangulation.py to avoid label edges and vertices
- [ ] Change name plot_triangulation.py to plot_mesh.py

