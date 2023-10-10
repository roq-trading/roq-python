import os
import sys

from glob import glob

from pybind11 import get_cmake_dir

from pybind11.setup_helpers import ParallelCompile, Pybind11Extension, build_ext

from setuptools import setup

ParallelCompile("NPY_NUM_BUILD_JOBS").install()

__version__ = os.environ.get("GIT_DESCRIBE_TAG", "0.0.0")

extra_compile_args = []
if sys.platform == "darwin":
    extra_compile_args += ["-DFMT_USE_NONTYPE_TEMPLATE_ARGS=1"]


ext_modules = [
    Pybind11Extension(
        "roq",
        sorted(
            glob(
                "src/**/*.cpp",
                recursive=True,
            )
        ),
        define_macros=[
            ("VERSION_INFO", __version__),
        ],
        cxx_std=20,
        include_dirs=[
            "src/",
        ],
        libraries=[
            "fmt",
            "roq-client",
            "roq-codec",
            "roq-io",
        ],
        extra_compile_args=extra_compile_args,
    ),
]

setup(
    name="roq",
    version=__version__,
    author="Hans Erik Thrane",
    author_email="thraneh@gmail.com",
    url="https://roq-trading.com/",
    description="Roq Python Tools",
    long_description="",
    ext_modules=ext_modules,
    extras_require={
        "test": "pytest",
    },
    cmdclass={
        "build_ext": build_ext,
    },
    zip_safe=False,
    python_requires=">=3.10",
)
