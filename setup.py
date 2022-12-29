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
        libraries=[
            "roq-client",
            "fmt",
        ],
    ),
]

extra_compile_args = sysconfig.get_config_var("CFLAGS").split()

if sys.platform == "darwin":
    extra_compile_args += ["-DFMT_USE_NONTYPE_TEMPLATE_ARGS=1"]

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
    extra_compile_args=extra_compile_args,
)
