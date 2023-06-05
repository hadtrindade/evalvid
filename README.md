
# Este repositório é um clone do [aphirak/evalvid-tool](https://github.com/aphirak/evalvid-tool) com algumas alterações.


* [EvalVid - A Framework for Video Transmission and Quality Evaluation (2003)](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.10.3918)
* Project homepage -
  [http://www2.tkn.tu-berlin.de/research/evalvid/](http://www2.tkn.tu-berlin.de/research/evalvid/)


# Instalação.

## Passo 1 - Buildar imagem Docker do Evalvid:
```bash
make docker-build
```

## Passo 2 - Inciar container:
```bash
make docker-run
```

## Passo 3 - Complilar Evalvid:

```bash
make all
```