node_file=$1
pg_file=$2

new_node_file="output.2.node"
output_off="output.1.off"
output_hedge="output.1.hedge"
output_hedge_polylla="output_polylla.1.hedge"
output_pdf="output.1.pdf"
output_pdf_polylla="output_polylla.1.pdf"

file_folder="../data/"


echo -n "Generating mesh..."
make && ./compressPolylla ${node_file} ${pg_file} ${file_folder}${output_off} ${file_folder}${output_hedge} ${file_folder}${output_hedge_polylla} ${file_folder}${new_node_file} 
echo "done"


echo -n "plotting polyllamesh\n"
cd ../data
python3 plot_triangulation.py ${new_node_file} ${output_hedge} ${output_pdf}
python3 plot_triangulation.py ${new_node_file} ${output_hedge_polylla} ${output_pdf_polylla}
cd ../build
echo -n "Done.\n"

#rm ${file_folder}${output_hedge} ${file_folder}${output_hedge_polylla}