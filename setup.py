from setuptools import setup, find_packages

setup(
    name="kraken-installer",
    version="0.1.0",
    packages=find_packages(),
    install_requires=[
        'PyQt5 >= 5.15',
        'psutil >= 5.8.0'
    ],
    entry_points={
        'console_scripts': [
            'kraken-installer = kraken_installer.main:main'
        ]
    },
    package_data={
        'kraken_installer': ['resources/*']
    }
)
