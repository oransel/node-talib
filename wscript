import sys
import subprocess

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")

def node_arch():
  if sys.platform != 'darwin':
    return
  cmd = [ 'node', '-e', 'console.log(process.arch)' ]
  p = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  out = p.communicate()[0].split('\n')[0]
  return out

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon") 
  flags = ["-g","-O3","-Wall","-D_LARGEFILE_SOURCE"]
  arch = node_arch()
  arch_mappings = {'ia32': 'i386', 'x64': 'x86_64'}
  if arch in arch_mappings:
    arch = arch_mappings[arch]
    flags += ['-arch', arch]
  obj.cxxflags = flags
  obj.ccflags = flags
  obj.linkflags = flags
  obj.target = "talib"
  obj.source = "src/talib.cpp"
  obj.add_obj_file("src/lib/src/.libs/libta_lib.a"); 
