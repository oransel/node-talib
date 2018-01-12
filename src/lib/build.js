var exec = require('child_process').exec;
var fs = require('fs');

console.log('building talib functions...');

if (process.platform == 'win32') {
  var msbuildPath = '"C:/Program Files';
  var frameworkPathStart = 'C:/Program Files';
  var frameworkPathEnd = '/Reference Assemblies/Microsoft/Framework/.NETFramework';
  var platform = 'Win32'
  if(process.arch == 'x64') {
    msbuildPath += ' (x86)'
    platform = 'x64'
  }

  msbuildPath += '/MSBuild/14.0/Bin/';

  var stats32, stats64, frameworkVersions = [], frameworkVersion;
  try {
    var stats32 = fs.statSync(frameworkPath + frameworkPathEnd);
    frameworkVersions.concat(fs.readdirSync(frameworkPath + frameworkPathEnd).map(function(val, i){
      return { index: val, path: frameworkPathStart + frameworkPathEnd + '/' + val };
    }));
  } catch (error) {}
  try {
    var stats64 = fs.statSync(frameworkPathStart + ' (x86)' + frameworkPathEnd);
    frameworkVersions = frameworkVersions.concat(fs.readdirSync(frameworkPathStart + ' (x86)' + frameworkPathEnd).map(function(val, i){
      return { index: val, path: frameworkPathStart + ' (x86)' + frameworkPathEnd + '/' + val };
    }));
  } catch (error) {}
  
  if(!frameworkVersions.length){
    console.log('No .NETFramework versions installed!')
  } else {
    frameworkVersions.sort(function(a,b){
      return (a.index > b.index) ? 1 : ((a.index < b.index) ? -1 : 0)
    });
    frameworkVersion = frameworkVersions.pop().path;
  }

  process.chdir('./src/lib/make/csr/windows/msbuild/');
  exec(msbuildPath + 'MSBuild.exe" ./ta_lib.sln /p:FrameworkPathOverride="'+frameworkVersion+'" /property:Configuration=csr /property:Platform=' + platform, function(err, stdout, stderr) {
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
