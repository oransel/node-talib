const { exec, execSync } = require('child_process');
const fs = require('fs');
const path = require('path');

console.log('building talib functions...');

if (process.platform === 'win32') {
  const arch = process.arch === 'x64' ? 'x64' : 'Win32';
  
  // Try to find MSBuild using vswhere (modern approach)
  let msbuildPath;
  try {
    const vswherePath = 'C:\\Program Files (x86)\\Microsoft Visual Studio\\Installer\\vswhere.exe';
    if (fs.existsSync(vswherePath)) {
      const vsPath = execSync(
        `"${vswherePath}" -latest -requires Microsoft.Component.MSBuild -property installationPath`,
        { encoding: 'utf8' }
      ).trim();
      
      // Try different MSBuild versions
      const msbuildPaths = [
        path.join(vsPath, 'MSBuild\\Current\\Bin\\MSBuild.exe'),
        path.join(vsPath, 'MSBuild\\15.0\\Bin\\MSBuild.exe')
      ];
      
      for (const p of msbuildPaths) {
        if (fs.existsSync(p)) {
          msbuildPath = `"${p}"`;
          break;
        }
      }
    }
  } catch (e) {
    // vswhere not found or failed
  }
  
  // Fallback to older MSBuild locations
  if (!msbuildPath) {
    const possiblePaths = [
      'C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\MSBuild\\Current\\Bin\\MSBuild.exe',
      'C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\MSBuild\\Current\\Bin\\MSBuild.exe',
      'C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe',
      'C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\MSBuild\\Current\\Bin\\MSBuild.exe',
      'C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Professional\\MSBuild\\Current\\Bin\\MSBuild.exe',
      'C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe',
      'C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Enterprise\\MSBuild\\15.0\\Bin\\MSBuild.exe',
      'C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Professional\\MSBuild\\15.0\\Bin\\MSBuild.exe',
      'C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\MSBuild\\15.0\\Bin\\MSBuild.exe',
      'C:\\Program Files (x86)\\MSBuild\\14.0\\Bin\\MSBuild.exe',
      'C:\\Program Files\\MSBuild\\14.0\\Bin\\MSBuild.exe'
    ];
    
    for (const p of possiblePaths) {
      if (fs.existsSync(p)) {
        msbuildPath = `"${p}"`;
        break;
      }
    }
  }
  
  if (!msbuildPath) {
    console.error('MSBuild not found. Please install Visual Studio Build Tools.');
    process.exit(1);
  }

  const makeDir = path.join(__dirname, 'make/csr/windows/msbuild/');
  process.chdir(makeDir);
  
  // Upgrade the solution to use the current toolset
  const buildCmd = `${msbuildPath} ./ta_lib.sln /t:Rebuild /property:Configuration=csr /property:Platform=${arch} /property:PlatformToolset=v143 /property:WindowsTargetPlatformVersion=10.0 /verbosity:minimal`;
  console.log(`Running: ${buildCmd}`);
  
  exec(buildCmd, (err, stdout, stderr) => {
    if (err) {
      console.error('Build failed:', err);
      console.error('stdout:', stdout);
      console.error('stderr:', stderr);
      process.exit(1);
    }
    console.log(stdout);
    if (stderr) console.error(stderr);
  });
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
  const makeDir = path.join(__dirname, 'make/csr/linux/g++/');
  process.chdir(makeDir);
  exec(`${flags}make`, (err, stdout, stderr) => {
    if (err) {
      console.error('Build failed:', err);
      process.exit(1);
    }
    console.log(stdout);
    if (stderr) console.error(stderr);
  });
}
