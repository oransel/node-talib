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
        `"${vswherePath}" -products * -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`,
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
  
  // Fallback to scanning standard MSBuild locations.
  // Covers all editions (including BuildTools, which the standalone Build
  // Tools installer and the Chocolatey/winget packages use) for VS 2017+.
  if (!msbuildPath) {
    const roots = [
      'C:\\Program Files\\Microsoft Visual Studio',
      'C:\\Program Files (x86)\\Microsoft Visual Studio'
    ];
    const years = ['2022', '2019', '2017'];
    const editions = ['Enterprise', 'Professional', 'Community', 'BuildTools'];
    const possiblePaths = [];
    for (const root of roots) {
      for (const year of years) {
        for (const edition of editions) {
          if (year === '2017') {
            possiblePaths.push(`${root}\\${year}\\${edition}\\MSBuild\\15.0\\Bin\\MSBuild.exe`);
          } else {
            possiblePaths.push(`${root}\\${year}\\${edition}\\MSBuild\\Current\\Bin\\MSBuild.exe`);
          }
        }
      }
    }
    possiblePaths.push(
      'C:\\Program Files (x86)\\MSBuild\\14.0\\Bin\\MSBuild.exe',
      'C:\\Program Files\\MSBuild\\14.0\\Bin\\MSBuild.exe'
    );

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

  // Map the VS release year in the MSBuild path to the MSVC platform toolset,
  // so builds also work when MSBuild comes from VS 2019/2017 (v143 is VS 2022-only).
  const yearMatch = msbuildPath.match(/Visual Studio\\(\d{4})\\/);
  const vsYear = yearMatch ? parseInt(yearMatch[1], 10) : 0;
  let toolset = 'v143'; // default: VS 2022 toolset
  if (vsYear === 2017) toolset = 'v141';
  else if (vsYear === 2019) toolset = 'v142';

  const makeDir = path.join(__dirname, 'make/csr/windows/msbuild/');
  process.chdir(makeDir);
  
  // Upgrade the solution to use the detected toolset
  const buildCmd = `${msbuildPath} ./ta_lib.sln /t:Rebuild /property:Configuration=csr /property:Platform=${arch} /property:PlatformToolset=${toolset} /property:WindowsTargetPlatformVersion=10.0 /verbosity:minimal`;
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
  // The npm tarball ships prebuilt .o objects in src/lib/temp/csr/.
  // make sees them as up-to-date, skips recompilation and archives stale
  // objects -> "undefined symbol: TA_DEF_*" at dlopen. Drop them so the
  // full source tree is recompiled for the current toolchain.
  try {
    const csrDir = path.join(__dirname, 'temp/csr');
    for (const f of fs.readdirSync(csrDir)) {
      if (f.endsWith('.o') || f.endsWith('.obj')) {
        fs.unlinkSync(path.join(csrDir, f));
      }
    }
  } catch (cleanErr) {
    console.warn('prebuilt object cleanup failed (non-fatal):', cleanErr.message);
  }
  exec(`${flags}make`, (err, stdout, stderr) => {
    if (err) {
      console.error('Build failed:', err);
      process.exit(1);
    }
    console.log(stdout);
    if (stderr) console.error(stderr);
    // Newer binutils (Debian trixie, binutils >= 2.44) reject .a archives
    // without a symbol index: "error adding symbols: archive has no index".
    // Best-effort ranlib pass over the built archives fixes the link step.
    try {
      const libDir = path.join(__dirname, 'lib');
      for (const f of fs.readdirSync(libDir)) {
        if (f.endsWith('.a')) {
          console.log(`ranlib ${f}`);
          execSync(`ranlib ${path.join(libDir, f)}`);
        }
      }
    } catch (ranlibErr) {
      console.warn('ranlib pass failed (non-fatal):', ranlibErr.message);
    }
  });
}
