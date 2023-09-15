# CompilerRinha

Interpretador C++ submetido para [rinha de compilers](https://github.com/aripiprazole/rinha-de-compiler).

## Build
```bash
./build.sh
```
Executavel fica em build/CompilerRinha

## Run

```bash
./run.sh <path-to-.json-file>
```

## Docker
Usando docker

```bash
docker build . -f Dockerfile -t compiler-rinha

docker run -v <path-to-ast.json>:/var/rinha/source.rinha.json compiler-rinha
```
ou
```bash
./docker.sh <path-to-ast.json>
```


