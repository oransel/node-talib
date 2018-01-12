var exec = require('child_process').exec;
var fs = require('fs');

console.log('building talib functions...');

if (process.platform == 'win32') {

  let msbuildPath;
  let frameworkPath;
  let arch;

  if (process.arch == 'x64') {
    msbuildPath = '"C:/Program Files (x86)/MSBuild/14.0/Bin/MSBuild.exe"';
    frameworkPath = 'C:/Program Files (x86)/Reference Assemblies/Microsoft/Framework/.NETFramework';
    arch = 'x64';
  } else {
    msbuildPath = '"C:/Program Files/MSBuild/14.0/Bin/MSBuild.exe"';
    frameworkPath = 'C:/Program Files/Reference Assemblies/Microsoft/Framework/.NETFramework';
    arch = 'Win32';
  }

  var frameworkVersions = fs.readdirSync(frameworkPath).map(function(val, i){
    return { index: val, path: frameworkPath + '/' + val };
  });

  if(!frameworkVersions.length){
    return console.error('No .NETFramework versions installed!');
  } else {
    frameworkVersions.sort(function(a,b){
      return (a.index > b.index) ? 1 : ((a.index < b.index) ? -1 : 0);
    });
    frameworkVersion = frameworkVersions.pop().path;
  }

  process.chdir('./src/lib/make/csr/windows/msbuild/');
  exec(msbuildPath + ' ./ta_lib.sln /p:FrameworkPathOverride="' + frameworkVersion + '" /property:Configuration=csr /property:Platform=' + arch, function(err, stdout, stderr) {
    console.log(stdout, stderr);
  });

} else {

  var flags = '';
  if (process.platform == 'darwin') {
    var arch = process.arch == 'ia32' ? 'i386' : process.arch == 'x64' ? 'x86_64' : process.arch;
    flags = 'MACOSX_DEPLOYMENT_TARGET=10.7 export CFLAGS="-arch ' + arch + '" && export LDFLAGS="-arch ' + arch + '" && ';
  }
  process.chdir('./src/lib/make/csr/linux/g++/');
  exec(flags + 'make', function(err, stdout, stderr) {
    console.log(stdout, stderr);
  });

}
