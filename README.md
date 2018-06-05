# UVincenty

A fast implementation of the inverse vincenty formula, using a C extension.

This code is heavily inspired from the [vincenty package](https://github.com/maurycyp/vincenty).

## Usage
```
import uvincenty
boston = (42.3541165, -71.0693514)
newyork = (40.7791472, -73.9680804)
distance = uvincenty.vincenty(*boston, *newyork)
```

## Installation
```bash
pip install uvincenty
```
