# IR and Probabilities

Q++ extends LLVM IR with metadata describing probabilistic branches. Each
measurement inserts a `@collapse` marker so that the runtime knows when to
resolve deferred scopes. This metadata also allows external simulators to
reconstruct the full quantum state when running offline analyses.
