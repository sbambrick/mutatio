from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


class ConanLibrary(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "enable_testing": [True, False],
        "enable_coverage": [True, False],
        "enable_asan": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "enable_testing": True,
        "enable_coverage": True,
        "enable_asan": False,
    }

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("geographiclib/[^2.4]")
        self.test_requires("gtest/[^1.15]")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["ENABLE_TESTING"] = self.options.enable_testing
        tc.cache_variables["ENABLE_COVERAGE"] = self.options.enable_coverage
        tc.cache_variables["ENABLE_ASAN"] = self.options.enable_asan
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def test(self):
        cmake = CMake(self)
        cmake.test()
