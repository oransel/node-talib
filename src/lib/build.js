var exec = require('child_process').exec;

console.log('building talib functions...');

if (process.platform == 'win32') {
  var msbuildPath = '"C:/Program Files';
  if(process.arch == 'x64') {
    msbuildPath += ' (x86)'
  }

  msbuildPath += '/MSBuild/14.0/Bin/';

  process.chdir('./src/lib/make/csr/windows/msbuild/');

  exec(msbuildPath + 'MSBuild.exe" ./ta_lib.sln /property:Configuration=csr /property:Platform=x64', function(err, stdout, stderr) {
    console.log(stdout, stderr);
  });
} else {
  var flags = '';
  if (process.platform == 'darwin') {
    var arch = process.arch == 'ia32' ? 'i386' : process.arch == 'x64' ? 'x86_64' : process.arch;
    flags = 'MACOSX_DEPLOYMENT_TARGET=10.7 export CFLAGS="-arch ' + arch + '" && export LDFLAGS="-arch ' + arch + '" && ';
  }
  process.chdir('./src/lib/make/csr/linux/g++/');
  exec(flags + 'make');
}
