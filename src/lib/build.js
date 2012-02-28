var exec = require('child_process').exec;
var arch = process.arch == 'ia32' ? 'i386' : process.arch == 'x64' ? 'x86_64' : process.arch;
console.log('building talib functions...');
exec('export CFLAGS="-arch ' + arch + '" && export LDFLAGS="-arch ' + arch + '" && ./configure && make ARCH=' + arch);