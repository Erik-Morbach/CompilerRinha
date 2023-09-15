for rinha_file in tests/*.rinha; do
	rinha $rinha_file > $rinha_file.json
done
