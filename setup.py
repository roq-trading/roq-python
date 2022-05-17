import os
import sys

from pybind11 import get_cmake_dir

from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

__version__ = os.environ.get("GIT_DESCRIBE_TAG", "0.0.0")

ext_modules = [
    Pybind11Extension(
        "roq",
        ["src/main.cpp"],
        define_macros=[("VERSION_INFO", __version__)],
        cxx_std=20,
        libraries=["roq-client", "fmt",],
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
    extras_require={"test": "pytest"},
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.8",
)
