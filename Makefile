# Alternate way of building node-talib module
# Similar results can be achieved with npm

all:
	@cd ./src/lib && node build.js && cd ../../ && node-gyp configure && node-gyp build
	@echo
	@echo build complete
	
clean:
	@rm -rf build/* && cd ./src/lib && node clean.js && cd ../../
	@echo
	@echo clean complete