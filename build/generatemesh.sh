if [ "$#" -ne 1 ];  then
    echo -e "Usage: $0 <number of vertices>" >&2
    exit
fi

file_folder="../data/"

num_vertices="$1"
node_file="points${num_vertices}.1.node"
new_node_file="points${num_vertices}.2.node"
pg_file="points${num_vertices}.1.pg"
output_off="points${num_vertices}.1.off"
output_hedge="points${num_vertices}.1.hedge"
output_hedge_polylla="points${num_vertices}_polylla.1.hedge"
output_pdf="points${num_vertices}.1.pdf"
output_pdf_polylla="points${num_vertices}_polylla.1.pdf"

echo -n "Generating mesh with ${num_vertices} vertices..."
cd ../data
./datagenerator.sh "${num_vertices}"
cd ../build
echo "done"

echo -n "Generating mesh..."
make && ./compressPolylla ${file_folder}${node_file} ${file_folder}${pg_file} ${file_folder}${output_off} ${file_folder}${output_hedge} ${file_folder}${output_hedge_polylla} ${file_folder}${new_node_file} 
echo "done"


echo -n "plotting polyllamesh\n"
cd ../data
python3 plot_triangulation.py ${new_node_file} ${output_hedge} ${output_pdf}
python3 plot_triangulation.py ${new_node_file} ${output_hedge_polylla} ${output_pdf_polylla}
cd ../build
echo -n "Done.\n"

#rm ${file_folder}${output_hedge} ${file_folder}${output_hedge_polylla}