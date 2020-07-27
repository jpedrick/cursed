Basic coding standards:
* use clang-format-11
* "#pragma once" header guards
* forward declarations wherever possible
* composite widgets provide references to underlying components and do not provide convenience functions
* no aggregation objects for construction, constructors must take simplest types that make sense
* avoid builder objects except where abstraction makes sense. Fundamental ethos of project is to be able to build an app layout using struct initialization.
