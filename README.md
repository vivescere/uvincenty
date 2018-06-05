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

## Benchmarks
Using ipython:
>> import vincenty, uvincenty, geopy.distance, geographiclib.geodesic
>> boston = (42.3541165, -71.0693514)
>> newyork = (40.7791472, -73.9680804)

>> %timeit geographiclib.geodesic.Geodesic.WGS84.Inverse(*boston, *newyork)['s12'] / 1000
200 µs ± 8.38 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)

>> %timeit geopy.distance.vincenty(boston, newyork).kilometers
42.4 µs ± 4.45 µs per loop (mean ± std. dev. of 7 runs, 10000 loops each)

>> %timeit vincenty.vincenty(boston, newyork)
18.3 µs ± 430 ns per loop (mean ± std. dev. of 7 runs, 100000 loops each)

>> %timeit uvincenty.vincenty(*boston, *newyork)
885 ns ± 7.74 ns per loop (mean ± std. dev. of 7 runs, 1000000 loops each)
