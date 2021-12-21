if [ "$#" -ne 1 ];  then
    echo -e "Usage: $0 <number of vertices>" >&2
    exit
fi

num_vertices="$1"

random_pts="points${num_vertices}.node"
node_file="points${num_vertices}.1.node"
ele_file="points${num_vertices}.1.ele"
neigh_file="points${num_vertices}.1.neigh"
pg_file="points${num_vertices}.1.pg"
output_hedge_file="points${num_vertices}.1.hedge"
output_pdf_original="points${num_vertices}_original.1.pdf"
#Generating random points
echo -n "Generating data\n"
python3 10000x10000RandomPoints.py $1 > $random_pts
echo -n "Done.\n"

echo -n "Generating triangulation\n"
triangle -n $random_pts
echo -n "Done.\n"

echo -n "Generating plannar graph\n"
g++ generateplannar.cpp -o generateplannar
./generateplannar ${node_file} ${ele_file} ${neigh_file} ${pg_file} ${output_hedge_file}
echo -n "Done.\n"

#echo -n "plotting triangle original mesh\n"
#python3 plot_triangulation.py ${node_file} ${output_hedge_file} ${output_pdf_original}
#echo -n "Done.\n"

rm ${output_hedge_file}
rm ${random_pts}
rm ${ele_file}
rm ${neigh_file}