#!/usr/bin/env python3

import setuptools
import platform


def main():

    packages = setuptools.find_packages()
    print(packages)

    setuptools.setup(
        name="imposter",
        version="0.1",
        author="Ralf Waspe",
        author_email="rwaspe@me.com",
        description="interact with the daisy patch imposter",
        license='LGPL',
        packages=packages,
        install_requires=['pyside6'],
        include_package_data=True,
        zip_safe=False,
    )


if __name__ == '__main__':

    main()
