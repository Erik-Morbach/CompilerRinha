for json_file in tests/*.json; do
docker run -v $PWD/$json_file:/var/rinha/source.rinha.json compiler-rinha
done
