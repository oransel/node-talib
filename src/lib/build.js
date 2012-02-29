var exec = require('child_process').exec;

console.log('building talib functions...');
var flags = "";
if (process.platform == 'darwin') {
	var arch = process.arch == 'ia32' ? 'i386' : process.arch == 'x64' ? 'x86_64' : process.arch;
	flags = 'export CFLAGS="-arch ' + arch + '" && export LDFLAGS="-arch ' + arch + '" && ';
}
exec(flags + './configure && make');