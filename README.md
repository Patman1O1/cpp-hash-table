# C/C++ Project Template

## Overview
A template repository for C/C++ projects with CMake and Conan. A GitHub Action 
workflow is provided with the on `workflow_dispatch` event. This event has three input
parameters which are `project_lang`, `project_type`, and `project_namespace`.
`project_lang` is the language the project will be generated in. As the name of this
template repo might suggest, the only available options are C and C++. For
`project_type`, one can choose from Executable, Static Library, Shared Library, or
Interface Library. The last  parameter, `project_namespace`, is optional and defaults 
to the name of the GitHub repository name. 

## Template Parameters: `project_lang`
`project_lang` is the language the template will be generated in. As the name of this
template repository might suggest, the only available options are C and C++

## Template Parameters: `project_type`
`project_type` determines how the project will be generated. The supported types are
Executable, Static Library, Shared Library, and Interface Library.

### Project Types: Executable

#### C Executable Project File Tree
```text
.
├── src/
│   ├── CMakeLists.txt
│   └── <project_name>.c
├── test_package/
│   ├── src/
│   │   ├── CMakeLists.txt
│   │   └── main.c
│   ├── CMakeLists.txt
│   └── conanfile.py
├── tests/
│   ├── CMakeLists.txt
│   └── <project_name>_test.cpp
├── .clang-format
├── .clang-tidy
├── .gitignore
├── CMakeLists.txt
├── conanfile.py
├── LICENSE
└── README.md
```

#### C++ Executable Project File Tree
```text
.
├── src/
│   ├── CMakeLists.txt
│   └── <project_name>.cpp
├── test_package/
│   ├── src/
│   │   ├── CMakeLists.txt
│   │   └── main.cpp
│   ├── CMakeLists.txt
│   └── conanfile.py
├── tests/
│   ├── CMakeLists.txt
│   └── <project_name>_test.cpp
├── .clang-format
├── .clang-tidy
├── .gitignore
├── CMakeLists.txt
├── conanfile.py
├── LICENSE
└── README.md
```

### Project Types: Static Library
#### C Static Project File Tree
```text
.
├── include/
│   └── <project_namespace>/
│       ├── <project_name>.h
│       └── export.h
├── src/
│   ├── CMakeLists.txt
│   └── <project_name>.c
├── test_package/
│   ├── src/
│   │   ├── CMakeLists.txt
│   │   └── main.c
│   ├── CMakeLists.txt
│   └── conanfile.py
├── tests/
│   ├── CMakeLists.txt
│   └── <project_name>_test.cpp
├── .clang-format
├── .clang-tidy
├── .gitignore
├── CMakeLists.txt
├── conanfile.py
├── LICENSE
└── README.md
```

#### C++ Static Project File Tree
```text
.
├── include/
│   └── <project_namespace>/
│       ├── <project_name>.hpp
│       └── export.hpp
├── src/
│   ├── CMakeLists.txt
│   └── <project_name>.cpp
├── test_package/
│   ├── src/
│   │   ├── CMakeLists.txt
│   │   └── main.cpp
│   ├── CMakeLists.txt
│   └── conanfile.py
├── tests/
│   ├── CMakeLists.txt
│   └── <project_name>_test.cpp
├── .clang-format
├── .clang-tidy
├── .gitignore
├── CMakeLists.txt
├── conanfile.py
├── LICENSE
└── README.md
```

### Project Types: Shared Library

#### C Shared Library Project File Tree
```text
.
├── include/
│   └── <project_namespace>/
│       ├── <project_name>.h
│       └── export.h
├── src/
│   ├── CMakeLists.txt
│   └── <project_name>.c
├── test_package/
│   ├── src/
│   │   ├── CMakeLists.txt
│   │   └── main.c
│   ├── CMakeLists.txt
│   └── conanfile.py
├── tests/
│   ├── CMakeLists.txt
│   └── <project_name>_test.cpp
├── .clang-format
├── .clang-tidy
├── .gitignore
├── CMakeLists.txt
├── conanfile.py
├── LICENSE
└── README.md
```

#### C++ Shared Library Project File Tree
```text
.
├── include/
│   └── <project_namespace>/
│       ├── <project_name>.hpp
│       └── export.hpp
├── src/
│   ├── CMakeLists.txt
│   └── <project_name>.cpp
├── test_package/
│   ├── src/
│   │   ├── CMakeLists.txt
│   │   └── main.cpp
│   ├── CMakeLists.txt
│   └── conanfile.py
├── tests/
│   ├── CMakeLists.txt
│   └── <project_name>_test.cpp
├── .clang-format
├── .clang-tidy
├── .gitignore
├── CMakeLists.txt
├── conanfile.py
├── LICENSE
└── README.md
```

### Project Types: Interface Library
#### C Interface Library Project File Tree
```text
.
├── include/
│   └── <project_namespace>/
│       └── <project_name>.h
├── test_package/
│   ├── src/
│   │   ├── CMakeLists.txt
│   │   └── main.c
│   ├── CMakeLists.txt
│   └── conanfile.py
├── tests/
│   ├── CMakeLists.txt
│   └── <project_name>_test.cpp
├── .clang-format
├── .clang-tidy
├── .gitignore
├── CMakeLists.txt
├── conanfile.py
├── LICENSE
└── README.md
```

#### C++ Interface Library Project File Tree
```text
.
├── include/
│   └── <project_namespace>/
│       └── <project_name>.hpp
├── test_package/
│   ├── src/
│   │   ├── CMakeLists.txt
│   │   └── main.cpp
│   ├── CMakeLists.txt
│   └── conanfile.py
├── tests/
│   ├── CMakeLists.txt
│   └── <project_name>_test.cpp
├── .clang-format
├── .clang-tidy
├── .gitignore
├── CMakeLists.txt
├── conanfile.py
├── LICENSE
└── README.md
```

## Template Parameters: `project_namespace`
