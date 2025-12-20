const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

console.log('building talib functions...');

if (process.platform === 'win32') {
  let msbuildPath;
  let frameworkPath;
  let arch;

  if (process.arch === 'x64') {
    msbuildPath = '"C:/Program Files (x86)/MSBuild/14.0/Bin/MSBuild.exe"';
    frameworkPath = 'C:/Program Files (x86)/Reference Assemblies/Microsoft/Framework/.NETFramework';
    arch = 'x64';
  } else {
    msbuildPath = '"C:/Program Files/MSBuild/14.0/Bin/MSBuild.exe"';
    frameworkPath = 'C:/Program Files/Reference Assemblies/Microsoft/Framework/.NETFramework';
    arch = 'Win32';
  }

  const frameworkVersions = fs.readdirSync(frameworkPath).map((val) => ({
    index: val,
    path: path.join(frameworkPath, val)
  }));

  if (!frameworkVersions.length) {
    console.error('No .NETFramework versions installed!');
    process.exit(1);
  }

  frameworkVersions.sort((a, b) => (a.index > b.index ? 1 : a.index < b.index ? -1 : 0));
  const frameworkVersion = frameworkVersions.pop().path;

  process.chdir('./src/lib/make/csr/windows/msbuild/');
  exec(
    `${msbuildPath} ./ta_lib.sln /p:FrameworkPathOverride="${frameworkVersion}" /property:Configuration=csr /property:Platform=${arch}`,
    (err, stdout, stderr) => {
      if (err) {
        console.error('Build failed:', err);
        process.exit(1);
      }
      console.log(stdout);
      if (stderr) console.error(stderr);
    }
  );
} else if (process.platform === 'freebsd') {
  if (fs.existsSync('/usr/local/lib/libta_lib.a')) {
    console.log('package devel/ta-lib is installed. No need to build talib functions.');
  } else {
    console.error('Please install ta-lib from ports collection: pkg install devel/ta-lib');
    process.exit(1);
  }
} else {
  let flags = '';
  if (process.platform === 'darwin') {
    const arch = process.arch === 'ia32' ? 'i386' : process.arch === 'x64' ? 'x86_64' : process.arch;
    flags = `MACOSX_DEPLOYMENT_TARGET=10.7 export CFLAGS="-arch ${arch}" && export LDFLAGS="-arch ${arch}" && `;
  }
  process.chdir('./src/lib/make/csr/linux/g++/');
  exec(`${flags}make`, (err, stdout, stderr) => {
    if (err) {
      console.error('Build failed:', err);
      process.exit(1);
    }
    console.log(stdout);
    if (stderr) console.error(stderr);
  });
}
