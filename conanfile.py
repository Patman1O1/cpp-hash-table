from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
import os

class HashTable(ConanFile):
    name = "hash_table"
    version = "0.1.0"
    author = "Patman1O1"
    description = ""

    settings = ("os", "arch", "compiler", "build_type")
    exports_sources = ("CMakeLists.txt", "cmake/*", "include/*", "tests/*")

    options = {
        "build_tests": [True, False]
    }

    default_options = {
        "build_tests": False
    }

    def configure(self) -> None:
        if self.settings.build_type == "Debug":
	    self.options.build_tests.value = True

    def build_requirements(self) -> None:
        self.tool_requires("cmake/[>=4.3.0]")
        if bool(self.options.build_tests):
            self.test_requires("gtest/1.17.0")

    def layout(self) -> None: cmake_layout(self)

    def generate(self) -> None:
        toolchain = CMakeToolchain(self)
        toolchain.variables["BUILD_TESTS"] = bool(self.options.build_tests)
        toolchain.generate()
        CMakeDeps(self).generate()

    def build(self) -> None:
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self) -> None: CMake(self).install()

    def package_info(self) -> None:
        self.cpp_info.set_property("cmake_target_name", "collections::hash_table")
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []
