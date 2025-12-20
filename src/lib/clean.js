const { exec } = require('child_process');

console.log('cleaning talib...');

if (process.platform === 'win32') {
  const msbuildPath = '"C:/Windows/Microsoft.NET/Framework64/v4.0.30319/';

  process.chdir('./src/lib/make/csr/windows/msbuild/');

  exec(`${msbuildPath}MSBuild.exe" ./ta_lib.sln /target:Clean`, (err, stdout, stderr) => {
    if (err) {
      console.error('Clean failed:', err);
      process.exit(1);
    }
    console.log(stdout);
    if (stderr) console.error(stderr);
  });
} else {
  let flags = '';
  if (process.platform === 'darwin') {
    const arch = process.arch === 'ia32' ? 'i386' : process.arch === 'x64' ? 'x86_64' : process.arch;
    flags = `export CFLAGS="-arch ${arch}" && export LDFLAGS="-arch ${arch}" && `;
  }
  process.chdir('./src/lib/make/csr/linux/g++/');
  exec(`rm -rf ./build && rm -rf ./node_modules && ${flags}make clean`, (err, stdout, stderr) => {
    if (err) {
      console.error('Clean failed:', err);
      process.exit(1);
    }
    console.log(stdout);
    if (stderr) console.error(stderr);
  });
}
