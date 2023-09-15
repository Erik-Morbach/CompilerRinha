docker build . -f Dockerfile -t compiler-rinha

docker run -v $(pwd)/$1:/var/rinha/source.rinha.json compiler-rinha
