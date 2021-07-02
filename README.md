# MFCC-WASM

A small and fast Mel Frequency Cepstral Coefficient calculator written in C.
Intended to be compiled to WASM and used in JavaScript.

Using KissFFT (BSD 3-clause) and libmfcc (MIT).

The module exports default async function that takes configuration parameters:

``` javascript
options = {
  frameSize:  480,
  sampleRate: 16000,
};
```

The function resolves to `MFCC` instance which has 2 methods.

``` javascript
mfcc.destroy();
mfcc.getMFCC(frame, filterBanks, m);
```

`destroy()` free resources used by this instance in c.
`getMFCC()` takes audio frame buffer as the first argument. It can be any sequence of numbers.
It's converted to `Float64Array` internally.

`filterBanks` and `m` are integers and optional. `m` is the number of coefficients to calculate.
This method returns `Float64Array` containing the coefficients.
