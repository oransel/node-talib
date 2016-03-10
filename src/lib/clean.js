var exec = require('child_process').exec;

console.log('cleaning talib...');

if(process.platform == 'win32') {
	var msbuildPath = '"C:/Windows/Microsoft.NET/Framework64/v4.0.30319/';

  process.chdir('./src/lib/make/csr/windows/msbuild/');

  exec(msbuildPath + 'MSBuild.exe" ./ta_lib.sln /target:Clean', function(err, stdout, stderr) {
    console.log(stdout, stderr);
	});
} else {
var flags = "";
	if (process.platform == 'darwin') {
		var arch = process.arch == 'ia32' ? 'i386' : process.arch == 'x64' ? 'x86_64' : process.arch;
		flags = 'export CFLAGS="-arch ' + arch + '" && export LDFLAGS="-arch ' + arch + '" && ';
	}
	process.chdir('./src/lib/make/csr/linux/g++/');
	exec('rm -rf ./build && rm -rf ./node_modules && ' + flags + 'make clean');
}
