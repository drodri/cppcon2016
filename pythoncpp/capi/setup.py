from distutils.core import setup, Extension

# SET VS90COMNTOOLS=%VS140COMNTOOLS%

module1 = Extension('mymath', sources = ['mymath.c'])

setup (name = 'MyMaths',
       version = '1.0',
       description = 'This is a math package',
       ext_modules = [module1])
