from distutils.core import setup, Extension

cvincenty = Extension(
  'uvincenty', sources=['vincenty.c'], extra_compile_args=['-O3']
)

setup(
  name='uvincenty',
  version='0.0.1',
  description='Fast inverse vincenty calculations using a C extension.',
  ext_modules=[cvincenty],
  license='Unlicense',
  url='https://github.com/vivescere/uvincenty',
  author='Vivere Discere'
)
