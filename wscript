import Options, Utils
from os import unlink, symlink, popen
from os.path import exists

blddir = 'build'
VERSION = '0.0.1'
sources = [
    "./src/cpp/binding.cpp",
    "./src/cpp/geometry.cpp",
    "./src/cpp/wktreader.cpp",
    "./src/cpp/wktwriter.cpp",
    "./src/cpp/geometryfactory.cpp",
    "./src/cpp/precisionmodel.cpp",
    "./src/cpp/quadtree.cpp",
    "./src/cpp/geojsonwriter.cpp"
 ]

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')

  geos_config = conf.find_program('geos-config', var='GEOS_CONFIG', mandatory=True)
  geos_ldflags = popen("%s --ldflags" % geos_config).readline().strip()[2:]
  conf.env.append_value("LIBPATH_GEOS", geos_ldflags)
  conf.env.append_value("LIB_GEOS", "geos")
  geos_includes = popen("%s --includes" % geos_config).readline().strip()
  conf.env.append_value("CPPPATH_GEOS", geos_includes)

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.cxxflags = ["-g", "-D_LARGEFILE_SOURCE", "-Wall"]
  obj.target = 'geos'
  obj.source = sources
  obj.uselib = "GEOS"
