import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy


class MutatioConan(ConanFile):
    name = "mutatio"
    version = "0.1.0"
    license = "MIT"
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://github.com/stbambrick/mutatio"
    description = (
        "Type-safe C++17 library for geographic coordinate conversions (WGS84)"
    )
    topics = (
        "geodesy", "coordinates", "wgs84", "ecef", "lla", "ned", "geography"
    )
    package_type = "library"

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
    }

    generators = "CMakeDeps"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("geographiclib/[^2.6]")

    def build_requirements(self):
        self.test_requires("gtest/[^1.15]")

    def export_sources(self):
        copy(self, "CMakeLists.txt",
             self.recipe_folder, self.export_sources_folder)
        copy(self, "mutatio/src/*",
             self.recipe_folder, self.export_sources_folder)
        copy(self, "mutatio/include/**",
             self.recipe_folder, self.export_sources_folder)
        copy(self, "LICENSE",
             self.recipe_folder, self.export_sources_folder)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["MUTATIO_ENABLE_TESTING"] = False
        tc.cache_variables["MUTATIO_ENABLE_COVERAGE"] = False
        tc.cache_variables["MUTATIO_ENABLE_ASAN"] = False
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE", self.source_folder,
             os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["mutatio"]
        self.cpp_info.set_property("cmake_file_name", "mutatio")
        self.cpp_info.set_property("cmake_target_name", "mutatio::mutatio")
        self.cpp_info.requires = ["geographiclib::geographiclib"]
